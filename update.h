#ifndef UPDATE_H
#define UPDATE_H

#include <QString>
#include "productversion.h"

namespace Sara
{
    class Update
    {
        public:
            Update();

            enum Type { INSTALLER = 1, EXECUTABLE = 2, NO_EXECUTABLE = 3, REGISTRY = 4, JAR = 5, UNDEFINED = 0 };

        public:
            void setTitle(const QString& aTitle);
            QString getTitle() const;

            void setDescription(const QString& aDescription);
            QString getDescription() const;

            void setDownloadLink(const QString& aDownloadLink);
            QString getDownloadLink() const;

            void setCommandLine(const QString& aCommandLine);
            QString getCommandLine() const;

            void setFileSize(const QString& aFileSize);
            QString getFileSize() const;

            void setType(int aType);
            int getType();
            Type getTypeEnum();

            void setTargetVersion(const ProductVersion& aTarget);
            ProductVersion getTargetVersion() const;

        private:
            QString m_strTitle;
            QString m_strDescription;
            QString m_strDownloadLink;
            QString m_strCommandLine;

            QString m_strFileSize;
            int m_iType;

            ProductVersion m_oTarget;
    };
}

#endif // UPDATE_H
