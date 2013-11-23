#ifndef TEXTBROWSER_H
#define TEXTBROWSER_H

#include <QTextBrowser>
#include "downloader.h"

namespace UpdateNode
{
    class TextBrowser : public QTextBrowser
    {
        Q_OBJECT
        public:
            TextBrowser(QWidget *parent = 0);
            ~TextBrowser();

            void loadHtml(const QUrl& address);

        protected:
            QVariant loadResource( int type, const QUrl & name );

        signals:

        public slots:
            void done(QByteArray array, const QString& fileName);

        private:
            UpdateNode::Downloader m_oDownloader;
            QList<QString> m_oDownloadList;

    };
}
#endif // TEXTBROWSER_H
