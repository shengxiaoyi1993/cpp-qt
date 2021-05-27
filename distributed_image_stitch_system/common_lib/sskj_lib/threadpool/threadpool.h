#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <iostream>
#include <thread>
#include <deque>
#include <mutex>
#include <condition_variable>
#include "taskexecutor.h"



#include <vector>
using namespace std;
class ThreadPool
{
public:
    typedef TaskExecutor task_t;
  ThreadPool();


  ThreadPool(int init_size = 3);
  ~ThreadPool();

  void stop();
  void add_task(task_t *);  //thread safe;

private:
  ThreadPool(const ThreadPool&);//禁止复制拷贝.
  const ThreadPool& operator=(const ThreadPool&);

  bool is_started() { return m_is_started; }
  void start();

  void thread_loop();
  task_t *take();


  typedef std::vector<std::thread*> threads_t;
  typedef std::deque<task_t*> tasks_t;

  int m_init_threads_size;

  threads_t m_threads;
  tasks_t m_tasks;

  //线程索
  std::mutex m_mutex;

  std::condition_variable m_cond;
  bool m_is_started;
};

#endif // THREADPOOL_H
