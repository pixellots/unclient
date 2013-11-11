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

#include <stdlib.h>
#include <QString>
#include "logging.h"
#include <QDomElement>
#include "xmlparser.h"

using namespace UpdateNode;

XmlParser::XmlParser(QObject* parent, UpdateNode::Config* aConfig)
    : QObject(parent)
{
    m_pDocument = NULL;
    m_pConfig = aConfig;
}

XmlParser::~XmlParser()
{
    if(m_pDocument)
        delete m_pDocument;
}

bool XmlParser::parse(const QString& aXmlData)
{
    QString errorMsg;
    int errorLine;
    int errorColumn;

    m_pDocument = new QDomDocument();

    if(m_pDocument->setContent(aXmlData, &errorMsg, &errorLine, &errorColumn))
    {
        if(!parseStatus() || !parseProduct() || !parseVersion() || !parseUpdates() || !parseMessages())
            return false;
    }
    else
    {
        UpdateNode::Logging() << "ERROR: " << errorMsg << "(Line " << errorLine << " - Column " << errorColumn << ")";
        return false;
    }

    return true;
}

bool XmlParser::parseStatus()
{
    QDomNodeList list = m_pDocument->elementsByTagName("status");

    if(list.isEmpty())
        return false;

    QDomNode status = list.at(0);

    QDomNode n = status.firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement();

        if(e.tagName()=="code")
            m_iStatus = e.text().toInt();
        else if(e.tagName()=="message")
            m_strStatus = e.text();

        n = n.nextSibling();
    }

    return true;
}

bool XmlParser::parseProduct()
{
    QDomNodeList list = m_pDocument->elementsByTagName("product");

    if(list.isEmpty())
        return false;

    UpdateNode::Product product;

    QDomNode productNode = list.at(0);

    QDomNode n = productNode.firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement();

        if(e.tagName()=="code")
            product.setCode(e.text());
        else if(e.tagName()=="name")
            product.setName(e.text());
        else if(e.tagName()=="image")
            product.setIconUrl(e.text());

        n = n.nextSibling();
    }

    m_pConfig->setProduct(product);

    return true;
}

UpdateNode::ProductVersion XmlParser::parseVersion(QDomNode aNode)
{
    UpdateNode::ProductVersion version;

    QDomNode n = aNode.firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement();

        if(e.tagName()=="code")
            version.setCode(e.text());
        else if(e.tagName()=="name")
            version.setName(e.text());
        else if(e.tagName()=="version")
            version.setVersion(e.text());

        n = n.nextSibling();
    }

    return version;
}

bool XmlParser::parseVersion()
{
    QDomNodeList list = m_pDocument->elementsByTagName("version");

    if(list.isEmpty())
        return false;

    QDomNode versionNode = list.at(0);

    UpdateNode::ProductVersion version = parseVersion(versionNode);

    m_pConfig->setVersion(version);

    return true;
}

UpdateNode::Update XmlParser::parseUpdate(QDomNode aNode)
{
    UpdateNode::Update update;

    QDomNode n = aNode.firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement();

        if(e.tagName()=="title")
            update.setTitle(e.text());
        else if(e.tagName()=="code")
            update.setCode(e.text());
        else if(e.tagName()=="description")
            update.setDescription(e.text());
        else if(e.tagName()=="type")
            update.setType(e.text().toInt());
        else if(e.tagName()=="file")
            update.setDownloadLink(e.text());
        else if(e.tagName()=="command")
            update.setCommand(e.text());
        else if(e.tagName()=="commandline")
            update.setCommandLine(e.text());
        else if(e.tagName()=="requires_admin")
            update.setRequiresAdmin(e.text().toInt()==1);
        else if(e.tagName()=="file_size")
            update.setFileSize(e.text());
        else if(e.tagName()=="target")
            update.setTargetVersion(parseVersion(n));

        n = n.nextSibling();
    }

    return update;
}

UpdateNode::Message XmlParser::parseMessage(QDomNode aNode)
{
    UpdateNode::Message message;

    QDomNode n = aNode.firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement();

        if(e.tagName()=="title")
            message.setTitle(e.text());
        else if(e.tagName()=="code")
            message.setCode(e.text());
        else if(e.tagName()=="message")
            message.setMessage(e.text());
        else if(e.tagName()=="link")
            message.setLink(e.text());

        n = n.nextSibling();
    }

    return message;
}

bool XmlParser::parseUpdates()
{
    QDomNodeList list = m_pDocument->elementsByTagName("updates");

    if(list.isEmpty())
        return false;

    QDomNode n = list.at(0).firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement();

        if(e.tagName()=="update")
            m_pConfig->addUpdate(parseUpdate(n));

        n = n.nextSibling();
    }

    return true;
}

bool XmlParser::parseMessages()
{
    QDomNodeList list = m_pDocument->elementsByTagName("messages");

    if(list.isEmpty())
        return false;

    QDomNode n = list.at(0).firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement();

        if(e.tagName()=="message")
            m_pConfig->addMessage(parseMessage(n));

        n = n.nextSibling();
    }

    return true;
}

int XmlParser::getStatus()
{
    return m_iStatus;
}

QString XmlParser::getStatusString()
{
    return m_strStatus;
}

