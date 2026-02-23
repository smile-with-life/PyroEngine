#include "pch.h"

#include "Symlink.h"

Symlink::Symlink(const String& path)
    : FileEntry(path)
{
    if (IsValid())
    {
        m_target = TargetPath();
    }
}

Symlink::Symlink(const String& path, const String& target)
    : FileEntry(path), m_target(target)
{
    if (IsValid())
    {
        SetTarget(target);
    }
    else
    {
        Create(target);
    }
}

bool Symlink::IsValid() const
{
    return FileType::Symlink == Type();
}

FileError Symlink::Create(const String& target)
{
    // 检查文件是否已存在
    if (IsExists())
    {
        if (IsValid())
        {
            return FileError(FileError::AlreadyExists, "File already exists");
        }
        else
        {
            return FileError(FileError::InvalidPath, "File Type Error");
        }
    }

    std::error_code error;
    if (FileSystem::IsDirectory(target))
    {
        std::filesystem::create_directory_symlink(target, m_path, error);
    }
    else if (FileSystem::IsFile(target))
    {
        std::filesystem::create_symlink(target, m_path, error);
    }
    else
    {
        return FileError(FileError::NotFound, "Target file does not exist");
    }

    if (error)
    {
        return FileError(FileError::PlatformErrorToType(error.value()), error.message().c_str());
    }

    return FileError(FileError::None, "");
}

Symlink Symlink::CreateSymlink(const String& path) const
{
    return Symlink(path, m_path);
}

String Symlink::ResolveLink() const
{
    if (!IsValid())
        return String();

    std::error_code error;
    auto path = std::filesystem::read_symlink(m_path, error);

    if (error)
        return String();

    // 递归解析，直到找到最终目标
    while (FileSystem::IsSymlink(path.string()))
    {
        path = std::filesystem::read_symlink(path, error);
        if (error)
            break;
    }

    return String(path.string());
}

String Symlink::TargetPath() const
{
    if (IsValid())
    {
        std::error_code error;

        auto path = std::filesystem::read_symlink(m_path, error);

        if (!error)
        {
            return String(path.string());
        }
    }

    return String();
}

FileError Symlink::SetTarget(const String& target)
{
    if (IsExists())
    {
        if (IsValid())
        {
            Delete();
            return Create(target);
        }
        else
        {
            return FileError(FileError::InvalidPath, "File Type Error");
        }
    }
    else
    {
        m_target = target;
    }

    return FileError(FileError::None, "");
}
