#include "pch.h"

#include "FileEntry.h"

FileEntry::FileEntry(const String& path)
    : m_path(path)
{
    if (IsExists())
    {
        m_type = _GetType();
    }
    else
    {
        m_type = FileType::None;
    } 
}

FileError FileEntry::Delete()
{
    return FileSystem::Delete(m_path);
}

FileError FileEntry::CopyTo(const String& path, FileOption option) const
{
    return FileSystem::Copy(m_path, path, option);
}

FileError FileEntry::MoveTo(const String& path, FileOption option)
{
    FileError error = FileSystem::Move(m_path, path, option);
    if (!error)
    {
        m_path = path;
    }
    return error;
}

void FileEntry::Rename(const String& name)
{
    m_path = FileSystem::Rename(m_path, name);
}

FileType FileEntry::Type() const
{
    return m_type;
}

String FileEntry::FileName() const
{
    return FileSystem::FileName(m_path);
}

String FileEntry::FilePath() const
{
    return FileSystem::AbsolutePath(m_path);
}

String FileEntry::ParentPath() const
{
    return FileSystem::ParentPath(m_path);
}

String FileEntry::RootPath() const
{
    return FileSystem::RootPath(m_path);
}

DateTime FileEntry::CreationTime() const
{
    return DateTime();
}

DateTime FileEntry::CreationTimeUTC() const
{
    return DateTime();
}

DateTime FileEntry::LastWriteTime() const
{
    return DateTime();
}

DateTime FileEntry::LastWriteTimeUTC() const
{
    return DateTime();
}

FilePermission FileEntry::Permission() const
{
    return FileSystem::Permission(m_path);
}

bool FileEntry::SetPermission(FilePermission permission)
{
    return FileSystem::SetPermission(m_path, permission);
}

uint64 FileEntry::Size() const
{
    return FileSystem::FileSize(m_path);
}

bool FileEntry::IsExists() const
{
    return FileSystem::IsExists(m_path);
}

bool FileEntry::IsValid() const
{
    return m_type != FileType::None;
}

FileType FileEntry::_GetType() const
{
    if (FileSystem::IsDirectory(m_path))
        return FileType::Directory;
    if (FileSystem::IsSymlink(m_path))
        return FileType::Symlink;
    if (FileSystem::IsFile(m_path))
        return FileType::Regular;
    if (FileSystem::IsDevice(m_path))
        return FileType::Device;
    if (FileSystem::IsFIFO(m_path))
        return FileType::FIFO;
    if (FileSystem::IsSocket(m_path))
        return FileType::Socket;

    return FileType::None;
}
