#include <stdlib.h>
#include <QString>
#include <QDebug>
#include <QDomElement>

#include "xmlparser.h"
#include "config.h"

using namespace Sara;

XmlParser::XmlParser(QObject* parent)
    : QObject(parent)
{
    m_pDocument = NULL;
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
        if(!parseStatus() || !parseProduct() || !parseVersion())
            return FALSE;

        bool isThere = parseUpdates();

        if(!isThere)
            return parseMessages();

    }
    else
    {
        qDebug() << "ERROR: " << errorMsg << "(Line " << errorLine << " - Column " << errorColumn << ")";
        return FALSE;
    }
    return TRUE;
}

bool XmlParser::parseStatus()
{
    QDomNodeList list = m_pDocument->elementsByTagName("status");

    if(list.isEmpty())
        return FALSE;

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

    return TRUE;
}

bool XmlParser::parseProduct()
{
    QDomNodeList list = m_pDocument->elementsByTagName("product");

    if(list.isEmpty())
        return FALSE;

    Sara::Product product;

    QDomNode productNode = list.at(0);

    QDomNode n = productNode.firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement();

        if(e.tagName()=="code")
            product.setCode(e.text());
        else if(e.tagName()=="name")
            product.setName(e.text());
        else if(e.tagName()=="iconUrl")
            product.setIconUrl(e.text());

        n = n.nextSibling();
    }

    Sara::Config::Instance()->setProduct(product);

    return TRUE;
}

bool XmlParser::parseVersion()
{
    QDomNodeList list = m_pDocument->elementsByTagName("version");

    if(list.isEmpty())
        return FALSE;

    Sara::ProductVersion version;

    QDomNode versionNode = list.at(0);

    QDomNode n = versionNode.firstChild();
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

    Sara::Config::Instance()->setVersion(version);

    return TRUE;
}

Sara::Update XmlParser::parseUpdate(QDomNode aNode)
{
    Sara::Update update;

    QDomNode n = aNode.firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement();

        if(e.tagName()=="title")
            update.setTitle(e.text());
        else if(e.tagName()=="description")
            update.setDescription(e.text());
        else if(e.tagName()=="type")
            update.setType(e.text().toInt());
        else if(e.tagName()=="file")
            update.setDownloadLink(e.text());
        else if(e.tagName()=="commandline")
            update.setCommandLine(e.text());
        else if(e.tagName()=="file_size")
            update.setFileSize(e.text());

        n = n.nextSibling();
    }

    return update;
}

Sara::Message XmlParser::parseMessage(QDomNode aNode)
{
    Sara::Message message;

    QDomNode n = aNode.firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement();

        if(e.tagName()=="title")
            message.setTitle(e.text());
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
        return FALSE;

    QDomNode n = list.at(0).firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement();

        if(e.tagName()=="update")
            Sara::Config::Instance()->addUpdate(parseUpdate(n));

        n = n.nextSibling();
    }

    return TRUE;
}

bool XmlParser::parseMessages()
{
    QDomNodeList list = m_pDocument->elementsByTagName("messages");

    if(list.isEmpty())
        return FALSE;

    QDomNode n = list.at(0).firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement();

        if(e.tagName()=="message")
            Sara::Config::Instance()->addMessage(parseMessage(n));

        n = n.nextSibling();
    }

    return TRUE;
}

int XmlParser::getStatus()
{
    return m_iStatus;
}

QString XmlParser::getStatusString()
{
    return m_strStatus;
}

