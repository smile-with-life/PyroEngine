#include "pch.h"

#include "File.h"

File::File(const String& path)
    : FileEntry(path)
{

}

bool File::IsValid() const
{
    return FileType::Regular == Type();
}

FileError File::Create()
{
    return FileSystem::CreateFile(m_path);
}

Symlink File::CreateSymlink(const String& path) const
{
    return Symlink(path, m_path);
}

FileStream File::CreateStream(OpenMode mode) const
{
    return FileStream(m_path, mode);
}

FileError File::ResizeFile(uint64 newSize)
{
    if (!IsExists())
        return FileError(FileError::NotFound, "File does not exist");

    if (!IsValid())
        return FileError(FileError::InvalidPath, "File Type Error");

    std::error_code error;
    std::filesystem::resize_file(m_path, newSize, error);

    if (error)
        return FileError(FileError::PlatformErrorToType(error.value()), error.message().c_str());

    return FileError(FileError::None, "");
}

String File::HashCode() const
{
    return String();
}

String File::FileStem() const
{
    if (IsValid())
    {
        return String(std::filesystem::path(m_path).stem().string());
    }
    return String();
}

String File::FileEXT() const
{  
    if (IsValid())
    {
        return String(std::filesystem::path(m_path).extension().string());
    }
    return String();
}