#include "pch.h"

#include "FileStream.h"

FileStream::FileStream(const String& path, OpenMode mode)
{
    Open(path, mode);
}

bool FileStream::Open(const String& path, OpenMode mode)
{
    if (!FileSystem::IsExists(path))
        return false;

    std::ios::openmode openMode = std::ios::in | std::ios::out;

    if (static_cast<int>(mode) & static_cast<int>(OpenMode::Append))
    {
        openMode |= std::ios::app;
    }
    if (static_cast<int>(mode) & static_cast<int>(OpenMode::Truncate))
    {
        openMode |= std::ios::trunc;
    }
    if (static_cast<int>(mode) & static_cast<int>(OpenMode::Binary))
    {
        openMode |= std::ios::binary;
    }
    
    m_file.open(path, openMode);

    return m_file.is_open();
}

void FileStream::Close()
{
    if (m_file.is_open())
    {
        m_file.close();
    }
}

bool FileStream::Read(ByteArray& buffer, uint64 size)
{
    if (!m_file.is_open() || !m_file.good())
        return false;

    buffer.Clear();
    buffer.Resize(size);
    m_file.read(reinterpret_cast<char*>(buffer.Data()), static_cast<std::streamsize>(size));

    // 检查读取的实际字节数
    std::streamsize readBytes = m_file.gcount();
    if (readBytes < static_cast<std::streamsize>(size))
    {
        // 调整缓冲区大小以匹配实际读取的字节数
        buffer.Resize(static_cast<uint64>(readBytes));
    }

    return !m_file.fail() || m_file.eof();
}

bool FileStream::ReadLine(ByteArray& buffer, uint8 delim)
{
    if (!m_file.is_open() || !m_file.good())
        return false;

    buffer.Clear();
    std::string line;
    if (std::getline(m_file, line, static_cast<char>(delim)))
    {
        buffer.Resize(line.size());
        std::copy(line.begin(), line.end(), reinterpret_cast<char*>(buffer.Data()));

        return true;
    }

    return false;
}

bool FileStream::ReadAll(ByteArray& buffer)
{
    if (!m_file.is_open() || !m_file.good())
        return false;

    buffer.Clear();
    // 保存当前位置
    std::streampos currentPos = m_file.tellg();

    // 移动到文件末尾获取大小
    m_file.seekg(0, std::ios::end);
    uint64 size = static_cast<uint64>(m_file.tellg());

    // 移回开头
    m_file.seekg(0, std::ios::beg);

    // 读取全部内容
    buffer.Resize(size);
    m_file.read(reinterpret_cast<char*>(buffer.Data()), static_cast<std::streamsize>(size));

    // 恢复原位置（可选，取决于需求）
    m_file.seekg(currentPos, std::ios::beg);

    return !m_file.fail();
}

bool FileStream::Write(ByteArray& buffer, uint64 size)
{
    if (!m_file.is_open() || !m_file.good())
        return false;

    // 确保不会越界
    uint64 actualSize = size;
    if (actualSize > buffer.Size())
        actualSize = buffer.Size();

    m_file.write(reinterpret_cast<const char*>(buffer.Data()),
        static_cast<std::streamsize>(actualSize));

    return !m_file.fail();
}

bool FileStream::WriteAll(ByteArray& buffer)
{
    return Write(buffer, buffer.Size());
}

void FileStream::Flush()
{
    if (m_file.is_open())
    {
        m_file.flush();
    }
}

uint64 FileStream::Position()
{
    if (!m_file.is_open())
        return 0;

    // 获取读取位置（tellg）或写入位置（tellp）
    // 通常使用读取位置作为当前位置
    return static_cast<uint64>(m_file.tellg());
}

bool FileStream::SeekBegin()
{
    if (!m_file.is_open())
        return false;

    m_file.seekg(0, std::ios::beg);
    m_file.seekp(0, std::ios::beg);

    return !m_file.fail();
}

bool FileStream::Seek(uint64 position)
{
    if (!m_file.is_open())
        return false;

    m_file.seekg(static_cast<std::streamoff>(position), std::ios::beg);
    m_file.seekp(static_cast<std::streamoff>(position), std::ios::beg);

    return !m_file.fail();
}

bool FileStream::SeekEnd()
{
    if (!m_file.is_open())
        return false;

    m_file.seekg(0, std::ios::end);
    m_file.seekp(0, std::ios::end);

    return !m_file.fail();
}

uint64 FileStream::Size()
{
    if (!m_file.is_open())
        return 0;

    // 保存当前位置
    std::streampos current = m_file.tellg();

    // 移动到末尾获取大小
    m_file.seekg(0, std::ios::end);
    std::streampos size = m_file.tellg();

    // 恢复原位置
    m_file.seekg(current, std::ios::beg);

    return static_cast<uint64>(size);
}

bool FileStream::IsOpen() const
{
    return m_file.is_open();
}

bool FileStream::IsEOF() const
{
    if (!m_file.is_open())
        return true;

    return m_file.eof();
}

bool FileStream::IsValid() const
{
    if (!m_file.is_open())
        return false;

    return m_file.good();
}