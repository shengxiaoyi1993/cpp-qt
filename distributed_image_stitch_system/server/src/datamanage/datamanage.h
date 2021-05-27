#ifndef DATAMANAGE_H
#define DATAMANAGE_H

/// 这里维护两个列表 待分配任务列表|已分配任务列表
/// 这里根据设计成支持多个项目同时进行，即每个任务包含项目名和执行端 根据项目名去寻找相应的输入和输出文件夹（在另一区域进行管理）
/// 均支持任务的增减和任务的提取
/// 接口设计按照严格的对象管理




class DataManage
{
public:
  DataManage();
};

#endif // DATAMANAGE_H
