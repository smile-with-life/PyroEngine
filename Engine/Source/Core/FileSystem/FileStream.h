#pragma once

#include "Core.h"
#include "String/String.h"
#include "FileSystem/FileSystem.h"


enum class OpenMode
{
    Default = 0, // 默认模式（读写模式，文件必须存在）
    Append = 1 << 1, // 追加模式
    Truncate = 1 << 2, // 截断模式
    Binary = 1 << 3, // 二进制模式
};

class FileStream
{
public:
    FileStream() = default;

    ~FileStream() = default;

    FileStream(const FileStream& other) = delete;

    FileStream& operator=(const FileStream& other) = delete;

    FileStream(FileStream&& other) = default;

    FileStream& operator=(FileStream&& other) = default;

    FileStream(const String& path, OpenMode mode = OpenMode::Default);
public:
    bool Open(const String& path, OpenMode mode = OpenMode::Default);

    void Close();  // 关闭文件

    bool Read(ByteArray& buffer, uint64 size);

    bool ReadLine(ByteArray& buffer, uint8 delim);

    bool ReadAll(ByteArray& buffer);

    bool Write(ByteArray& buffer, uint64 size);

    bool WriteAll(ByteArray& buffer);

    void Flush();

    uint64 Position();

    bool SeekBegin();

    bool Seek(uint64 position);

    bool SeekEnd();

    uint64 Size();
    
    bool IsOpen() const;

    bool IsEOF() const;

    bool IsValid() const;
public:
    friend void operator<<(ByteArray& buffer, FileStream& stream);

    friend void operator>>(ByteArray& buffer, FileStream& stream);
public:
    /// <summary>
    /// 文件流
    /// </summary>
    std::fstream m_file{};

};