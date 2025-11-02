#include "pch.h"
#include "File.h"


/* static */
void File::Copy(const String& filePath_, const String& newPath_)
{
    std::filesystem::path newPath(AbsolutePath(newPath_).ToStdString());
    std::filesystem::path filePath(AbsolutePath(filePath_).ToStdString());
    std::error_code err;

    if (!std::filesystem::exists(newPath))
    {
        std::filesystem::path newDir;
        if (std::filesystem::is_directory(newPath))
        {
            newDir = newPath;
        }
        else
        {
            newDir = newPath.parent_path();
        }
        std::filesystem::create_directories(newDir, err);

        if (err.value()) [[unlikely]]
        {
            // @log err.message();
            return;
        }
    }

    std::filesystem::copy(filePath, newPath, err);

    if (err.value()) [[unlikely]]
    {
        // log->err.message();
    }

}

bool File::Exists(const String& path_)
{
    std::filesystem::path path(path_.ToStdString());

    return std::filesystem::exists(path);
}

void File::Remove(const String& path_)
{
    std::filesystem::path path(path_.ToStdString());
    std::error_code err;

    std::filesystem::remove_all(path, err);

    if (err.value()) [[unlikely]]
    {
        // @log err.message();
    }
}

String File::Rename(const String& oldPath_, const String& newPath_)
{
    std::filesystem::path oldPath(AbsolutePath(oldPath_).ToStdString());
    std::filesystem::path newPath(AbsolutePath(newPath_).ToStdString());
    std::error_code err;

    std::filesystem::rename(oldPath, newPath, err);

    if (err.value()) [[unlikely]]
    {
        // @log err.message();
    }

    return newPath.string();
}

int64 File::FileSize(const String& path_)
{
    std::filesystem::path path(path_.ToStdString());
    int64 size = 0;
    std::error_code err;

    size = std::filesystem::file_size(path, err);

    if (err.value()) [[unlikely]]
    {
        // @log err.message();
        return -1;
    }

    return size;
}

bool File::ResizeFile(const String& path_, int64 size_)
{
    std::filesystem::path path(path_.ToStdString());
    std::error_code err;

    std::filesystem::resize_file(path, size_, err);

    if (err.value()) [[unlikely]]
    {
        // @log err.message();
        return false;
    }

    return true;
}

String File::AbsolutePath(const String& path_)
{
    std::filesystem::path path(path_.ToStdString());

    return String(std::filesystem::absolute(path).string());
}

String File::FileDir(const String& path_)
{
    std::filesystem::path path(std::filesystem::absolute(path_.ToStdString()));

    return String(path.parent_path().string());
}

String File::FileName(const String& path_)
{
    std::filesystem::path path(path_.ToStdString());

    return String(path.filename().string());
}

String File::FileStem(const String& path_)
{
    std::filesystem::path path(path_.ToStdString());

    return String(path.stem().string());
}

String File::FileEXT(const String& path_)
{
    std::filesystem::path path(path_.ToStdString());

    return String(path.extension().string());
}

void File::CreateDir(const String& dir_)
{
    std::filesystem::path dir(dir_.ToStdString());
    std::error_code err;

    std::filesystem::create_directories(dir, err);

    if (err.value()) [[unlikely]]
    {
        // @log err.message();
    }
}

bool File::IsDir(const String& path_)
{
    std::filesystem::path path(path_.ToStdString());

    return std::filesystem::is_directory(path);
}

bool File::IsFile(const String& path_)
{
    std::filesystem::path path(path_.ToStdString());

    return std::filesystem::is_regular_file(path);
}

bool File::IsOther(const String& path_)
{
    std::filesystem::path path(path_.ToStdString());

    return std::filesystem::is_other(path);
}

bool File::IsFIFO(const String& path_)
{
    std::filesystem::path path(path_.ToStdString());

    return std::filesystem::is_fifo(path);
}

bool File::IsSocket(const String& path_)
{
    std::filesystem::path path(path_.ToStdString());

    return std::filesystem::is_socket(path);
}

