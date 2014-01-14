# Welcome to UpdateNode's unclient

In order to receive updates and messages with the unclient, you will need to **register** on [updatenode.com](http://updatenode.com).

## General

## License

**Commercial License Usage**
Licensees holding valid commercial UpdateNode license may use unclient
under the terms of the the Apache License, Version 2.0
Full license description file: LICENSE.COM

**GNU General Public License Usage**
Alternatively, this project may be used under the terms of the GNU
General Public License version 3.0 as published by the Free Software
Foundation. Please review the following information to ensure the
GNU General Public License version 3.0 requirements will be met:
http://www.gnu.org/copyleft/gpl.html.
Full license description file: LICENSE.GPL

## Prerequisites

### Windows

* Install Vistual Studio
* Install [Qt Framework](http://qt-project.org/)
* Install [OpenSSL](http://slproweb.com/products/Win32OpenSSL.html)
* Install [Inno Setup](http://www.jrsoftware.org/isinfo.php) *(Optional - only if you want to build an installer)*

### Mac

* Install XCode
* Install [Qt Framework](http://qt-project.org/)

### Linux (Ubuntu)

* sudo apt-get install g++ openssl qt4-dev-tools 

## How to build

1. **qmake** (In case you want to build without webkit) or **qmake -config webkit** for integrated webkit
2. **make** on Linux/Mac or **nmake release** on Windows
3. If you want to create a fresh version, you might call **(n)make deploy**
4. On Windows, you can additionally create an installer based on installer/setup.iss definition using **name build_installer** (Requires Inno Setup in PATH)

## Browse the Wiki

Read more about features and all the different ways to call and communicate with unclient in our [Wiki](https://bitbucket.org/updatenode/unclient/wiki)