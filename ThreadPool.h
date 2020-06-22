#ifndef _THREADPOOL_H
#define _THREADPOOL_H
#include <chrono>
#include <vector>
#include <utility>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>


class ThreadPool
{
private:
    typedef int priority;
    typedef double addTime;
    typedef std::function<void()> Task;
    typedef std::vector<std::thread*> Threads;
    ThreadPool(const ThreadPool&);
    const ThreadPool& operator=(const ThreadPool&);
    void threadLoop();
    Task take();

    priority _defaultPori;
    int _defaultPoolNum;

    std::chrono::time_point<std::chrono::high_resolution_clock> _begintime;
    std::mutex _mutex;
    bool _isStarted;
public:
    typedef std::pair<std::pair<priority,addTime>, Task> TaskPair;
    struct poCmp
    {
        bool operator()(const ThreadPool::TaskPair p1, const ThreadPool::TaskPair p2)
        {
            return p1.first > p2.first;
        }
    };
    typedef std::priority_queue<TaskPair, std::vector<TaskPair>, poCmp> Tasks;



    ThreadPool();
    ~ThreadPool();

    void start();
    void start(int);
    void stop();
    void addTask(const Task&);
    void addTask(const Task&,priority);
    void setDefaultPori(priority);
    void setDefaultPoolNum(int);
private:
    Threads _threads;
    Tasks _tasks;
};

#endif
