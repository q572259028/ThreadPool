#include <assert.h>
#include "ThreadPool.h"

ThreadPool::ThreadPool():_mutex(),_isStarted(false),_defaultPori(3),_defaultPoolNum(3)
{
}

ThreadPool::~ThreadPool()
{
  if(_isStarted)
  {
    stop();
  }
}

void ThreadPool::start()
{
  assert(_threads.empty());
   _begintime = std::chrono::high_resolution_clock::now();
  _isStarted = true;
  _threads.reservstruct poCmp
    {
        bool operator()(const ThreadPool::TaskPair p1, const ThreadPool::TaskPair p2)
        {
            return p1.first > p2.first;
        }
    };e(_defaultPoolNum);
  for (int i = 0; i < _defaultPoolNum; ++i)
  {
    _threads.push_back(new std::thread(std::bind(&ThreadPool::threadLoop, this)));
  }

}
void ThreadPool::start(int maxth)
{
  assert(_threads.empty());
  _begintime = std::chrono::high_resolution_clock::now();
  _isStarted = true;
  _threads.reserve(maxth);
  for (int i = 0; i < maxth; ++i)
  {
    _threads.push_back(new std::thread(std::bind(&ThreadPool::threadLoop, this)));
  }

}
void ThreadPool::setDefaultPoolNum(int num)
{
    _defaultPoolNum = num;
}
void ThreadPool::setDefaultPori(priority po)
{
    _defaultPori = po;
}
void ThreadPool::stop()
{
  for (auto it:_threads)
  {
    it->join();
    delete it;
  }
  _threads.clear();
  _isStarted = 0;
}
struct poCmp
    {
        bool operator()(const ThreadPool::TaskPair p1, const ThreadPool::TaskPair p2)
        {
            return p1.first > p2.first;
        }
    };

void ThreadPool::threadLoop()
{
  while(_isStarted)
  {
    Task task = take();
    if(task)
    {
      task();
    }
  }
}

void ThreadPool::addTask(const Task& task,priority po)
{
  std::unique_lock<std::mutex> lock(_mutex);
  addTime addtime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - _begintime).count();
  _tasks.push(std::make_pair(std::make_pair(po,addtime), task));
}

void ThreadPool::addTask(const Task& task)
{
  std::unique_lock<std::mutex> lock(_mutex);
  addTime addtime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - _begintime).count();
  _tasks.push(std::make_pair(std::make_pair(_defaultPori,addtime), task));
}

ThreadPool::Task ThreadPool::take()
{
  std::unique_lock<std::mutex> lock(_mutex);
  Task ret;
  Tasks::size_type _size = _tasks.size();
  if(!_tasks.empty() && _isStarted)
  {
    ret = _tasks.top().second;
    _tasks.pop();
    assert(_size - 1 == _tasks.size());
  }

  return ret;

}
