#pragma once
#include "Core.h"
#include "String/String.h"
#include "File.h"
#include "Directory.h"

/// <summary>
/// 文件类型
/// </summary>
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

/// <summary>
/// 文件访问权限
/// </summary>
enum class FilePermission
{
    None,                   // 未知
    OwnerRead = 0400,       // 文件所有者有读权限       
    OwnerWrite = 0200,      // 文件所有者有写权限
    OwnerExec = 0100,       // 文件所有者有执行/搜索权限
    OwnerAll = 0700,        // 文件所有者有所有权限
    GroupRead = 0040,       // 文件用户组有读权限
    GroupWrite = 0020,      // 文件用户组有写权限
    GroupExec = 0010,       // 文件用户组有执行/搜索
    GroupAll = 0070,        // 文件用户组有所有权限
    OthersRead = 0004,      // 其他用户有读权限
    OthersWrite = 0002,     // 其他用户有写权限
    OthersExec = 0001,      // 其他用户有执行/搜索权
    OthersAll = 0007,       // 其他用户有所有权限
    All = 0777              // 所有用户都有全部权限
};

/// <summary>
/// 文件操作的语义
/// </summary>
enum class FileOption
{
    None,   // 报告错误
    Skip,   // 保留现有文件，不报告错误
    Cover   // 替换现有文件
};

/// <summary>
/// 文件错误
/// </summary>
class FileError
{
public:
    enum ErrorType
    {
        None = 0,           // 无错误
        NotFound,           // 文件不存在
        Permission,         // 权限错误
        AlreadyExists,      // 已存在
        IOError,            // I/O错误
        OutOfSpace,         // 空间不足
        InvalidPath,        // 路径无效
        Timeout,            // 超时
        Unknown             // 未知错误
    };
public:
    FileError() = default;

    ~FileError() = default;

    FileError(const FileError& other) = default;

    FileError& operator=(const FileError& other) = default;

    FileError(FileError&& other) = default;

    FileError& operator=(FileError&& other) = default;

    FileError(ErrorType type, const String& message);
public:
    explicit operator bool() const;
public:
    static ErrorType PlatformErrorToType(int32 error);
public:
    /// <summary>
    /// 错误类型
    /// </summary>
    ErrorType Type = ErrorType::None;
    /// <summary>
    /// 错误详情
    /// </summary>
    String Detail = nullptr;
};

/// <summary>
/// 文件系统
/// </summary>
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

    static FileError Copy(const String& filePath, const String& newPath, FileOption option = FileOption::Cover);

    static FileError Move(const String& filePath, const String& newPath, FileOption option = FileOption::Cover);

    static FileError CreateFile(const String& path);

    static FileError CreateDirectory(const String& path);

    static String CurrentPath();

    static bool SetCurrentPath(const String& path);

    static bool IsExists(const String& filePath);

    static FileError Delete(const String& path);

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

/// <summary>
/// 文件访问权限按位或操作
/// </summary>
/// <returns></returns>
inline FilePermission operator|(FilePermission left, FilePermission right)
{
    return static_cast<FilePermission>(static_cast<uint32>(left) | static_cast<uint32>(right));
}

/// <summary>
/// 文件访问权限按位与操作
/// </summary>
/// <returns></returns>
inline FilePermission operator&(FilePermission left, FilePermission right)
{
    return static_cast<FilePermission>(static_cast<uint32>(left) & static_cast<uint32>(right));
}