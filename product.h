#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>

namespace Sara
{
    class Product
    {
        public:
            Product();

        public:
            void setName(const QString& aName);
            QString getName() const;

            void setCode(const QString& aCode);
            QString getCode() const;

            void setIconUrl(const QString& aUrl);
            QString getIconUrl() const;

            QString getLocalIcon() const;

        private:
            QString m_strName;
            QString m_strCode;
            QString m_strIconUrl;

    };
}

#endif // PRODUCT_H
