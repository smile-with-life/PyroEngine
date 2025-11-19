#pragma once
#include "Core.h"
#include "FileSystem.h"
#include "String/String.h"
#include "Container/Array.h"


class DateTime;
class Directory 
{
public:
    ~Directory();

    Directory(const String& path);

    // 复制构造函数被删除
    Directory(const Directory& other) = delete;

    // 拷贝赋值运算符被删除
    Directory& operator=(const Directory& other) = delete;

    Directory(Directory&& other);

    Directory& operator=(Directory&& other);
public:
    bool CreateFile(const String& name);

    bool CreateDirectory(const String& name);

    File GetFile(const String& name);

    Directory GetDirectory(const String& name);

    FileSystemEntry GetEntry(const String& name);

    Array<File> GetFiles(const String& searchPattern);

    Array<Directory> GetDirectories(const String& searchPattern);

    Array<FileSystemEntry> GetEntries(const String& name);

    bool CreateSymlink(const String& path);
};