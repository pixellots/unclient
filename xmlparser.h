#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QObject>
#include <QString>
#include <QDomDocument>

#include "update.h"
#include "message.h"
#include "config.h"

namespace Sara
{
    class XmlParser : public QObject
    {
        Q_OBJECT

        public:
        XmlParser(QObject* parent, Sara::Config* aConfig);
            ~XmlParser();

        public:
            bool parse(const QString& aXmlData);

            int getStatus();
            QString getStatusString();

        private:
            bool parseStatus();
            bool parseProduct();
            bool parseVersion();
            bool parseUpdates();
            bool parseMessages();

            Sara::ProductVersion parseVersion(QDomNode aNode);
            Sara::Update parseUpdate(QDomNode aNode);
            Sara::Message parseMessage(QDomNode aNode);
        private:
            QDomDocument* m_pDocument;

            int m_iStatus;
            QString m_strStatus;
            Sara::Config* m_pConfig;
    };
}

#endif // XMLPARSER_H