bool File::IsSymlink(const String& path_)
{
    std::filesystem::path path(path_.ToStdString());

    return std::filesystem::is_symlink(path);
}

/* member */
File::File(const String& path_)
    : m_path(std::filesystem::absolute(path_.ToStdString()))
{

}

File::~File()
{
    Close();
}

void File::Manage(const String& path_)
{
    m_path = std::filesystem::absolute(path_.ToStdString());
}

bool File::Open(OpenMode mode_)
{
    Close();
    std::ios_base::openmode mode = std::ios::binary;

    switch (mode_)
    {
    case OpenMode::ReadOnly:
    {
        mode |= std::ios::in;
        break;
    }
    case OpenMode::ReadWrite:
    {
        mode |= std::ios::in | std::ios::out;
        break;
    }
    case OpenMode::Append:
    {
        mode |= std::ios::out | std::ios::app;
        break;
    }
    case OpenMode::Cover:
    {
        mode |= std::ios::out | std::ios::trunc;
        break;
    }
    }// switch

    for (int tries = 0; tries < m_tries; ++tries)
    {
        // 关联文件流
        m_file.open(m_path, mode);
        if (m_file.is_open()) [[likely]]
        {
            return true;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(m_interval));
    }

    return false;
}

bool File::Open(const String& path_, OpenMode mode_)
{
    m_path = std::filesystem::absolute(path_.ToStdString());
    return Open(mode_);
}

void File::Close()
{
    m_file.close();
}

bool File::Read(ByteArray& buffer, int64 size)
{
    if (size)
    {
        buffer.Resize(size);
        m_file.read(&buffer[0], size);
        return !m_file.fail();
    }

    return true;

}

bool File::ReadAll(ByteArray& buffer)
{
    int64 size = Size();

    if (size)
    {
        buffer.Resize(size);
        m_file.read(&buffer[0], size);
        return !m_file.fail();
    }

    return true;

}

bool File::ReadLine(ByteArray& buffer)
{
    char ch = '\0';
    buffer.Clear();
    while (m_file.get(ch))
    {
        if (ch == '\n')
        {
            break;
        }
        buffer << ch;
    }
    return !m_file.fail();
}

bool File::Write(ByteArray& buffer, int64 size)
{
    if (size)
    {
        buffer.Resize(size);

        m_file.write(&buffer[0], size);
        return !m_file.fail();
    }

    return true;
}

bool File::WriteAll(ByteArray& buffer)
{
    int64 size = buffer.Size();

    if (size)
    {
        buffer.Resize(size);

        m_file.write(&buffer[0], size);
        return !m_file.fail();
    }

    return true;
}

void File::Flush()
{
    m_file.flush();
}

bool File::IsOpen()
{
    return m_file.is_open();
}

bool File::IsEOF()
{
    return m_file.eof();
}

bool File::Create() const
{
    //如果路径不存在则创建相应路径
    if (!std::filesystem::exists(m_path))
    {
        String dir = FileDir(m_path.string());
        CreateDir(dir);
        std::ofstream file(m_path);
        if (!file.is_open()) [[unlikely]]
        {
            return false;
        }
        file.close();
    }

    return true;
}

void File::Copy(const String& path_) const
{
    Copy(m_path.string(), path_);
}

void File::Remove()
{
    Remove(m_path.string());
    m_path = "";
}

void File::Rename(const String& path_)
{
    String newPath = Rename(m_path.string(), path_);
    m_path = std::filesystem::absolute(newPath.ToStdString());
}

int64 File::Size() const
{
    return FileSize(m_path.string());
}

bool File::Resize(int64 size) const
{
    return ResizeFile(m_path.string(), size);
}

void* File::GetNativeHandle() const
{
    return m_file.rdbuf();
}

String File::GetFilePath() const
{
    return m_path.string();
}

String File::GetFileDir() const
{
    return FileDir(m_path.string());
}

String File::GetFileName() const
{
    return FileName(m_path.string());
}

String File::GetFileStem() const
{
    return FileStem(m_path.string());
}

String File::GetFileEXT() const
{
    return FileEXT(m_path.string());
}
