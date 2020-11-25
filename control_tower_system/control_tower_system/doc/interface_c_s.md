# API between Client & Server & Server_service

##  1 http
//client
- 用户登录
  - 服务器登录
  - 客户端登录
- 对直显相机的操作（某个点+该点的位移）

- 请求某一用户权限下的设备及设备参数
  - 对空
  - 对地
  - 直显
  - NVR

//server
- log信息
- 运行起始时间
- 当前时间
- 运行时长
- 所有用户/密码及权限下的设备
- 用户的登录状态
- 所有设备/设备类型/用户名密码等
- 添加设备
- 添加用户
- 关闭服务
- 修改用户的信息
- 修改设备相关信息

//from server-service
- 主动报警信息
  - 服务器配置修改，要求重启

### 1.1 连接过程

```
- server-service 监听端口
- server/client 向server:port发送请求，建立连接
- server-service处理请求返回消息
- client接收response，关闭连接
```

### 1.2 请求
#### 1.2.1 请求格式

- requset明文

```
http://192.168.1.238:64336?
{
  "request_type":m,
  "data":{
    "src":"client",  #client / server
    "key1":"value1"
    ...
  }
}
```
- response 明文

```
{
  "return":0,
  "data":{
  }
}
```

- 错误列表

```
//参照 HTTP状态码
//https://www.runoob.com/http/http-status-codes.html

```

- 加密方式
  - 由调用的通讯库决定是否加密
  - 对字符串进行加密/解密处理

### 1.2.2 请求类型
#### 用户登录
- 分为客户端登录与服务端登录验证;当客户端和服务端可以同时以同一账户登录
- request
  - 直接尝试建立ws连接
```
//request_type

//request_data
struct AuthPara{
  string _username;
  string _password;
}AuthPara;


{
  "request_type":m,
  "data":{
    "src":"client",  #client / server
    "user":"admin",
    "password":"admin123"
  }
}
```
- response

```
//return_value
int v_return;
//response_data

{
  "data":{
    "return":0
  }
}
```

#### 对直显相机的操作（某个点+该点的位移）
- 由客户端发起请求，操作成功后返回reponse;server-service通过串口进行操作
- request

```
//request_type

//request_data
struct Point{
  int _x;
  int _y;
}Point;
struct Pad{
  int _x;
  int _y;
}Pad;
struct PointAndPad{
  Point _point;
  Pad _pad;
};

```
- response

```

//return_value
int v_return;
//response_data
```

#### 请求某一用户权限下的设备及设备控制
- 由客户端/服务器发起请求，
- request

```
//request_type

//request_data
NONE
```
- response

```

//return_value
int v_return;
//response_data
as server_design.md
User;
```

//server
#### 运行时间记录
- request

```
//request_type

//request_data
NONE
```
- response

```

//return_value
int v_return;
//response_data
struct TimeRecord{
  unsigned _time_start;
  unsigned _time_cur;
  unsigned _time_interval;
}TimeRecord;

```

#### 所有用户/密码及权限下的设备
- request

```
//request_type

//request_data
NONE
```
- response

```
//return_value
int v_return;
//response_data
as server_design.md
std::vector<User> userlist;
```

#### 用户的登录状态
- request

```
//request_type

//request_data
NONE
```
- response

```

//return_value
int v_return;
//response_data
struct UserStatus{
  string _username;
  bool   _status;
};
```


#### 所有设备/设备类型/用户名密码等
- request

```
//request_type

//request_data
NONE
```
- response

```

//return_value
int v_return;
//response_data
as server_design.md
CameraArray
```
#### 请求设备的连接状态
- request

```
//request_type

//request_data
vector<string> cams;
```
- response

```

//return_value
int v_return;
//response_data
struct CamStatus{
  string _camname;
  bool _status;
}CamStatus;
vector<CamStatus> camstatuss;
```


#### 添加设备
- request

```
//request_type

//request_data
as server_design.md
Camera
```
- response

```

//return_value
int v_return;
//response_data
NONE
```

#### 添加用户
- 由客户端/服务器发起请求，
- request

```
//request_type

//request_data
as server_design.md
User;
```
- response

```

//return_value
int v_return;
//response_data
NONE
```

#### 关闭服务
- request

```
//request_type

//request_data
NONE
```

- response

```

//return_value
int v_return;
//response_data
NONE
```


#### 修改用户的信息
- 由客户端/服务器发起请求，
- request

```
//request_type

//request_data
as server_design.md
User;
```
- response

```

//return_value
int v_return;
//response_data
NONE
```
#### 修改设备相关信息
- request

```
//request_type

//request_data
as server_design.md
Camera
```
- response

```

//return_value
int v_return;
//response_data
NONE
```
#### 删除设备
- request

```
//request_type

//request_data
string camname;
```
- response

```

//return_value
int v_return;
//response_data
NONE
```
#### 删除用户
- request

```
//request_type

//request_data
string username;
```
- response

```

//return_value
int v_return;
//response_data
NONE
```
## 2 ws
### 2.1 建立websocket连接

```
//url
ws://192.168.1.100/xxx


//return_value
int v_return;
```

### 2.2 通过ws发送消息
- 暂时不对ws发送消息进行应答
#### 2.2.1 要求重启

- request

```
//request_type

//request_data
NONA
```



#### 2.2.2 log信息


- request

```
//request_type

//request_data
string log;
```

- 用户登陆状态发生改变

```
vector<UserStatus> list;
```
