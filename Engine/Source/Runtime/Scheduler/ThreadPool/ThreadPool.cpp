#include "pch.h"

#include "ThreadPool.h"

#include "Logger/Logger.h"

ThreadPool::~ThreadPool()
{
    m_isStop.store(false);
    m_cv.NotifyAll();
    m_workers.Clear();
}

ThreadPool::ThreadPool(uint32 num)
{
    m_isStop.store(true);
    for (uint32 i = 0; i < num; i++)
    {
        m_workers.EmplaceBack([this]() { this->_ThreadRun(); });
    }
    GLog->Info("ThreadPool Init with {} threads.", num);
}

void ThreadPool::_ThreadRun()
{
    while (m_isStop.load())
    {
        std::function<void()> task;
        {
            MutexLock<Mutex> lock(m_mutex);
            m_cv.Wait(lock, [this]() {return !m_isStop.load() || !m_tasks.empty(); });
            if (m_tasks.empty())
            {
                return;
            }
            task = std::move(m_tasks.front());
            this->m_tasks.pop();
        }
        //执行任务
        task();
    }
}