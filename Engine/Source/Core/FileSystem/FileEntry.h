#pragma once

#include "Core.h"
#include "String/String.h"
#include "Time/DateTime.h"
#include "FileSystem/FileSystem.h"

/// <summary>
/// 文件条目
/// </summary>
class FileEntry
{
public:
    // 析构函数
    ~FileEntry() = default;

    // 复制构造函数被删除
    FileEntry(const FileEntry& other) = default;

    // 拷贝赋值运算符被删除
    FileEntry& operator=(const FileEntry& other) = default;

    // 移动构造函数
    FileEntry(FileEntry&& other) = default;

    // 移动赋值运算符
    FileEntry& operator=(FileEntry&& other) = default;

    // 构造函数
    FileEntry(const String& path);
public:
    // 删除文件
    virtual FileError Delete();

    // 将文件拷贝到指定路径
    virtual FileError CopyTo(const String& path, FileOption option = FileOption::Cover) const;

    // 将文件移动到指定路径
    virtual FileError MoveTo(const String& path, FileOption option = FileOption::Cover);

    // 重命名
    virtual void Rename(const String& name);

    // 获取文件类型
    virtual FileType Type() const;

    // 文件名
    virtual String FileName() const;

    // 文件路径
    virtual String FilePath() const;

    // 文件所在目录
    virtual String ParentPath() const;

    // 根目录
    virtual String RootPath() const;

    // 文件创建时间
    virtual DateTime CreationTime() const;

    // 文件创建时间 UTC
    virtual DateTime CreationTimeUTC() const;

    // 文件上次修改时间
    virtual DateTime LastWriteTime() const;

    // 文件上次修改时间 UTC
    virtual DateTime LastWriteTimeUTC() const;

    // 获取文件权限
    FilePermission Permission() const;

    // 设置文件权限
    bool SetPermission(FilePermission permission);

    // 文件大小（字节）
    virtual uint64 Size() const;

    // 判断条目是否存在
    virtual bool IsExists() const;

    virtual bool IsValid() const;
private:
    FileType _GetType() const;
protected:
    /// <summary>
    /// 文件路径
    /// </summary>
    String m_path;
    /// <summary>
    /// 文件类型
    /// </summary>
    FileType m_type;
};