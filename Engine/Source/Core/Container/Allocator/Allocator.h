#pragma once

#include "Core.h"
#include "Memory/Memory.h"

/// <summary>
/// 内存分配器
/// </summary>
class Allocator
{
public:
    /// <summary>
    /// 分配指定数量的内存
    /// <para>分配 count * sizeof(Type) 字节的未初始化存储空间</para>
    /// </summary>
    /// <typeparam name="Type">要分配内存的元素类型</typeparam>
    /// <param name="count">需要分配的元素个数</param>
    /// <returns>指向分配内存的指针</returns>
    /// <exception cref="std::bad_alloc">当内存分配失败时抛出</exception>
    template<class Type>
    constexpr Type* Allocate(int64 count)
    {
        // 参数校验
        if (count <= 0) [[unlikely]]
        {
            throw std::invalid_argument("Allocate count must be positive");
        }
        // 计算要分配的字节数
        const int64 size = count * sizeof(Type);
        // 检查整数溢出
        if (size / sizeof(Type) != count) [[unlikely]]
        {
            //throw std::bad_alloc("Integer overflow");
        }
        // 检查是否有指定的内存资源
        if (m_resource)
        {
            // 尝试从内存池分配
            if (void* ptr = m_resource->Malloc(size))
            {
                return static_cast<Type*>(ptr);
            }
        }
        else
        {
            // 回退到 new 分配
            if (void* ptr = ::operator new(size, std::align_val_t(alignof(Type)))) {
                return static_cast<Type*>(ptr);
            }
        }
        // 分配失败
        //throw std::bad_alloc("Insufficient available space");
    }

    /// <summary>
    /// 释放指定数量的内存
    /// <para>用于释放通过对应Allocate函数分配的连续内存块</para>
    /// </summary>
    /// <typeparam name="Type">要释放内存的元素类型</typeparam>
    /// <param name="count">指向要释放的内存块的指针</param>
    /// <returns>无返回值</returns>
    /// <exception cref="std::invalid_argument">当ptr为空指针时可能抛出</exception>
    template<class Type>
    constexpr void Deallocate(Type* ptr, int64 count)
    {
        // 参数校验
        if (!ptr) [[unlikely]] 
        {
            throw std::invalid_argument("Deallocate ptr must not be");
        }
        if (count <= 0) [[unlikely]] 
        {
            throw std::invalid_argument("Deallocate count must be positive");
        }
        // 计算要分配的字节数
        const int64 size = count * sizeof(Type);
        // 检查整数溢出
        if (size / sizeof(Type) != count) [[unlikely]]
        {
            //throw std::bad_alloc("Integer overflow");
        }
        // 检查是否有指定的内存资源
        if (m_resource) {
            /*if (m_resource->Contains(ptr))
            {
                m_resource.Free(ptr);
            }    */ 
        }
        else {
            // 回退到 delete 释放
            ::operator delete(ptr, size, std::align_val_t(alignof(Type)));
        }
    }
public:
    MemoryResource* m_resource = nullptr;
};