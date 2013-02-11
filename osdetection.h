#ifndef OSDETECTION_H
#define OSDETECTION_H

#include <QString>

namespace Sara {

    class OSDetection
    {
        public:
            static QString getArch();
            static QString getOS();

            static QString getWindowsVersion();
            static QString getMacVersion();
            static QString getLinuxVersion();
            static QString getOthersVersion();

    };
}

#endif // OSDETECTION_H
