#pragma once

#include "Core.h"

#include <vector>

class ByteArray
{
public:
    /// <summary>
    /// 默认构造函数
    /// </summary>
    constexpr ByteArray() = default;
    /// <summary>
    /// 析构函数
    /// </summary>
    constexpr ~ByteArray() = default;
    /// <summary>
    /// 拷贝构造函数
    /// </summary>
    /// <param name="other">要拷贝的对象</param>
    constexpr ByteArray(const ByteArray& other) = default;
    /// <summary>
    /// 拷贝赋值运算符
    /// </summary>
    /// <param name="other">要拷贝的对象</param>
    /// <returns>this</returns>
    constexpr ByteArray& operator=(const ByteArray& other) = default;
    /// <summary>
    /// 移动构造函数
    /// </summary>
    /// <param name="other">要移动的对象</param>
    constexpr ByteArray(ByteArray&& other) noexcept = default;
    /// <summary>
    /// 移动赋值运算符
    /// </summary>
    /// <param name="other">要移动的对象</param>
    /// <returns>this</returns>
    constexpr ByteArray& operator=(ByteArray&& other) noexcept = default;
    /// <summary>
    /// 构造函数，创建指定大小的字节数组
    /// </summary>
    /// <param name="size">字节数组大小</param>
    constexpr ByteArray(int64 size)
        : m_data(size)
    {

    }
    /// <summary>
    /// 构造函数，指定字节数组大小并赋初值
    /// </summary>
    /// <param name="size">字节数组大小</param>
    /// <param name="byte">填充值</param>
    constexpr ByteArray(int64 size, byte byte)
        : m_data(size, byte)
    {
        
    }
    /// <summary>
    /// 构造函数，从原始数据构造字节数组
    /// </summary>
    /// <param name="data">原始数据指针</param>
    /// <param name="size">数据大小</param>
    constexpr ByteArray(const byte* data, int64 size)
        : m_data(data, data + size)
    {

    }
public:
    // <summary>
    /// 访问指定位置的字节
    /// </summary>
    /// <param name="index">索引位置</param>
    /// <returns>指定位置的字节</returns>
    constexpr byte At(int64 index) const
    {
        return m_data.at(index);
    }
    /// <summary>
    /// 访问第一个字节
    /// </summary>
    /// <returns>返回第一个字节的引用</returns>
    byte& Front()
    {
        return m_data.front();
    }
    /// <summary>
    /// 访问第一个字节（const版本）
    /// </summary>
    /// <returns>返回第一个字节</returns>
    constexpr byte Front() const
    {
        return m_data.front();
    }
    /// <summary>
    /// 访问最后一个字节
    /// </summary>
    /// <returns>返回最后一个字节的引用</returns>
    constexpr byte& Back()
    {
        return m_data.back();
    }
    /// <summary>
    /// 访问最后一个字节（const版本）
    /// </summary>
    /// <returns>返回最后一个字节</returns>
    constexpr byte Back() const
    {
        return m_data.back();
    }
    /// <summary>
    /// 获取底层容器指针
    /// </summary>
    /// <returns></returns>
    constexpr byte* Data()
    {
        return m_data.data();
    }
    /// <summary>
    /// 获取底层容器指针（const版本）
    /// </summary>
    /// <returns></returns>
    constexpr const byte* Data() const
    {
        return m_data.data();
    }
    /// <summary>
    /// 获取当前字节数
    /// </summary>
    /// <returns>字节数组大小</returns>
    constexpr int64 Size() const
    {
        return m_data.size();
    }
    /// <summary>
    /// 获取字节数组的最大可容纳字节数
    /// </summary>
    /// <returns>最大可容纳字节数</returns>
    constexpr int64 MaxSize() const
    {
        return m_data.max_size();
    }
    /// <summary>
    /// 获取当前分配的存储容量（字节数）
    /// </summary>
    /// <returns>当前容量</returns>
    constexpr int64 Capacity() const
    {
        return m_data.capacity();
    }
    /// <summary>
    /// 获取剩余可用容量
    /// </summary>
    /// <returns>剩余容量</returns>
    constexpr int64 Remain() const
    {
        return Capacity() - Size();
    }
    /// <summary>
    /// 清空字节数组
    /// 设置大小为0，但不释放内存
    /// </summary>
    constexpr void Clear()
    {
        m_data.clear();
    }
    /// <summary>
    /// 重置字节数组
    /// 清空内容并释放未使用的内存
    /// </summary>
    constexpr void Reset()
    {
        m_data.clear();
        m_data.shrink_to_fit();
    }
    /// <summary>
    /// 预留存储空间
    /// </summary>
    /// <param name="size">要预留的字节数</param>
    constexpr void Reserve(int64 size)
    {
        m_data.reserve(size);
    }
    /// <summary>
    /// 调整字节数组大小
    /// </summary>
    /// <param name="size">新的字节数组大小</param>
    constexpr void Resize(int64 size)
    {
        m_data.resize(size);
    }
    /// <summary>
    /// 释放未使用的内存
    /// </summary>
    constexpr void Shrink()
    {
        m_data.shrink_to_fit();
    }
    /// <summary>
    /// 在末尾添加一个字节
    /// </summary>
    /// <param name="value">要添加的字节</param>
    constexpr void Add(byte value)
    {
        m_data.push_back(value);
    }
    /// <summary>
    /// 替换指定范围内的字节
    /// </summary>
    /// <param name="pos">起始位置</param>
    /// <param name="length">替换长度</param>
    /// <param name="other">源数据</param>
    constexpr void Replace(int64 pos, int64 length, const ByteArray& other)
    {
        if (pos < 0 || pos > Size())
            return;

        int64 endPos = std::min(pos + length, Size());
        int64 replaceSize = endPos - pos;
        int64 copySize = std::min(replaceSize, static_cast<int64>(other.Size()));

        // 复制新数据
        std::memcpy(m_data.data() + pos, other.Data(), copySize);
    }
    /// <summary>
    /// 用指定字节填充整个数组
    /// </summary>
    /// <param name="byte">填充值</param>
    constexpr void Fill(byte byte)
    {
        std::fill(m_data.begin(), m_data.end(), byte);
    }
    /// <summary>
    /// 在末尾追加另一个字节数组
    /// </summary>
    /// <param name="other">要追加的字节数组</param>
    /// <returns>this</returns>
    constexpr ByteArray& Append(const ByteArray& other)
    {
        m_data.insert(m_data.end(), other.m_data.begin(), other.m_data.end());
        return *this;
    }
    /// <summary>
    /// 在末尾追加另一个字节数组（移动语义）
    /// </summary>
    /// <param name="other">要追加的字节数组</param>
    /// <returns>this</returns>
    constexpr ByteArray& Append(ByteArray&& other)
    {
        m_data.insert(m_data.end(),
            std::make_move_iterator(other.m_data.begin()),
            std::make_move_iterator(other.m_data.end()));
        return *this;
    }
    /// <summary>
    /// 在起始追加另一个字节数组
    /// </summary>
    /// <param name="other">要追加的字节数组</param>
    /// <returns>this</returns>
    constexpr ByteArray& Prepend(const ByteArray& other)
    {
        m_data.insert(m_data.begin(), other.m_data.begin(), other.m_data.end());
        return *this;
    }
    /// <summary>
    /// 在起始追加另一个字节数组（移动语义）
    /// </summary>
    /// <param name="other">要追加的字节数组</param>
    /// <returns>this</returns>
    constexpr ByteArray& Prepend(ByteArray&& other) noexcept
    {
        m_data.insert(m_data.begin(),
            std::make_move_iterator(other.m_data.begin()),
            std::make_move_iterator(other.m_data.end()));
        return *this;
    }
    /// <summary>
    /// 在指定位置插入单个字节
    /// </summary>
    /// <param name="index">插入位置</param>
    /// <param name="value">要插入的字节</param>
    constexpr void Insert(int64 index, byte value)
    {
        if (!IsValidIndex(index))
            return;
        m_data.insert(m_data.begin() + index, value);
    }
    /// <summary>
    /// 在指定位置插入另一个字节数组
    /// </summary>
    /// <param name="index">插入位置</param>
    /// <param name="other">要插入的字节数组</param>
    constexpr void Insert(int64 index, const ByteArray& other)
    {
        if (!IsValidIndex(index))
            return;
        m_data.insert(m_data.begin() + index, other.m_data.begin(), other.m_data.end());
    }
    /// <summary>
   /// 在指定位置插入原始数据
   /// </summary>
   /// <param name="index">插入位置</param>
   /// <param name="data">原始数据指针</param>
   /// <param name="size">数据大小</param>
    constexpr void Insert(int64 index, const byte* data, int64 size)
    {
        if (!IsValidIndex(index) || !data || size <= 0)
            return;
        m_data.insert(m_data.begin() + index, data, data + size);
    }
    /// <summary>
    /// 在指定位置插入多个相同字节
    /// </summary>
    /// <param name="index">插入位置</param>
    /// <param name="count">插入数量</param>
    /// <param name="value">要插入的字节值</param>
    constexpr void Insert(int64 index, int64 count, byte value)
    {
        if (!IsValidIndex(index) || count <= 0)
            return;
        m_data.insert(m_data.begin() + index, count, value);
    }
    /// <summary>
    /// 移除指定范围内的字节
    /// </summary>
    /// <param name="pos">起始位置</param>
    /// <param name="length">要移除的长度</param>
    /// <returns>this</returns>
    constexpr ByteArray& Remove(int64 pos, int64 length)
    {
        if (pos < 0 || pos >= Size() || length <= 0)
            return *this;

        int64 endPos = std::min(pos + length, Size());
        m_data.erase(m_data.begin() + pos, m_data.begin() + endPos);
        return *this;
    }
    /// <summary>
    /// 检查是否包含指定字节
    /// </summary>
    /// <param name="value">要查找的字节</param>
    /// <returns>存在返回true，否则false</returns>
    constexpr bool Contains(byte value)
    {
        return std::find(m_data.begin(), m_data.end(), value) != m_data.end();
    }
    /// <summary>
    /// 统计指定字节的出现次数
    /// </summary>
    /// <param name="value">要统计的字节</param>
    /// <returns>出现次数</returns>
    constexpr int64 Count(byte value)
    {
        return static_cast<int64>(std::count(m_data.begin(), m_data.end(), value));
    }
    /// <summary>
    /// 查找指定字节第一次出现的位置
    /// </summary>
    /// <param name="value">要查找的字节</param>
    /// <param name="start">起始搜索位置</param>
    /// <returns>索引位置，未找到返回-1</returns>
    constexpr int64 IndexOf(byte value, int64 start = 0)
    {
        if (start < 0 || start >= Size())
            return -1;

        auto it = std::find(m_data.begin() + start, m_data.end(), value);
        if (it != m_data.end())
            return static_cast<int64>(std::distance(m_data.begin(), it));
        return -1;
    }
    /// <summary>
    /// 查找指定字节最后一次出现的位置
    /// </summary>
    /// <param name="value">要查找的字节</param>
    /// <param name="start">起始搜索位置，-1表示从末尾开始</param>
    /// <returns>索引位置，未找到返回-1</returns>
    constexpr int64 LastIndexOf(byte value, int64 start = -1)
    {
        if (IsEmpty())
            return -1;

        if (start < 0 || start >= Size())
            start = Size() - 1;

        for (int64 i = start; i >= 0; --i)
        {
            if (m_data[i] == value)
                return i;
        }
        return -1;
    }
    /// <summary>
    /// 检查字节数组是否为空
    /// </summary>
    /// <returns>空返回true，否则false</returns>
    constexpr bool IsEmpty() const
    {
        return m_data.empty();
    }
    /// <summary>
    /// 检查索引是否有效
    /// </summary>
    /// <param name="index">要检查的索引</param>
    /// <returns>有效返回true，否则false</returns>
    constexpr bool IsValidIndex(int64 index) const
    {
        return index >= 0 && index < Size();
    }
    /// <summary>
    /// 交换两个字节数组的内容
    /// </summary>
    /// <param name="other">要交换的字节数组</param>
    constexpr void Swap(ByteArray& other) 
    {
        m_data.swap(other.m_data);
    }
public:
    /// <summary>
    /// 下标运算符
    /// </summary>
    constexpr byte& operator[](int64 index)
    {
        return m_data[index];
    }
    /// <summary>
    /// 下标运算符（const 版本）
    /// </summary>
    constexpr byte operator[](int64 index) const
    {
        return m_data[index];
    }
    /// <summary>
    /// 加法运算符
    /// </summary>
    constexpr friend ByteArray operator+(const ByteArray& left, const ByteArray& right)
    {
        ByteArray result;
        result.m_data.reserve(left.Size() + right.Size());
        result.m_data = left.m_data;
        result.m_data.insert(result.m_data.end(), right.m_data.begin(), right.m_data.end());
        return result;
    }
    /// <summary>
    /// 加法赋值运算符
    /// </summary>
    constexpr ByteArray& operator+=(const ByteArray& other)
    {
        return Append(other);
    }
    /// <summary>
    /// 相等运算符
    /// </summary>
    constexpr friend bool operator==(const ByteArray& left, const ByteArray& right)
    {
        return left.m_data == right.m_data;
    }
    /// <summary>
    /// 不等运算符
    /// </summary>
    constexpr friend bool operator!=(const ByteArray& left, const ByteArray& right)
    {
        return left.m_data != right.m_data;
    }
private:
    std::vector<byte> m_data;
};