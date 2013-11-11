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

#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>

namespace UpdateNode
{
    class Message
    {
        public:
            Message();

        public:
            void setTitle(const QString& aTitle);
            QString getTitle() const;

            void setMessage(const QString& aMessage);
            QString getMessage() const;

            void setLink(const QString& aLink);
            QString getLink() const;

            void setCode(const QString& aCode);
            QString getCode() const;

        private:
            QString m_strTitle;
            QString m_strMessage;
            QString m_strLink;
            QString m_strCode;
    };
}

#endif // MESSAGE_H
