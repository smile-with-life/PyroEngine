#pragma once
#include "Core.h"
#include "String/String.h"
#include "Memory/ByteArray.h"


enum class OpenMode
{
    ReadOnly,       // 只读
    ReadWrite,      // 读写
    Append,         // 追加
    Cover           // 覆盖
};

class File
{
public:
    // 默认构造函数
    File() = default;

    // 构造函数
    File(const String& path);

    // 析构函数
    ~File();

    // 管理文件
    void Manage(const String& path);

    // 打开文件
    bool Open(OpenMode mode);

    // 打开文件
    bool Open(const String& path, OpenMode mode);

    // 关闭文件
    void Close();

    // 读取文件，指定字节数
    bool Read(ByteArray& buffer, int64 size);

    // 读取全部文件
    bool ReadAll(ByteArray& buffer);

    // 按行读取文件
    bool ReadLine(ByteArray& buffer);

    // 写入文件，指定字节数
    bool Write(ByteArray& buffer, int64 size);

    // 全部写入文件
    bool WriteAll(ByteArray& buffer);

    // 刷新缓冲区
    void Flush();

    // 判断文件是否打开
    bool IsOpen();

    // 是否读到文件末尾
    bool IsEOF();

    // 文件不存在则创建文件
    bool Create() const;

    // 将文件拷贝到指定路径
    void Copy(const String& path) const;

    // 删除文件或目录
    void Remove();

    // 重命名文件
    void Rename(const String& path);

    // 文件大小，当文件存在时返回文件大小，否则返回 -1
    int64 Size() const;

    // 设置文件大小
    bool Resize(int64 size) const;

    // 获取与文件关联的原生操作系统句柄
    void* GetNativeHandle() const;

    // 获取文件完整路径
    String GetFilePath() const;

    // 获取文件所在目录
    String GetFileDir() const;

    // 获取文件名
    String GetFileName() const;

    // 获取文件名主干部分
    String GetFileStem() const;

    // 获取文件扩展名
    String GetFileEXT() const;
public:
    // 复制文件或目录
    static void Copy(const String& filePath, const String& newPath);

    // 检查文件或目录是否存在
    static bool Exists(const String& filePath);

    // 删除路径所标识的文件或目录
    static void Remove(const String& path);

    // 移动或重命名一个文件或目录
    static String Rename(const String& oldPath, const String& newPath);

    // 文件大小，当文件存在时返回文件大小，否则返回 -1
    static int64 FileSize(const String& path);

    // 设置文件大小
    static bool ResizeFile(const String& path, int64 size);

    // 返回绝对路径
    static String AbsolutePath(const String& path);

    // 返回文件所在目录
    static String FileDir(const String& path);

    // 返回文件名
    static String FileName(const String& path);

    // 返回文件名主体部分
    static String FileStem(const String& path);

    // 返回文件扩展名
    static String FileEXT(const String& path);

    // 创建目录，当给定路径不为目录时，行为未定义
    static void CreateDir(const String& dir);

    // 判断给定的路径是否表示一个目录
    static bool IsDir(const String& path);

    // 判断给定的路径是否表示一个常规文件
    static bool IsFile(const String& path);

    // 判断给定的路径是否表示一个其他文件
    static bool IsOther(const String& path);

    // 判断给定的路径是否表示一个命名管道
    static bool IsFIFO(const String& path);

    // 判断给定的路径是否表示一个具名 IPC 套接字
    static bool IsSocket(const String& path);

    // 判断给定的路径是否表示一个符号链接
    static bool IsSymlink(const String& path);
private:
    // 尝试打开文件时应该重试的次数
    const int m_tries = 3;

    // 每次尝试打开文件之间的时间间隔ms
    const unsigned int m_interval = 400;

    // 文件的路径
    std::filesystem::path m_path;

    // 文件流
    std::fstream m_file{};
};
