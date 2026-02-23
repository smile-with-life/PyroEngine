#pragma once

#include "Core.h"
#include "FileSystem/FileEntry.h"

class Symlink : public FileEntry
{
public:
    ~Symlink() = default;

    // 复制构造函数被删除
    Symlink(const Symlink& other) = default;

    // 拷贝赋值运算符被删除
    Symlink& operator=(const Symlink& other) = default;

    Symlink(Symlink&& other) = default;

    Symlink& operator=(Symlink&& other) = default;

    Symlink(const String& path);

    Symlink(const String& path, const String& target);
public:
    virtual bool IsValid() const override;

    FileError Create(const String& target);

    Symlink CreateSymlink(const String& path) const;

    // 解析链接，获取链接最终指向目标的路径
    String ResolveLink() const;

    // 获取链接指向路径
    String TargetPath() const;

    // 设置链接指向目标
    FileError SetTarget(const String& target);
private:
    /// <summary>
    /// 链接指向路径
    /// </summary>
    String m_target;
};