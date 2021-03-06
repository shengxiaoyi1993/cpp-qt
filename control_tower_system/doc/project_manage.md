# project manage

- 需求分析：分析用户需求和已有的设备的行为
- 概要设计：对软件系统进行逻辑设计，界面设计，模块设计过程设计
- 设计确认：设计体现功能的界面，交付给甲方，由甲方确认我方的理解没有问题
- 详细设计：对各个模块进行分析，支持的功能和需要的技术支撑，对不确定的技术进行确认
- 编码：编码各个模块，每个模块完成后完成一定的测试；组合模块，建立模块间交互
- 完成软件系统的编码和打包，编写说明书，发布初版

## 1 需求分析
## 2 概要设计
## 3 设计确认
- [ ] 设计界面
- [ ] 交付确认
- [ ] 确认修改
## 4 详细设计
- [ ] API设计
### 4.1 server-service
- [ ] server-service 设计
### 4.2 server
### 4.3 client

## 5 编码
## 6 打包
## 7 测试


## 进度记录

### 20201203

- 当前状况
  - 大致完项目的基础功能
  - 没有进行系统的测试
  - 稳定性不高，具有不确定性
  - 功能具有可提升的空间
  - 没有完善的程序调试机制
  - 对项目总结，抽象出可重复利用的部分


- 当前的主要任务
  - 对系统进行测试，以避免在使用时造成崩溃/死机等错误，顺利完成演示工作
  - 增加对系统的调试机制，在错误发生时能够快速定位出错点
  - 解决bug
  - 对基础模块进行单元测试 对几个数据模型进行测试输入和输出

- 任务执行顺序  
  - bug出现就应该第一时间解决
  - 先进行若干次实际场景的操作，确保在任何时候都有一个可用的版本
  - 设计与实现的顺序；增加log与单元测试的顺序





```
- 走几次流程，是否能够正常操作（可能先设计整个系统测试方案）

```

- 调试方案

```
- 将抛出错误的地方，替换为弹出警告弹窗
- 对于关键对象的关键操作，输出log文件
- 关闭基础模块的log输出，避免对调试的干扰
```

- bug列表

```
- [x] win下初次启动时，没有配置文件时会报错
- [x] 对于信号和槽传输自定义数据结构的必要设置，是否都需要注册数据结构及在何处注册数据结构
```

- 任务列表

```
- [x] 修改初次启动程序崩溃的问题
  - 函数没有正确返回值，debug模式下编译没有问题，但是release下会导致崩溃，而且调试器不会找到该问题
- [x] 对系统测试过程进行确认，模拟实际流程对设备进行配置和操作
- [x] 确认信号和槽传输自定义数据结构的问题
  - 同一线程内不需要注册（connect的直接连接模式）
- [x] 一段时间后ws连接断开   
  - 完成：server端需要在20min发送心跳检测
```
