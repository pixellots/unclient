/****************************************************************************
**
** Copyright (C) 2014 UpdateNode UG (haftungsbeschränkt)
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

#include "textbrowser.h"
#include "localfile.h"
#include "logging.h"

#include <QDebug>
#include <QStringList>
#include <QTimer>
#include <QLayout>
#include <QTemporaryFile>

using namespace UpdateNode;

/*!
\class TextBrowser
\brief Special implementation of QTextBrowser to support external images
*/

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

void TextBrowser::loadHtml(const QUrl& address)
{
    QString fileName = UpdateNode::LocalFile::getCachePath() + QFileInfo(address.toString()).fileName();
    if(QFile::exists(fileName))
    {
        QString html;
        QFile file(fileName);
        if(file.open(QIODevice::ReadOnly))
        {
            html = file.readAll();
            file.close();
        }
        setHtml(html);
    }
    else if(!m_oDownloadList.contains(fileName))
    {
        m_oDownloadList.append(fileName);
        m_oDownloader.doDownload(address, fileName);
    }
}

QVariant TextBrowser::loadResource( int type, const QUrl & name )
{
    switch(type)
    {
        case QTextDocument::ImageResource:
        {
            QString fileName = UpdateNode::LocalFile::getCachePath() + QFileInfo(name.toString()).fileName();
            if(QFile::exists(fileName))
                return QPixmap(fileName);
            else if(!m_oDownloadList.contains(fileName))
            {
                m_oDownloadList.append(fileName);
                m_oDownloader.doDownload(name, fileName);
            }
            return QPixmap(":/images/empty.png");
        }
        default:
            return QTextBrowser::loadResource(type, name);
    }
}

void TextBrowser::done(QByteArray array, const QString& fileName)
{
    if(!array.isEmpty())
    {
        QFileInfo info(fileName);
        QStringList supported_images;
        supported_images << "png" << "jpeg" << "jpg" << "gif" << "tiff";

        if(supported_images.indexOf(info.suffix().toLower()) >-1)
        {
            QImage image = QImage::fromData(array);

            if(image.save(fileName))
                setHtml(toHtml());
            else
                UpdateNode::Logging() << "Unable to save file: " << fileName;
            return;
        }
        else
        {
            QFile file(fileName);
            if(file.open(QIODevice::WriteOnly))
            {
                QTextStream stream(&file);
                stream << QString::fromUtf8(array.data(), array.size());
                file.close();
            }
            setHtml(QString::fromUtf8(array.data(), array.size()));
        }
    }
}

