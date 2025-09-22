#pragma once
#include "Core.h"

/// <summary>
/// 内存资源
/// </summary>
class MemoryResource
{
public:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="size"></param>
    explicit MemoryResource(int64 alignment);
public:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="bytes"></param>
    /// <param name="alignment"></param>
    /// <returns></returns>
    void* Malloc(int64 bytes);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="ptr"></param>
    /// <param name="bytes"></param>
    /// <param name="alignment"></param>
    void Free(void* ptr, int64 bytes);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="other"></param>
    bool IsEqual(const MemoryResource& other) const;
protected:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="bytes"></param>
    /// <param name="alignment"></param>
    /// <returns></returns>
    virtual void* DoMalloc(int64 bytes) = 0;

    /// <summary>
    /// 
    /// </summary>
    /// <param name="ptr"></param>
    /// <param name="bytes"></param>
    /// <param name="alignment"></param>
    virtual void DoFree(void* ptr, int64 bytes) = 0;

    /// <summary>
    /// 
    /// </summary>
    /// <param name="other"></param>
    virtual bool DoIsEqual(const MemoryResource& other) const = 0;
private:
    // 内存对齐大小
    int64 m_alignment = alignof(std::max_align_t);
};
