# 数据结构定义

- 凡是与数据传输与文件存取相关的数据结构都继承自一个基础的具有json处理结口的基类,继承自该类的类都可以方便地与JSON字符串进行转换
- 该文档中数据结构默认继承自该基类并实现接口

```
class JsonEle{
JsonEle(cJson* v_root);
~JsonEle();
virtual string getString()=0;
virtual string jsonObj()=0;
virtual string jsonObj()=0;

}
```

- 请求列表

```

```



- 客户端状态

```cpp
class ClientStatus_JE {
    int __count_total_thread;
    int __count_working_thread;
    int __count_total_task;
}

// 服务器根据该数据对任务进行分配
```


- 请求执行结果


```cpp
class  ReqReturn_JE{
  int __status_code;
}
```


- 任务的数据结构

```cpp
class Task_JE{
string __id_server;
string __id_client;
string __id_project;
int  __group;
int __block;
}
  
```

- 资源数据结构

```cpp
class Resources_JE{
Task_JE __task;
int __type_resources; //文本文件或者图片文件
string __name;//资源名
int __type_data;//路径，是属于源数据还是结果数据
}
//客户端或者服务器的资源查找规定
- 两者会设置原始数据路径和拼接结果路径
- 两种类型的数据会分别到目录下去寻找，根据__task找到对应的子文件夹，然后根据__name找到对应的资源

``` 



- 客户端分配的任务
```cpp
class TaskList_JE{
  vector<Task_JE>  __l_task;
}；
```

