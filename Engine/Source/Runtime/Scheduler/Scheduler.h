#pragma once

#include "Core.h"

#include "Scheduler/ThreadPool/ThreadPool.h"

#include <coroutine>
#include <optional>
#include <exception>
#include <stdexcept>

enum class TaskPriority
{
    Critical,
    Normal,
    Low
};

template<class Type>
class Result
{
public:

public:
    Type Result()
    {
        
    }

    bool IsReady() const
    {
        
    }
};

ThreadPool GThreadPool(4);
ThreadPool GIOThreadPool(4);

struct Executor
{
    using Func = std::function<void()>;
    std::function<void(Func)> Execute;

    static Executor IOThreadPool;
};

Executor Executor::IOThreadPool = { [](auto func) { GIOThreadPool.Submit(std::move(func)); } };



template<class Type>
class [[nodiscard]] Async
{
public:
    struct promise_type
    {
        struct FinalAwaiter
        {
            bool await_ready() const noexcept
            {
                return false;
            }
            template<typename Promise>
            std::coroutine_handle<> await_suspend(std::coroutine_handle<Promise> handle) noexcept
            {
                auto& promise = handle.promise();
                if (promise.Continuation)
                {
                    // 直接返回延续句柄，编译器会负责跳转
                    return promise.Continuation;
                }
                else
                {
                    // 没有延续时返回 std::noop_coroutine()，表示不恢复任何协程
                    return std::noop_coroutine();
                }
            }
            void await_resume() noexcept {}
        };
        // 协程返回给调用者用于操作协程的对象
        Async<Type> get_return_object() noexcept
        {
            return Async<Type>{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        // 初始挂起，等待调度
        std::suspend_always initial_suspend() noexcept
        {
            return std::suspend_always{};
        }
        // 最终挂起，等待查询结果
        FinalAwaiter final_suspend() noexcept
        {
            return FinalAwaiter{};
        }
        // 协程正常结束时的返回值
        void return_value(Type value)
        {
            Result = value;
        }
        // 协程发生未处理异常时的处理
        void unhandled_exception() noexcept
        {
            Exception = std::current_exception();
        }

        std::optional<Type> Result;
        std::exception_ptr Exception;
        std::coroutine_handle<> Continuation;
    };

    struct awaitable_base
    {
        std::coroutine_handle<promise_type> m_coroutine;

        awaitable_base(std::coroutine_handle<promise_type> coroutine) noexcept
            : m_coroutine(coroutine)
        {

        }

        // 协程等待接口
        bool await_ready() const noexcept
        {
            return m_coroutine.done();
        }
        // 协程挂起接口
        // 对称转移：返回 coroutine_handle 而不是 void
        std::coroutine_handle<> await_suspend(std::coroutine_handle<> awaiting_handle) noexcept
        {
            // 保存等待者句柄到被等待协程的 promise 中
            m_coroutine.promise().Continuation = awaiting_handle;

            // 直接返回被等待协程的句柄，编译器会负责跳转
            // 这样就不会增加栈深度
            return m_coroutine;
        }
    };

    auto operator co_await() const& noexcept
    {
        struct TaskAwaiter : awaitable_base
        {
            using awaitable_base::awaitable_base;

            decltype(auto) await_resume()
            {
                if (!this->m_coroutine)
                {
                    throw std::logic_error("broken promise");
                }

                return this->m_coroutine.promise().Result.value();
            }
        };

        return TaskAwaiter{ m_handle };
    }

    auto operator co_await() const&& noexcept
    {
        struct TaskAwaiter : awaitable_base
        {
            using awaitable_base::awaitable_base;

            decltype(auto) await_resume()
            {
                if (!this->m_coroutine)
                {
                    throw std::logic_error("broken promise");
                }

                return std::move(this->m_coroutine.promise()).Result.value();
            }
        };

        return TaskAwaiter{ m_handle };
    }

    operator std::coroutine_handle<>() const noexcept
    {
        return m_handle;
    }
public:
    explicit Async(std::coroutine_handle<promise_type> handle)
        : m_handle(handle)
    {

    }

    ~Async()
    {
        if (m_handle)
        {
            m_handle.destroy();
        }
    }

    Async(const Async&) = delete;

    Async& operator=(const Async&) = delete;

    Async(Async&& other)
        : m_handle(other.m_handle)
    {
        other.m_handle = nullptr;
    }

    Async& operator=(Async&& other)
    {
        if (this != &other)
        {
            if (m_handle)
                m_handle.destroy();
            m_handle = other.m_handle;
            other.m_handle = nullptr;
        }
        return *this;
    }
public:
    void Execute()
    {

    }

    Type Result()
    {
        if (m_handle.promise().Exception)
        {
            std::rethrow_exception(m_handle.promise().Exception);
        }
        return m_handle.promise().Result.value();
    }

    bool IsReady() const
    {
        return m_handle.done();
    }

    auto Handle() const noexcept { return m_handle; }
private:
    std::coroutine_handle<promise_type> m_handle;
};

template<>
class [[nodiscard]] Async<void>
{
public:
    struct promise_type
    {
        struct FinalAwaiter
        {
            bool await_ready() const noexcept
            {
                return false;
            }
            template<typename Promise>
            std::coroutine_handle<> await_suspend(std::coroutine_handle<Promise> handle) noexcept
            {
                auto& promise = handle.promise();
                if (promise.Continuation)
                {
                    // 直接返回延续句柄，编译器会负责跳转
                    return promise.Continuation;
                }
                else
                {
                    // 没有延续时返回 std::noop_coroutine()，表示不恢复任何协程
                    return std::noop_coroutine();
                }
            }
            void await_resume() noexcept {}
        };

