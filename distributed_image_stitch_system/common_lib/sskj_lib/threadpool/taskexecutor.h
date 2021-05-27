#ifndef TASKEXECUTOR_H
#define TASKEXECUTOR_H

#include <iostream>
#include <string>

using  namespace std;
/**
 * @brief The TaskExecutor class
 * 该类用于作为任务执行器的父类
 */

class TaskExecutor
{
public:
  virtual int conclude(const string &v_command)=0;
  virtual int conclude()=0;
  virtual   ~TaskExecutor();

protected:
  TaskExecutor();

};



#endif // TASKEXECUTOR_H
