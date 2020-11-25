# client_design
## 1 功能
- 用户登陆
- 三种相机配置
- 几种相机的查看
- 电子态势图查看
- NVR历史数据查看

## 2 模型设计
### 2.1 对空相机

```cpp
class CamToAir
{
  Q_OBJECT
private:
  bool _status;
  string _camname;
  string _ip;
  string _username;
  string _password;
  int mode;
public:
  void setMode(int mode);//设置全景手动模式，全景自动模式
  void setTargetInManualMode(Target t);//在全景手动模式下，设置或取消目标
signal:
  void signalOfThermalInfraredDetect(Rect v);//热红外检测到空中目标
};
```

### 2.2 对地相机

```cpp
class CamToAir
{
  Q_OBJECT
private:
  bool _status;
  string _camname;
  string _ip;
  string _username;
  string _password;
public:
  void setMode(int mode);//设置全景手动模式，运动目标跟踪
  void observeDetial(Rect v_rect,int direct);//查看某一区域细节放大或者缩小查看
  void setTargetInManualMode(Target t);//在全景手动模式下，设置或取消目标
  void setAlertArea(std::vector<Point> v_list,int v_seq);//设置警戒区域

signal:
  void signalOfDefailedImage(string v_path);//细节通道拍摄图片
  void signalOfMovingObjecet(std::vector<Point> v_list);//接收到运动目标
  void signalOfDisconnect();//设备断开连接时发出的信号
  void signalOfAlert(int v_area,Rect v_rect);//设置报警区域的报警信号
};
```

### 2.3 直显相机
```cpp
class CamToAir
{
  Q_OBJECT
private:
  bool _status;
  string _camname;
  string _tty;
public:
  void makeMove(Point v_p,int v_offset_x,int v_offset_y); //使显示图片向某个方向移动，调整重合边界时需要使用
};
```

### 2.4 电子态势图

```cpp
class Coverage{
  int _degree;
  int _inside_diameter;
  int _outside_diameter;
};



class TargetTrack{
  std::vector<Point> _targets;
  string _targetid;
  int _limit;
  bool _valid; //不会被自动移除
  bool _ignore;//不重要的，不会显示在界面

};



class ElectronicSituationMap {
private:
  string _imagepath;
  Coverage _coverage;
  TargetTrack _tracks; //每一行都是一种目标的轨迹
  int _limit_tracks;  //轨迹数量上限


public:
  int setCoverage(Coverage v_cover);       //设置观察区域
  int addTargetTrack(TargetTrack v_track); //设置某一观测物的路径到电子态势图
  int addTargetToTrack(Point v_pos,string v_targetid); //更新某一观测物的位置
  int setEmpty();  //清空电子态势图，包括覆盖区域和观测物路径
  int setAllTrackEmpty(); //清空所有观测物轨迹
signal:
  void signalOfTrackChange(TargetTrack v_tracks);
  void signalOfRepaintAll(Coverage v_coverage,TargetTrack v_tracks);
};
```

### 2.5 NVR
```cpp
class NvrDev{
private:
  string _ip;
  string _username;
  string _password;
  bool _status;
public:
  ResultSet search(SearchCondi v_condi); //通过某一条件找到NVR保存的数据集合
  File getFile(FileLoc v_condi);         //通过某一条件找到某一文件
signal:
void signalOfDisconnect();
void signalOfConnect();

}NvrDev;

```

### 2.6 客户端状态
```cpp
class ClientStatus{
private:
  int status;
  string _username;
  string _server_ip;
  string _password;
  std::vector<ResultEntry> _resultentryies;
  std::vector<Deviceid> _devices;


public:
  int logToServer(string v_serverip,string v_username,string v_password); #登陆,并获取初始设备列表
  int initDeviceList(std::vector<Deviceid> v_device); #根据用户具有的设备权限，取并集为当前界面能访问的设备
  Media requestForMediaFile(ResultEntry);
  ResultSet requestForResultSet(Condi v_condi);  #查询历史数据
signal:
  void signalOfStatusChange(int v_status); //客户端登陆状态发生变化
  void signalOfConnectError();              //连接失败
  void signalOfFailToLogin(string v_error); //登陆失败


}ClientStatus;
```


