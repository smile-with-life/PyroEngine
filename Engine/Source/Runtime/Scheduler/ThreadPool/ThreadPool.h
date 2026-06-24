#pragma once

#include "Core.h"
#include "Runtime.h"
#include "Container/Array.h"
#include "Thread/Thread.h"
#include "Thread/Mutex.h"
#include "Thread/WaitCondition.h"

class ThreadPool
{
public:
    ~ThreadPool();

    explicit ThreadPool(uint32 num);
public:
    template<class Func, class... Args>
    auto Submit(Func&& func, Args&&... args) -> std::future<decltype(func(args...))>
    {
        using ReturnType = decltype(func(args...));
        if (!m_isStop.load())
            return std::future<ReturnType>{};
        // 包装任务     
        auto shared_task = std::make_shared<std::packaged_task<ReturnType()>>(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
        std::future<ReturnType> result = shared_task->get_future();
        std::packaged_task<void()> task([shared_task]() { (*shared_task)(); });
        {
            MutexLock<Mutex> lock(m_mutex);
            m_tasks.emplace(std::move(task));
        }
        m_cv.NotifyOne();
        return result;
    }
private:
    void _ThreadRun();
private:
    Array<Thread> m_workers;
    std::queue<std::function<void()>> m_tasks;
    Mutex m_mutex;
    WaitCondition m_cv;
    std::atomic<bool> m_isStop;
};