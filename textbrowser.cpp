#include "textbrowser.h"
#include "localfile.h"

#include <QDebug>
#include <QTimer>
#include <QLayout>
#include <QTemporaryFile>

using namespace UpdateNode;

TextBrowser::TextBrowser(QWidget *parent) :
    QTextBrowser(parent)
{
    connect( &m_oDownloader, SIGNAL(done(QByteArray, const QString&)), this, SLOT(done(QByteArray, const QString&)));
}

TextBrowser::~TextBrowser()
{
    foreach(QString item, m_oDownloadList)
        QFile::remove(item);
}

QVariant TextBrowser::loadResource( int type, const QUrl & name )
{
    switch(type)
    {
        case QTextDocument::ImageResource:
        {
            QString fileName = UpdateNode::LocalFile::getDownloadPath() + "/cache/" + QFileInfo(name.toString()).fileName();
            if(QFile::exists(fileName))
                return QPixmap(fileName);
            else if(!m_oDownloadList.contains(fileName))
            {
                m_oDownloadList.append(fileName);
                m_oDownloader.doDownload(name, fileName);
            }
            return QTextBrowser::loadResource(type, name);
        }
        default:
            return QTextBrowser::loadResource(type, name);
    }
}

void TextBrowser::done(QByteArray array, const QString& fileName)
{
    if(!array.isEmpty())
    {
        QImage image = QImage::fromData(array);

        if(image.save(fileName))
            setHtml(toHtml());
    }
}