## 3 view设计
- 登陆界面
- 主界面
  - 菜单栏
  - 对空相机显示
  - 对地相机显示
  - 直显相机配置
  - NVR数据查找与显示
  - 电子态势图

### 登陆界面
- logo显示
- ServerIP
- username
- password
- 连接
- 退出

### 标题栏
- logo
- 系统名称
- 切换到对空相机
- 切换到对地相机
- 切换到直显相机配置
- 切换到电子态势图
- 切换到NVR数据查询
- 系统配置按钮
- 退出按钮

### 对空相机显示
- 相机控制栏
  - 相机状态栏：显示4台相机的连接状态
  - 相机切换按钮（未分配相机不能选中，当前显示相机处于激活状态）
  - 相机功能选择
    - 自动跟踪功能 选择框
    - 手动目标跟踪 选择框
      - 目标选择 选中后鼠标指针特化成十字，可以在界面上标记出目标物；取消选中后，清除标记的目标物
- 相机显示界面 ：显示视频流（假定传过来的视频流中已经标记了目表物）


### 对地相机显示
- 相机控制栏
  - 相机状态栏：显示4台相机的连接状态
  - 相机切换按钮（未分配相机不能选中，当前显示相机处于激活状态）
  - 相机功能选择
    - 自动动物检测功能 选择框
    - 手动目标跟踪 （内置于相机显示界面）
    - 设置入侵区域设置
    - 启动监控设置的入侵区域
    - 巡航扫描监控路径设置
    - 巡航扫描监控功能启动

- 设置入侵区域设置
  - 添加 通过点击表的最后一项进行天件
  - 修改 右键区域条目或者右键某一点进行修改
  - 删除 右键区域条目或者右键某一点进行删除

- 相机显示界面
  - 主图显示
    - 显示相机的视频流（假设相机视频流中已经对检出的目标物进行标记）
    - 主界面画框时，左向画框和右向画框，分别在局部图片显示区显示不同的图片（缩小局部图，放大局部图）
  - 局部图片显示：同时横向显示三张
    - 显示相机细节通道抓牌的图片

### 直显相机显示
- 示意图区域
  - 一张示意图片，包含几个可以选中的点。
  - 点移动控制栏。控制点向外侧移动，会使图片显示区域扩大，向内移动则会使显示区域缩小
    - 上/下/左/右 移动按钮
- 相机切换界面
  - 两排12*按钮（1～12分别是可见光和红外光），按钮颜色标识相机的连接状态

### 电子态势图
- 控制栏
  - 初始化栏
    - 设置俯视图（要求输入图片的大小，方向和比例尺）
    - 生成电子态势图
  - 实时控制  #均是实时生效
    - 清空按钮：清空所有目标轨迹
    - 设置监控数量上限，若目标超出上限则会被自动移除
    - 切换专注模式/全显示
  - 轨迹表：列举所有目标，不能超出数量上限
    - 目标项
      - ignore    #重要程度，ignore的目标在专注模式不会显示
      - focus     #该属性的条目会在电子态势图上突出显示
      - valid     #该属性的条目不会被主动移除
      - 删除       #点击删除后，该项被移除，轨迹也被移除

- 显示界面
  - 俯视图/监控范围/检出目标轨迹各图层叠合的结果


### NVR界面
- 控制栏
  - NVR状态：连接/未连接/报警信息
  - 搜索栏
    - 设备名选项 ：列出每个设备及全部选项（需要在用户权限内；且在NVR上配置过的）
    - 时间段选项：自选
    - 搜索按钮
  - 搜索结果栏
    - 每一项表示一个录制的视频：点击后显示在显示界面上
      - 显示简要信息：设备名/录制时间/
- 显示界面  
  - 显示单个图片或视频：点击项目栏后显示
