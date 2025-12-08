#pragma once
#include "Core.h"
#include "FileSystem.h"
#include "String/String.h"

class DateTime;
// 文件系统条目
class FileSystemEntry
{
public:
    // 析构函数
    ~FileSystemEntry();

    // 构造函数
    FileSystemEntry(const String& path);

    // 复制构造函数被删除
    FileSystemEntry(const FileSystemEntry& other) = delete;

    // 拷贝赋值运算符被删除
    FileSystemEntry& operator=(const FileSystemEntry& other) = delete;

    // 移动构造函数
    FileSystemEntry(FileSystemEntry&& other);

    // 移动赋值运算符
    FileSystemEntry& operator=(FileSystemEntry&& other);
public:
    // 创建文件
    virtual bool Create() const;

    // 删除文件
    virtual void Delete();

    // 将文件拷贝到指定路径
    virtual void CopyTo(const String& path) const;

    // 将文件移动到指定路径
    virtual void MoveTo(const String& path) const;

    // 重命名
    virtual void Rename(const String& name);

    // 获取文件类型
    virtual FileType Type();

    // 文件名
    virtual String Name();

    // 文件路径
    virtual String Path();

    // 文件所在目录
    virtual String Parent();

    // 根目录
    virtual String Root();

    // 文件创建时间
    virtual DateTime CreationTime() const;

    // 文件创建时间 UTC
    virtual DateTime CreationTimeUTC() const;

    // 文件上次访问时间
    virtual DateTime LastAccessTime() const;

    // 文件上次访问时间 UTC
    virtual DateTime LastAccessTimeUTC() const;

    // 文件上次修改时间
    virtual DateTime LastWriteTime() const;

    // 文件上次修改时间 UTC
    virtual DateTime LastWriteTimeUTC() const;

    // 判断条目是否存在
    virtual bool Exists();

    // 判断条目是否有效
    virtual bool IsValid();
};