        // 协程返回给调用者用于操作协程的对象
        Async<void> get_return_object() noexcept
        {
            return Async<void>{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }

        // 初始挂起，等待调度
        std::suspend_always initial_suspend() noexcept
        {
            return std::suspend_always{};
        }

        // 最终挂起，等待查询结果
        FinalAwaiter final_suspend() noexcept
        {
            return FinalAwaiter{};
        }

        // 协程正常结束时的返回值（void版本没有返回值）
        void return_void() noexcept
        {
            // void协程不需要返回值
        }

        // 协程发生未处理异常时的处理
        void unhandled_exception() noexcept
        {
            Exception = std::current_exception();
        }

        std::exception_ptr Exception;
        std::coroutine_handle<> Continuation;
    };

    struct awaitable_base
    {
        std::coroutine_handle<promise_type> m_coroutine;

        awaitable_base(std::coroutine_handle<promise_type> coroutine) noexcept
            : m_coroutine(coroutine)
        {
        }

        // 协程等待接口
        bool await_ready() const noexcept
        {
            return !m_coroutine || m_coroutine.done();
        }

        // 协程挂起接口
        // 对称转移：返回 coroutine_handle 而不是 void
        std::coroutine_handle<> await_suspend(std::coroutine_handle<> awaiting_handle) noexcept
        {
            // 保存等待者句柄到被等待协程的 promise 中
            m_coroutine.promise().Continuation = awaiting_handle;

            // 直接返回被等待协程的句柄，编译器会负责跳转
            return m_coroutine;
        }
    };

    auto operator co_await() const& noexcept
    {
        struct TaskAwaiter : awaitable_base
        {
            using awaitable_base::awaitable_base;

            void await_resume()
            {
                if (!this->m_coroutine)
                {
                    throw std::logic_error("broken promise");
                }

                // 检查是否有异常
                if (this->m_coroutine.promise().Exception)
                {
                    std::rethrow_exception(this->m_coroutine.promise().Exception);
                }
                // void版本不需要返回值
            }
        };

        return TaskAwaiter{ m_handle };
    }

    auto operator co_await() const&& noexcept
    {
        struct TaskAwaiter : awaitable_base
        {
            using awaitable_base::awaitable_base;

            void await_resume()
            {
                if (!this->m_coroutine)
                {
                    throw std::logic_error("broken promise");
                }

                // 检查是否有异常
                if (this->m_coroutine.promise().Exception)
                {
                    std::rethrow_exception(this->m_coroutine.promise().Exception);
                }
                // void版本不需要返回值
            }
        };

        return TaskAwaiter{ m_handle };
    }

    operator std::coroutine_handle<>() const noexcept
    {
        return m_handle;
    }
public:
    explicit Async(std::coroutine_handle<promise_type> handle)
        : m_handle(handle)
    {

    }

    ~Async()
    {
        if (m_handle)
        {
            m_handle.destroy();
        }
    }

    Async(const Async&) = delete;

    Async& operator=(const Async&) = delete;

    Async(Async&& other) noexcept
        : m_handle(other.m_handle)
    {
        other.m_handle = nullptr;
    }

    Async& operator=(Async&& other) noexcept
    {
        if (this != &other)
        {
            if (m_handle)
                m_handle.destroy();
            m_handle = other.m_handle;
            other.m_handle = nullptr;
        }
        return *this;
    }

public:
    bool IsReady() const
    {
        return !m_handle || m_handle.done();
    }

    void Wait() const
    {
        while (!IsReady())
        {
            std::this_thread::yield();
        }
    }

    auto Handle() const noexcept { return m_handle; }

    void RethrowIfException() const
    {
        if (m_handle && m_handle.promise().Exception)
        {
            std::rethrow_exception(m_handle.promise().Exception);
        }
    }
private:
    std::coroutine_handle<promise_type> m_handle;
};

class Scheduler
{
public:
    ~Scheduler() = default;
public:
    void Init(uint32 num);

    void Tick();

    void Exit();

    void Submit();
};