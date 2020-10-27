# server_design

## 1 功能

- 用户/用户权限管理
- 接入相机
- 为用户分配一定数量的相机
- 客户端管理

## 2 模块设计(模型与控制)
### 2.1 用户/用户权限管理
- 核心数据结构

```cpp
enum UserType{UserType_admin,UserType_ordinary};
struct User{
  string _username;
  string _password;
  UserType _type;
  CameraArray _cameras;
}User;
std::vector<User> userlist;
```

- 控制

```shell
- 添加用户
- 删除用户
- 修改用户类型
- 修改用户密码
- 修改相机阵列
- 参数的输出
```

### 2.2 相机配置

- 核心数据结构

```cpp
enum CameraType{CameraType_to_air,CameraType_to_ground,CameraType_direct};

struct Camera{
  string _camname;
  string _ip;
  string _username;
  string _password;
}Camera;

struct DirectCamera{
  string _camname;
  string _tty;
  string _ip;
}Camera;

struct CameraArray{
  std::vector<Camera> air_camera_list;
  std::vector<Camera> ground_camera_list;
  std::vector<DirectCamera> direct_camera_list;
}CameraArray;

```

- 控制

```
- 添加对空相机
- 添加对地相机
- 添加直显相机
- 清空相机矩阵
- 清空对空相机矩阵
- 清空对地相机矩阵
- 清空直显相机矩阵
- 删除某一ip相机
- 输出相机阵列
```



### 2.3 客户端管理
- 核心数据结构

```cpp
struct ClientCon{
  string _ip;
  string _username;
  time_t _time;
}ClientCon;
std::vector<ClientCon> clientlist;
```

- 控制

```
- 添加ClientCon到列表中
- 删除登陆的客户端
```

### 2.4 服务器运行状态
- 核心数数据结构

```cpp
struct ServerStatic{
  time_t _starttime;
  unsign long _lasttime;
  int    _quentity_of_user;
}ServerStatic;
```

- 控制

```cpp
- 设置开启时间
- 设置持续运行时间
- 设置登陆的用户数量
```

### 2.5 NVR配置
- 核心数据结构

```cpp
struct NvrDev{
  string _ip;
  string _username;
  string _password;
}NvrDev ;

```

- 控制

```cpp
- 设置NVR参数

```



## 3 view设计
- 启动界面：由于Server服务与Server配置界面分离，因此Server配置界面连接至服务器服务时需要一个服务器配置界面
- 主界面：显示客户端的登陆状态；切换到其他界面；
- 用户配置界面：增删用户，切换到为单一用户配置已有的设备
- 设备配置界面；显示已添加的设备，切换到添加，某一设备窗口
### 启动界面
- 服务器IP
- 用户名   #admin用户
- 密码
- 是否开机启动
- 启动
- 取消

### 主界面
- 菜单栏
  - 用户管理[切换到用户管理界面]
  - 设备管理[切换到设备管理界面]
  - NVR配置[切换到NVR配置界面]
  - about:显示软件的信息
- 标题栏：显示软件系统名称；logo
- 实时信息界面
  - 显示服务器状态信息：启动时间;运行时间；用户登陆数量
  - log信息：关键信息的变化，如服务器启动，用户配置，设备配置，客户端登陆与退出的时间点
  - 客户端登陆状态：显示所有的用户，并显示登陆状态和配置的各个设备的数量

### 用户管理界面
- 用户列表：但击条目出现用户编辑界面
- 添加新用户按钮：点击后出现用户编辑界面
- 用户编辑界面
  - 用户名栏（该项只有单击添加新用户才会允许编辑）
  - 用户密码
  - 设置设备
    - 采用树状表方式+单选框实现对设备的添加
  - 删除用户按钮
  - 应用按钮
  - 确定按钮
  - 取消按钮
- 关闭按钮

### 设备管理界面
- 设备列表：三种类型的设备分别显示；双击某条目出现设备编辑界面
- 添加新设备按钮：点击后出现设备编辑界面
- 关闭按钮
- 设备编辑窗口(对地/对空相机)
  - 设备名栏：只有添加新用户时才能修改
  - 设备类型选择栏
  - 设备ip
  - 串口标识
  - 设备用户名
  - 设备密码
  - 测试连接按钮：测试是否能连接到设备
  - 删除按钮
  - 应用按钮
  - 确定按钮
  - 取消按钮


### NVR配置界面
- nvr ip输入框
- nvr username输入框
- nvr password输入框
- 测试连接
- 启动NVR配置程序 # 配置权限需要由admin进行分配，没有权限时不可操作；点击后弹出NVR配置程序，独立的程序  
- 应用按钮
- 确定按钮
- 取消按钮


## 界面过程

### 手动启动过程
- 启动界面，此时会检测服务器核心服务是否启动（设置本机的IP/用户名/密码/是否设置为开机启动模式）
  - 若服务器IP与本地IP一致则将本机作为服务器，
  - 若服务器IP与本地IP不一致，则尝试连接到服务器，连接不上则提醒
- 若连接上服务器则显示主界面，否则提醒出错
- 进行主界面上的操作，操作结果提交至服务器核心服务

### 服务器核心服务开机自启
- 开机自动启动；界面也同时启动，此时服务器核心服务也同时启动
