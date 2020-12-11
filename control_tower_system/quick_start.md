# quick start

- 本文档用于介绍该项目的是进度及快速上手该项目
- 项目依赖
  - 依赖于公司的服务器： com_client com_server模块保存在ftp://192.168.1.238/lib/
  - 依赖于互联网：linuxdeployqt-6-x86_64.AppImage依赖于github上的资源
  - 依赖于公司编译环境：windeployqt 是windows下公司编译打包环境中的一个小工具
- 运行环境
  - windows 10：
  - centos 7  ：能运行但是NVR界面失效，因为大华NVR没有centos下的管理平台


## 项目进度
- 本项目共三个部分组成：Server-service/Server/Client，下面简述大概情况,详见doc/的文档
- Server-service
  - 现只实现单线程的管理；
  - API的功能没有完全覆盖所有需求；
  - 没有接入正规的数据库，不存储录制的文件数据
- Server
  - 已实现大部分功能：用户的管理、设备的管理、NVR的启动，Server-service的交互
  - 接入的NVR界面是大华的NVR管理界面
  - 没有实现管理Server-service自动启动或关闭自动启动的功能
- Client
  - 暂时只实现了直显相机的控制功能与NVR的控制界面启动功能
  - 对于对空相机、电子态势图与对地相机只有演示界面
  - 以后可能需要自行实现NVR存储数据的搜索和操作功能



## 环境搭建
- 按照`doc/system_setup.md`建立环境
- 按照`doc/system_deploy.md`文档进行项目的编译和打包，运行程序就在`scripts`文件夹下
