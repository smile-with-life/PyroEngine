#pragma once

#include "Core.h"
#include "FileSystem/FileEntry.h"
#include "FileSystem/Symlink.h"
#include "FileSystem/FileStream.h"

class File : public FileEntry
{
public:
    ~File() = default;

    File(const File& other) = default;

    File& operator=(const File& other) = default;

    File(File&& other) = default;

    File& operator=(File&& other) = default;

    File(const String& path);
public:
    virtual bool IsValid() const override;

    FileError Create();

    // 创建指向 FilePath 的符号链接
    Symlink CreateSymlink(const String& path) const;

    FileStream CreateStream(OpenMode mode = OpenMode::Default) const;

    /// <summary>
    /// 通过截断或零填充更改常规文件的大小
    /// 如果是目录或其他类型的文件，则返回错误（不支持更改大小）
    /// </summary>
    /// <param name="path">文件路径</param>
    /// <param name="newSize">文件大小（字节）</param>
    FileError ResizeFile(uint64 newSize);
    
    // 获取文件内容的哈希值（使用 SHA-256 算法）
    String HashCode() const;

    String FileStem() const;

    String FileEXT() const;
};