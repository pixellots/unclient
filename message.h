#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>

namespace Sara
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

        private:
            QString m_strTitle;
            QString m_strMessage;
            QString m_strLink;
    };
}

#endif // MESSAGE_H
