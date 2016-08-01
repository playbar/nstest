OPENSSL 编译过程记录

0 信息

(1) 本工作最初基于源码1070版本。

(2) 使用的Openssl源码包为openssl-1.0.0e.tar.gz。




1 准备工作

(1) 我们使用Openssl 1.0.0e版本。

(2) 先创建一个trunk的分支，其名为ssl，路径为as3/branches/ssl。在调试完成后，我们再将该分支合并到主干之中。

(3) 我们记总项目根为$AS3，分支根为$BRANCH，Openssl源码根为$OPENSSL，那么：

	$AS3=as3
	$BRANCH=$AS3/branches/ssl
	$SSL=$BRANCH/dependency/ssl
	$OPENSSL=$SSL/openssl-1.0.0e

在$SSL/scripts下保存有我们预先编写好的一些辅助脚本。

(4) 在$SSL下解压openssl-1.0.0e.tar.gz。




2 MACOSX上的配置及编译

(1) 进入目录$OPENSSL，运行脚本runall.sh

	$ cd $OPENSSL
	$ ../scripts/runall.sh

runall.sh将在$SSL下生成如下目录：

	src
	include
	include-common
	include-local
	include-darwin

它们分别保存有我们剪裁的.c文件和.h文件。在这些目录中，include-darwin仅用于MACOSX下的编译。

(2) 在XCODE中，将

	src
	include
	include-common
	include-local
	include-darwin

中所有源码文件添加到baiwan项目中。

(3) 将

	include
	include-common
	include-local
	include-darwin

设置到baiwan项目的头文件查找路径之中。

(4) 添加以下文件到baiwan项目：

	$BRANCH/include/base/SSLAPI.h
	$BRANCH/include/base/XCom.h
	$BRANCH/src/base/SSLAPI.cpp
	$BRANCH/src/base/XCom.cpp

(5) 对以下文件进行修改，使用XCom类替换XSocket类：

	$BRANCH/include/dom/XConnect.h
	$BRANCH/src/dom/XConnect.cpp
	$BRANCH/src/dom/XClient.cpp
	$BRANCH/src/dom/XURL.cpp

(6) 编译和调试。




3 WIN32上配置过程:

注：必须在MACOSX配置过程完成后再进行此过程。

(1) 打开一个VS2008命令行窗口，进入目录$OPENSSL，运行脚本runwin32.bat：

	> cd $OPENSSL
	> ..\scripts\runwin32.bat

runwin32.bat将在$SSL下生成如下目录：

	include-win32

并且将有关文件复制到其中。

(2) 在VS2008中，打开CCClient.sln。

(3) 将

	src
	include
	include-common
	include-local
	include-win32

中所有源码文件添加到GGBase项目中。

(3) 将

	include
	include-common
	include-local
	include-win32

设置到GGBase项目的头文件查找路径之中。

(4) 添加以下文件到GGDom项目：

	$BRANCH/include/base/SSLAPI.h
	$BRANCH/include/base/XCom.h
	$BRANCH/src/base/SSLAPI.cpp
	$BRANCH/src/base/XCom.cpp

(5) 编译和调试。


