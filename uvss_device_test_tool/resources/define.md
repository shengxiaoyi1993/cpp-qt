
### device_setting.config

- 格式

```
//对于不同的接口可以使用配置文件规定key队列，
//device_setting.config
{
"ip":"192.168.1.168",
"username":"root",
"password":"admin",
}
```

- 数据结构

```cpp
UVSSDeviceSetting
```


### test_config.config

```txt

//test_config.config
//预先定义的参数格式，初始化需要进行验证
//要执行的参数
{

//预先定义的参数列表

"task_settings":[
{
"name":"getip",
"file_input":"",
"file_output":"",
"input_params":[
"deviceip",
"username",
"password",
"expect_return",
"errcode",
"corcode"
],
"output_params":[
"deviceip",
"username",
"password",
"expect_return",
"errcode",
"corcode",

"return",
"deviceip_return"
]
}
],
//将要执行的任务列表
"tasks_to_do":[
{
  "task":"getip"
}
]

}
```

- cpp
```
typedef struct TaskParam{
  QString _name;
  QString _file_input;
  QString _file_output;
  QList<QString> _input_params;
  QList<QString> _output_params;
}TaskParam;



typedef struct TaskConfig{
  QList<TaskParam> _defined_params;
  QList<QString> _tasks_to_do;
}
```


### 输入文件格式

//如果对每项测试都要构造数据结构工作量较大，可以使用QPair<string,string>,若参数类型不符合在具体使用时进行转换
//input
{
"input":
{
"type":"1",              #测试类型

"ip":"192.168.1.168",
"username":"root",
"password":"admin",

"expect_return":,"1001",            #期望的返回值
#期望获取的参数由函数进行检验

"errcode":"错误验证信息获取到IP",         #不符合预期时输出的错误
"corcode":"正常",                     #不符合预期时输出的错误
}

}


### 输出文件格式


//output
{
"output":
{
"type":"1",              #测试类型

"ip":"192.168.1.168",
"username":"root",
"password":"admin",

"expect_return":,"1001",            #期望的返回值

"errcode":"错误验证信息获取到IP",         #不符合预期时输出的错误
"corcode":"正常"     #不符合预期时输出的错误

"return_value":,"1001",               #实际的返回值
"return_deviceip":,"192.168.1.168"             #实际的返回值
"return_result":,"192.168.1.168"             #实际返回值与预期返回值对比的结果

}

}


### 测试结果输出
- 数据结构
```
//包含需要测试任务队列
//成功测试的任务队列
//测试失败的任务，找不到输入文件或者其他
//每一项的测试结果
//  测试运行时间
//  测试结果是否符合预期

typedef struct SingleResule{
  QString _task_to
}


QMap<QString,QList<QMap<QString,QString>> > v_result_ouput;

```

- json结构
```
{
  "Data":[
  {
    "api":"getip",
    "result":[
    {
      "type":	"0",
      "corcode":	"ok",
  		"deviceip":	"192.168.1.168",
  		"errcode":	"Fail",
  		"expect_return":	"1000",
  		"password":	"admin",
  		"return_deviceip":	"laZHlzNtwTdajCPj",
  		"return_result":	"ok",
  		"return_value":	"1000",
  		"username":	"root"      
    }
    ]
  }
  ]
}
```
