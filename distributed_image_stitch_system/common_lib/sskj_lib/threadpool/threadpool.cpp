#include "threadpool.h"
#include <sstream>
#include <assert.h>

ThreadPool::ThreadPool()
{

}



static std::string
get_tid(){
  std::stringstream tmp;
  tmp << std::this_thread::get_id();
  return tmp.str();
}

ThreadPool::ThreadPool(int init_size)
  :m_init_threads_size(init_size),
  m_mutex(),
  m_cond(),
  m_is_started(false)
{
  start();
}

ThreadPool::~ThreadPool()
{
  if(m_is_started)
  {
    stop();
  }
}

void ThreadPool::start()
{
  assert(m_threads.empty());
  m_is_started = true;
  m_threads.reserve(m_init_threads_size);
  for (int i = 0; i < m_init_threads_size; ++i)
  {
    m_threads.push_back(new std::thread(std::bind(&ThreadPool::thread_loop, this)));
  }

}

void ThreadPool::stop()
{
//  __SOLA_LOG(debug, "ThreadPool::stop() stop.");
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_is_started = false;
    m_cond.notify_all();
//    __SOLA_LOG(debug, "ThreadPool::stop() notifyAll().");
  }

  for (threads_t::iterator it = m_threads.begin(); it != m_threads.end() ; ++it)
  {
    (*it)->join();
    delete *it;
  }
  m_threads.clear();
}


void ThreadPool::thread_loop()
{

//  __SOLA_LOG(debug, "ThreadPool::threadLoop() tid : " + get_tid() + " start.");
  while(m_is_started)
  {
    task_t* task = take();

    task->conclude();

  }
//  __SOLA_LOG(debug, "ThreadPool::threadLoop() tid : " + get_tid() + " exit.");
}

void ThreadPool::add_task( task_t* task)
{
  std::unique_lock<std::mutex> lock(m_mutex);
  /*while(m_tasks.isFull())
    {//when m_tasks have maxsize
      cond2.notify_one();
    }
  */

//  cout<<"add: group: "<<task.num_group<<" seq: "<<task.num_seq<<endl;
//  cout<<"add: in: "<<task.frame_in.dir_root<<" out: "<<task.frame_out.dir_root<<endl;

  m_tasks.push_back(task);

//  cout<<"m_tasks.size():"<<m_tasks.size()<<endl;
//  cout<<"inlist: group: "<<m_tasks[0].num_group<<" seq: "<<m_tasks[0].num_seq<<endl;
//  cout<<"inlist: in: "<<m_tasks[0].frame_in.dir_root<<" out: "<<m_tasks[0].frame_out.dir_root<<endl;

  m_cond.notify_one();
}

ThreadPool::task_t* ThreadPool::take()
{
  std::unique_lock<std::mutex> lock(m_mutex);
  //always use a while-loop, due to spurious wakeup
  while(m_tasks.empty() && m_is_started)
  {
//    __SOLA_LOG(debug, "ThreadPool::take() tid : " + get_tid() + " wait.");
    m_cond.wait(lock);
  }

//  __SOLA_LOG(debug, "ThreadPool::take() tid : " + get_tid() + " wakeup.");

  task_t *task;
  tasks_t::size_type size = m_tasks.size();
  if(!m_tasks.empty() && m_is_started)
  {
    task = m_tasks.front();
    m_tasks.pop_front();
    assert(size - 1 == m_tasks.size());
    /*if (TaskQueueSize_ > 0)
    {
      cond2.notify_one();
    }*/
  }

  return task;

}



