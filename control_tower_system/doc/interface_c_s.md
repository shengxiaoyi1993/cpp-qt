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
  "data":{
    "return":0
  }
}
```

- 错误列表
```
//参照 HTTP状态码
//https://www.runoob.com/http/http-status-codes.html
- 成功
- 没有权限
- 未知错误
- 网络无法连接
```

- 加密方式
  - 由调用的通讯库决定是否加密
  - 对字符串进行加密/解密处理

### 1.2.2 请求类型
#### 用户登录
- 分为客户端登录与服务端登录验证;当客户端和服务端可以同时以同一账户登录
- request
```
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
{
  "request_type":m,
  "src":"client",  #client / server
  "data":{
    "user":"admin",
    "password":"admin123"
  }
}
```
- response
```
{
  "data":{
    "return":0
  }
}
```

#### 请求某一用户权限下的设备及设备参数
- 由客户端/服务器发起请求，
- request
```
{
  "request_type":m,
  "src":"client",  #client / server
  "data":{
  }
}
```
- response
```
struct 


{
  "data":{

  }
}
```


//server
#### 运行起始时间
#### 当前时间
#### 运行时长
#### 所有用户/密码及权限下的设备
#### 用户的登录状态
#### 所有设备/设备类型/用户名密码等
#### 添加设备
#### 添加用户
#### 关闭服务
#### 修改用户的信息
#### 修改设备相关信息

## 2 ws
- 建立websocket连接
- 通过ws发送消息
  - 要求重启
  - log信息
