#pragma once

#include "Core.h"
#include "Container/Array.h"
#include "FileSystem/FileEntry.h"
#include "FileSystem/Symlink.h"
#include "FileSystem/File.h"

class Directory : public FileEntry
{
public:
    ~Directory() = default;

    // 复制构造函数被删除
    Directory(const Directory& other) = default;

    // 拷贝赋值运算符被删除
    Directory& operator=(const Directory& other) = default;

    Directory(Directory&& other) = default;

    Directory& operator=(Directory&& other) = default;

    Directory(const String& path);
public:
    virtual bool IsValid() const override;

    FileError Create();

    Symlink CreateSymlink(const String& path) const;

    Array<FileEntry> GetEntries() const;

    Array<Directory> GetDirectories() const;

    Array<File> GetFiles() const;

    bool IsEmpty() const;
};