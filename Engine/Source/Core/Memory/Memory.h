#pragma once
#include "Core.h"

/// <summary>
/// 内存资源类
/// </summary>
class MemoryResource
{
public:
    /// <summary>
    /// 析构函数
    /// </summary>
    virtual ~MemoryResource() = default;
    /// <summary>
    /// 禁止拷贝构造
    /// </summary>
    MemoryResource(const MemoryResource& other) = delete;
    /// <summary>
    /// 禁止拷贝赋值
    /// </summary>
    MemoryResource& operator=(const MemoryResource& other) = delete;
    /// <summary>
    /// 移动构造函数
    /// </summary>
    /// <param name="other"></param>
    MemoryResource(MemoryResource&& other) = default;
    /// <summary>
    /// 移动赋值函数
    /// </summary>
    /// <param name="other"></param>
    /// <returns></returns>
    MemoryResource& operator=(MemoryResource&& other) = default;
    /// <summary>
    /// 构造函数，指定内存对齐大小
    /// </summary>
    /// <param name="alignment">内存对齐大小，对齐值必须是2的幂且大于0</param>
    explicit MemoryResource(int64 alignment)
    {

    }
public:
    /// <summary>
    /// 分配指定大小的内存块
    /// </summary>
    /// <param name="bytes">请求分配的内存字节数，必须大于0</param>
    /// <returns>分配失败，返回 nullptr</returns>
    void* Malloc(int64 bytes)
    {
        if (bytes <= 0) return nullptr;

        void* ptr = DoMalloc(bytes);

        return ptr;
    }
    /// <summary>
    /// 释放分配的内存块
    /// </summary>
    /// <param name="ptr">要释放的内存指针</param>
    /// <param name="bytes">分配的内存字节数</param>
    void Free(void* ptr, int64 bytes)
    {
        if (!ptr) return;

        DoFree(ptr, bytes);
    }
    /// <summary>
    /// 获取当前内存资源的对齐大小
    /// </summary>
    /// <returns>内存对齐大小</returns>
    int64 Alignment() const 
    { 
        return m_alignment; 
    }
    /// <summary>
    /// 比较两个内存资源是否相等
    /// </summary>
    /// <param name="other">要比较的内存资源</param>
    bool IsEqual(const MemoryResource& other) const
    {
        return this == &other || DoIsEqual(other);
    }
protected:
    /// <summary>
    /// 实际的内存分配实现，由派生类重写
    /// </summary>
    /// <param name="bytes">请求分配的内存字节数，必须大于0</param>
    /// <returns>分配失败，返回 nullptr</returns>
    virtual void* DoMalloc(int64 bytes) = 0;
    /// <summary>
    /// 实际的内存释放实现，由派生类重写
    /// </summary>
    /// <param name="ptr">要释放的内存指针</param>
    /// <param name="bytes">分配的内存字节数</param>
    virtual void DoFree(void* ptr, int64 bytes) = 0;
    /// <summary>
    /// 实际的内存资源相等比较实现，由派生类重写
    /// </summary>
    /// <param name="other">要比较的内存资源</param>
    virtual bool DoIsEqual(const MemoryResource& other) const = 0;
private:
    /// <summary>
    /// 内存对齐大小
    /// </summary>
    int64 m_alignment = alignof(std::max_align_t);
};
