#pragma once

#include "Core.h"
#include "String/String.h"

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
    /// <summary>
    /// 错误类型
    /// </summary>
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
    /// <summary>
    /// 默认构造函数
    /// </summary>
    FileError() = default;
    /// <summary>
    /// 析构函数
    /// </summary>
    ~FileError() = default;
    /// <summary>
    /// 拷贝构造函数
    /// </summary>
    /// <param name="other">要拷贝的对象</param>
    FileError(const FileError& other) = default;
    /// <summary>
    /// 拷贝赋值运算符
    /// </summary>
    /// <param name="other">要拷贝的对象</param>
    /// <returns>this</returns>
    FileError& operator=(const FileError& other) = default;
    /// <summary>
    /// 移动构造函数
    /// </summary>
    /// <param name="other">要移动的对象</param>
    FileError(FileError&& other) = default;
    /// <summary>
    /// 移动赋值运算符
    /// </summary>
    /// <param name="other">要移动的对象</param>
    /// <returns>this</returns>
    FileError& operator=(FileError&& other) = default;
    /// <summary>
    /// 构造函数
    /// </summary>
    /// <param name="type">错误类型</param>
    /// <param name="message">错误详细信息</param>
    FileError(ErrorType type, const String& message);
public:
    /// <summary>
    /// 隐式转换为 bool 类型，表示是否存在错误
    /// </summary>
    explicit operator bool() const;
public:
    /// <summary>
    /// 将平台特定错误码转换为统一的错误类型
    /// </summary>
    /// <param name="error">平台错误码（如errno）</param>
    /// <returns>对应的ErrorType枚举值</returns>
    static ErrorType PlatformErrorToType(int32 error);
public:
    /// <summary>
    /// 错误类型
    /// </summary>
    ErrorType Type = ErrorType::None;
    /// <summary>
    /// 错误详情
    /// </summary>
    String Detail;
};

/// <summary>
/// 文件系统
/// </summary>
class FileSystem
{
public:
    /// <summary>
    /// 文件系统空间信息
    /// </summary>
    struct SpaceInfo
    {
        uint64 Capacity;    // 文件系统的总大小
        uint64 Free;        // 文件系统上的可用空间
        uint64 Available;   // 非特权进程可用的空闲空间
    };
public:
    /// <summary>
    /// 获取文件的绝对路径
    /// </summary>
    /// <param name="path">相对路径或绝对路径</param>
    /// <returns>规范化后的绝对路径</returns>
    static String AbsolutePath(const String& path);
    /// <summary>
    /// 获取文件所在目录的路径
    /// </summary>
    /// <param name="path">文件路径</param>
    /// <returns>目录部分路径</returns>
    static String ParentPath(const String& path);
    /// <summary>
    /// 获取文件所在根目录的路径
    /// </summary>
    /// <param name="path">文件路径</param>
    /// <returns>根目录部分路径</returns>
    static String RootPath(const String& path);
    /// <summary>
    /// 获取文件名（不含目录部分）
    /// </summary>
    /// <param name="path">文件路径</param>
    /// <returns>文件名</returns>
    static String FileName(const String& path);
    /// <summary>
    /// 复制文件
    /// </summary>
    /// <param name="filePath">源文件路径</param>
    /// <param name="newPath">目标文件路径</param>
    /// <param name="option">已存在时的处理选项</param>
    /// <returns>操作结果错误信息（无错误时Type为None）</returns>
    static FileError Copy(const String& filePath, const String& newPath, FileOption option = FileOption::Cover);
    /// <summary>
    /// 移动文件（重命名）
    /// </summary>
    /// <param name="filePath">源文件路径</param>
    /// <param name="newPath">目标文件路径</param>
    /// <param name="option">已存在时的处理选项</param>
    /// <returns>操作结果错误信息</returns>
    static FileError Move(const String& filePath, const String& newPath, FileOption option = FileOption::Cover);
    /// <summary>
    /// 创建空文件
    /// 如果文件已存在，则返回错误（不覆盖）
    /// </summary>
    /// <param name="path">文件路径</param>
    /// <returns>操作结果错误信息</returns>
    static FileError CreateFile(const String& path);
    /// <summary>
    /// 创建目录
    /// 如果父目录不存在，将创建所有必需的父目录
    /// </summary>
    /// <param name="path">目录路径</param>
    /// <returns>操作结果错误信息</returns>
    static FileError CreateDirectory(const String& path);
    /// <summary>
    /// 获取当前工作目录
    /// </summary>
    /// <returns>当前工作目录的绝对路径</returns>
    static String CurrentPath();
    /// <summary>
    /// 设置当前工作目录
    /// </summary>
    /// <param name="path">要设置的工作目录路径</param>
    /// <returns>是否设置成功</returns>
    static bool SetCurrentPath(const String& path);
    /// <summary>
    /// 检查文件或目录是否存在
    /// </summary>
    /// <param name="filePath">文件/目录路径</param>
    /// <returns>是否存在</returns>
    static bool IsExists(const String& filePath);
    /// <summary>
    /// 删除文件或空目录
    /// </summary>
    /// <param name="path">要删除的文件/目录路径</param>
    /// <returns>操作结果错误信息</returns>
    static FileError Delete(const String& path);
    /// <summary>
    /// 重命名文件/目录
    /// </summary>
    /// <param name="path">原路径</param>
    /// <param name="name">新名称（不含目录部分）</param>
    /// <returns>重命名后的完整路径</returns>
    static String Rename(const String& path, const String& name);
    /// <summary>
    /// 获取文件系统空间信息
    /// </summary>
    /// <param name="path">路径（用于确定挂载点）</param>
    /// <returns>空间信息结构体</returns>
    static SpaceInfo Space(const String& path);
    /// <summary>
    /// 获取文件权限
    /// </summary>
    /// <param name="path">文件路径</param>
    /// <returns>文件权限掩码</returns>
    static FilePermission Permission(const String& path);
    /// <summary>
    /// 设置文件权限
    /// </summary>
    /// <param name="path">文件路径</param>
    /// <param name="permission">要设置的权限掩码</param>
    /// <returns>是否设置成功</returns>
    static bool SetPermission(const String& path, FilePermission permission);
    /// <summary>
    /// 获取文件大小
    /// </summary>
    /// <param name="path">文件路径</param>
    /// <returns>文件大小（字节），如果是目录则返回0</returns>
    static uint64 FileSize(const String& path);
    /// <summary>
    /// 判断路径是否为目录
    /// </summary>
    static bool IsDirectory(const String& path);
    /// <summary>
    /// 判断路径是否为普通文件
    /// </summary>
    static bool IsFile(const String& path);
    /// <summary>
    /// 判断是否为其他类型的文件（非普通文件、非目录、非符号链接）
    /// </summary>
    static bool IsOther(const String& path);
    /// <summary>
    /// 判断是否为设备文件
    /// </summary>
    static bool IsDevice(const String& path);
    /// <summary>
    /// 判断是否为命名管道（FIFO）
    /// </summary>
    static bool IsFIFO(const String& path);
    /// <summary>
    /// 判断是否为套接字文件
    /// </summary>
    static bool IsSocket(const String& path);
    /// <summary>
    /// 判断是否为符号链接
    /// </summary>
    static bool IsSymlink(const String& path);     
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