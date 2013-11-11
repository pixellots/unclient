/****************************************************************************
**
** Copyright (C) 2013 UpdatNode UG.
** Contact: code@updatenode.com
**
** This file is part of the UpdateNode Client.
**
** Commercial License Usage
** Licensees holding valid commercial UpdateNode license may use this file
** under the terms of the the Apache License, Version 2.0
** Full license description file: LICENSE.COM
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation. Please review the following information to ensure the
** GNU General Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
** Full license description file: LICENSE.GPL
**
****************************************************************************/

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
