#ifndef PRODUCTVERSION_H
#define PRODUCTVERSION_H

#include <QString>

namespace UpdateNode
{
    class ProductVersion
    {
        public:
            ProductVersion();

        public:
            void setName(const QString& aName);
            QString getName() const;

            void setCode(const QString& aCode);
            QString getCode() const;

            void setVersion(const QString& aVersion);
            QString getVersion() const;

        private:
            QString m_strName;
            QString m_strCode;
            QString m_strVersion;
    };
}

#endif // PRODUCTVERSION_H
