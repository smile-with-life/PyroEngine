#pragma once
#include "Core.h"
#include "String/String.h"
#include "File.h"
#include "Directory.h"

// 文件类型
enum class FileType
{
    None,       // 未知
    Regular,    // 常规文件
    Directory,  // 目录
    Symlink,    // 符号链接
    Device,     // 设备文件
    FIFO,       // 管道文件
    Socket      // 套接字文件
};

// 文件访问权限
enum class FilePermission
{

};

// 文件操作的语义
enum class FileOption
{
    None,   // 报告错误
    Skip,   // 保留现有文件，不报告错误
    Cover   // 替换现有文件
};

class FileSystem
{
public:
    // 文件系统信息
    struct SpaceInfo
    {
        uint64 Capacity;    // 文件系统的总大小
        uint64 Free;        // 文件系统上的可用空间
        uint64 Available;   // 非特权进程可用的空闲空间
    };
public:
    static String AbsolutePath(const String& path);

    static String FileDir(const String& path);

    static String FileName(const String& path);

    static void Copy(const String& filePath, const String& newPath, bool overwrite = true);

    static void Move(const String& filePath, const String& newPath, bool overwrite = true);

    static void CreateFile(const String& path);

    static void CreateDirectory(const String& path);

    static String CurrentPath();

    static bool SetCurrentPath(const String& path);

    static bool Exists(const String& filePath);

    static void Delete(const String& path);

    static String Rename(const String& path, const String& name);

    static bool IsDirectory(const String& path);

    static bool IsFile(const String& path);

    static bool IsOther(const String& path);

    static bool IsDevice(const String& path);

    static bool IsFIFO(const String& path);

    static bool IsSocket(const String& path);

    static bool IsSymlink(const String& path);

    static SpaceInfo Space(const String& path);

    static FilePermission Permission();

    static bool SetPermission(const String& path, FilePermission permission);
};
