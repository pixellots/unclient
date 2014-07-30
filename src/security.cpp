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
#include "security.h"

#include <QFile>
#include <QCryptographicHash>

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>

using namespace UpdateNode;

/*!
\class UpdateNode::Security
\brief Main class used for verifying checksum and signature. Implementation inspired by http://delta.affinix.com/qca/.
*/

/*!
Static method to validate SHA-1 checksum for a given QByteArray and aHashReference
*/
bool Security::validateChecksumData(const QByteArray& aData, const QString& aHashReference)
{
    if(aHashReference.toLower() == Security::generateChecksumData(aData).toLower())
        return true;
    else
       return false;
}

/*!
Static method to validate SHA-1 checksum for a given aFileName and aHashReference
*/
bool Security::validateChecksum(const QString& aFileName, const QString& aHashReference)
{
    if(aHashReference.toLower() == Security::generateChecksum(aFileName).toLower())
        return true;
    else
       return false;
}

/*!
Static method to validate checksum for a given aFileName, aHashReference and selected hashing algorithm
\note Following algorithms are supported: MD5, SHA-1, SHA-224, SHA-256, SHA-384 and SHA-512
*/
bool Security::validateChecksum(const QString& aFileName, const QString& aHashReference, const QString aAlgorithm)
{
    if(aHashReference.toLower() == Security::generateChecksum(aFileName.toLower(), aAlgorithm.toLower()))
        return true;
    else
       return false;
}

/*!
Static method to generate a checksum for a given aFileName and selected hashing algorithm
\note Following algorithms are supported: MD5, SHA-1, SHA-224, SHA-256, SHA-384 and SHA-512
*/
QString Security::generateChecksum(const QString& aFileName, const QString& aAlorithm)
{
    EVP_MD_CTX ctx;
    const EVP_MD* alg;

    QFile file(aFileName);

    if(!file.open(QIODevice::ReadOnly))
        return QString();

    if(aAlorithm == "sha1")
        alg = EVP_sha1();
    else if (aAlorithm == "md5")
        alg = EVP_md5();
    else if (aAlorithm == "sha224")
        alg = EVP_sha224();
    else if (aAlorithm == "sha256")
        alg = EVP_sha256();
    else if (aAlorithm == "sha384")
        alg = EVP_sha384();
    else if (aAlorithm == "sha512")
        alg = EVP_sha512();
    else
        alg = EVP_sha1();

    EVP_DigestInit(&ctx, alg);

    const int block_size = 10*1024;
    char buffer[block_size];
    int bytes_read;

    while( (bytes_read = file.read(buffer, block_size))>0)
        EVP_DigestUpdate(&ctx, buffer, bytes_read);

    file.close();

    QByteArray block(EVP_MD_size(alg), 0);
    EVP_DigestFinal( &ctx, (unsigned char*)block.data(), 0 );

    EVP_MD_CTX_cleanup(&ctx);

    return QString(block.toHex());
}

/*!
Static method to generate SHA-1 checksum for a given aFileName
*/
QString Security::generateChecksum(const QString& aFileName)
{
    QFile file(aFileName);

    if(!file.open(QIODevice::ReadOnly))
        return QString();

    const int block_size = 10*1024;
    char buffer[block_size];
    int bytes_read;

    QCryptographicHash::Algorithm hash_alg = QCryptographicHash::Sha1;
    QCryptographicHash hash(hash_alg);

    while( (bytes_read = file.read(buffer, block_size))>0)
        hash.addData(buffer, bytes_read);

    file.close();

    return hash.result().toHex();
}

/*!
Static method to generate SHA-1 checksum for a given QByteArray
*/
QString Security::generateChecksumData(const QByteArray& aData)
{
    QCryptographicHash::Algorithm hash_alg = QCryptographicHash::Sha1;
    QCryptographicHash hash(hash_alg);

    hash.addData(aData);
    return hash.result().toHex();
}


/*!
Static method to verify SHA-512 signature for a given QByteArray, a hex encoded signature and a public key file
\note
*/
bool Security::verfiySignature(const QByteArray& aData, const QByteArray& aSignature, const QString& aPublicKeyFile /* = QString()*/)
{
    EVP_MD_CTX mdctx;
    const EVP_MD* alg;

    QByteArray sig = QByteArray::fromHex(aSignature);

    QString fileName;

    if(aPublicKeyFile.isEmpty())
        fileName = ":/signature/public.key";
    else
        fileName = aPublicKeyFile;

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return false;

    QByteArray publicKey = file.readAll();
    file.close();

    EVP_MD_CTX_init(&mdctx);

    if(aPublicKeyFile.isEmpty())
        alg = EVP_sha1();
    else
        alg = EVP_sha512();

    if(EVP_VerifyInit_ex(&mdctx, alg, NULL) != 1)
    {
        EVP_cleanup();
        return false;
    }

    if(EVP_VerifyUpdate(&mdctx, aData.data(), (unsigned int)aData.size()) != 1)
    {
        EVP_cleanup();
        return false;
    }

    BIO *bio = BIO_new_mem_buf(publicKey.data(), (unsigned int)publicKey.size());
    EVP_PKEY *pkey = NULL;
    pkey = PEM_read_bio_PUBKEY(bio, NULL, NULL, NULL);

    if(EVP_VerifyFinal(&mdctx, (unsigned char *)sig.data(), (unsigned int)sig.size(), pkey) != 1)
    {
        if(pkey)
            EVP_PKEY_free(pkey);

        EVP_cleanup();
        return false;
    }

    if(pkey)
        EVP_PKEY_free(pkey);

    EVP_cleanup();
    return true;
}
