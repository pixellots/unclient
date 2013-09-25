#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QObject>
#include <QString>
#include <QDomDocument>

#include "update.h"
#include "message.h"
#include "config.h"

namespace UpdateNode
{
    class XmlParser : public QObject
    {
        Q_OBJECT

        public:
        XmlParser(QObject* parent, UpdateNode::Config* aConfig);
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

            UpdateNode::ProductVersion parseVersion(QDomNode aNode);
            UpdateNode::Update parseUpdate(QDomNode aNode);
            UpdateNode::Message parseMessage(QDomNode aNode);
        private:
            QDomDocument* m_pDocument;

            int m_iStatus;
            QString m_strStatus;
            UpdateNode::Config* m_pConfig;
    };
}

#endif // XMLPARSER_H
