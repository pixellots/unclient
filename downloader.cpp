#include "downloader.h"

using namespace Sara;

Downloader::Downloader()
{
 connect(&manager, SIGNAL(finished(QNetworkReply*)),
         SLOT(downloadFinished(QNetworkReply*)));
}

Downloader::Downloader(const QString& aTarget)
{
    m_strTarget = aTarget;
 connect(&manager, SIGNAL(finished(QNetworkReply*)),
         SLOT(downloadFinished(QNetworkReply*)));
}

void Downloader::doDownload(const QUrl &url)
{
 QNetworkRequest request(url);
 QNetworkReply *reply = manager.get(request);

 currentDownloads.append(reply);
}

bool Downloader::saveToDisk(const QString &filename, QIODevice *data)
{
 QFile file(filename);
 if (!file.open(QIODevice::WriteOnly)) {
     fprintf(stderr, "Could not open %s for writing: %s\n",
             qPrintable(filename),
             qPrintable(file.errorString()));
     return false;
 }

 file.write(data->readAll());
 file.close();

 return true;
}

void Downloader::execute()
{
 QStringList args = QCoreApplication::instance()->arguments();
 args.takeFirst();           // skip the first argument, which is the program's name
 if (args.isEmpty()) {
     printf("Qt Download example - downloads all URLs in parallel\n"
            "Usage: download url1 [url2... urlN]\n"
            "\n"
            "Downloads the URLs passed in the command-line to the local directory\n"
            "If the target file already exists, a .0, .1, .2, etc. is appended to\n"
            "differentiate.\n");
     QCoreApplication::instance()->quit();
     return;
 }

 foreach (QString arg, args) {
     QUrl url = QUrl::fromEncoded(arg.toLocal8Bit());
     doDownload(url);
 }
}

void Downloader::downloadFinished(QNetworkReply *reply)
{
 QUrl url = reply->url();
 if (reply->error()) {
     fprintf(stderr, "Download of %s failed: %s\n",
             url.toEncoded().constData(),
             qPrintable(reply->errorString()));
 } else {
     QString filename = m_strTarget;
     if (saveToDisk(filename, reply))
         printf("Download of %s succeeded (saved to %s)\n",
                url.toEncoded().constData(), qPrintable(filename));
 }

 currentDownloads.removeAll(reply);
 reply->deleteLater();

 emit done();
}
