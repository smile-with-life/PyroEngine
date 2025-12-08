#pragma once
#include "Core.h"

#include "Container/Array.h"

class ByteArray
{
public:
    ByteArray() = default;

    ~ByteArray() = default;

    ByteArray(const char* data);

    ByteArray(const ByteArray& other);

    ByteArray& operator=(const ByteArray& other);

    ByteArray(ByteArray&& other) noexcept;

    ByteArray& operator=(ByteArray&& other) noexcept;
public:
    // 访问指定元素,不进行边界检查
    char& operator[](int64 index);

    // 访问指定元素,不进行边界检查
    const char& operator[](int64 index) const;

    // 向字节数组中写入数据
    void Add(const char* data, int64 size);

    ByteArray& operator<<(char ch);

    char* Data();

    const char* Data() const;

    // 返回数组的元素个数
    int64 Size() const;

    // 返回数组的最大元素个数
    int64 Max() const;

    // 判断数组是否为空
    bool IsEmpty();

    // 改变存储元素的个数
    void Resize(int64 size);

    // 清空数组,实际内存并不释放
    void Clear();

    // 交换内容
    void Swap(ByteArray& other);
private:
    Array<char> m_data;
};