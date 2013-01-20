#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QObject>
#include <QString>
#include <QDomDocument>

#include "update.h"
#include "message.h"

namespace Sara
{
    class XmlParser : public QObject
    {
        Q_OBJECT

        public:
            XmlParser(QObject* parent);
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
    };
}

#endif // XMLPARSER_H
