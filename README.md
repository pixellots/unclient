[![logo](http://updatenode.com/images/logo/updatenode_96.png)](http://www.updatenode.com)
# Welcome to UpdateNode's unclient

UpdateNode is a Update and Messaging System for your software.

In order to receive updates and messages with the unclient, you will need to **register** on [www.updatenode.com](http://www.updatenode.com).

## General

unclient is a Qt based cross platform client to execute updates and show messages based on your definition made on UpdateNode. The client communicates with the service, downloads all required data and executes the provided command based on your definition. Messages can be displayed to customers in 3 different ways: as simple HTML, HTML using webkit (JS, HTML5, etc.), or in an external browser. 
Read more about how to use and various ways to customize unclient in our [Wiki](https://bitbucket.org/updatenode/unclient/wiki)

You are allowed to use unclient with UpdateNode service only. We do not guarantee the correct execution of your update defined on UpdateNode. You are responsible to test your update scenarios and to verify its integration into your own software. unclient does not have any automatic build-in mechanism to do self-updates - if you want to update unclient, you need to send an update for your software and include the new unclient there, or you might launch unclient with all needed settings to replace itself. 

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
4. On Windows, you can additionally create an installer based on installer/setup.iss definition using **nmake build_installer** (Requires Inno Setup in PATH)

## Browse the Wiki

Read more about features and all the different ways to call and communicate with unclient in our [Wiki](https://bitbucket.org/updatenode/unclient/wiki)

*Copyright (C) 2014 UpdateNode UG (haftungsbeschränkt)*

