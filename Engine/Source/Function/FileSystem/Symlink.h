#pragma once
#include "Core.h"
#include "String/String.h"

class Symlink
{
public:
    ~Symlink();

    Symlink(const String& path);

    // 复制构造函数被删除
    Symlink(const Symlink& other) = delete;

    // 拷贝赋值运算符被删除
    Symlink& operator=(const Symlink& other) = delete;

    Symlink(Symlink&& other);

    Symlink& operator=(Symlink&& other);
public:
    // 解析链接，获取链接最终指向目标的路径
    String ResolveLink();

    // 获取链接指向路径
    String TargetPath() const;

    // 设置链接指向目标
    bool SetTarget(const String& path);

    bool CreateSymlink(const String& path);
};