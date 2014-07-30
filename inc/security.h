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

#ifndef SECURITY_H
#define SECURITY_H

#include <QString>

namespace UpdateNode
{
    class Security
    {
        public:
            static bool validateChecksumData(const QByteArray& aData, const QString& aHashReference);
            static bool validateChecksum(const QString& aFileName, const QString& aHashReference);
            static bool validateChecksum(const QString& aFileName, const QString& aHashReference, const QString aAlgorithm);
            static QString generateChecksumData(const QByteArray& aData);
            static QString generateChecksum(const QString& aFileName);
            static QString generateChecksum(const QString& aFileName, const QString& aAlorithm);
            static bool verfiySignature(const QByteArray& aData, const QByteArray& aSignature, const QString& aPublicKeyFile = QString());
    };

}
#endif // SECURITY_H
