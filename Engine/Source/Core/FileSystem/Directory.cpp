#include "pch.h"

#include "Directory.h"

Directory::Directory(const String& path)
    : FileEntry(path)
{

}

bool Directory::IsValid() const
{
    return FileType::Directory == Type();
}

FileError Directory::Create()
{
    return FileSystem::CreateDirectory(m_path);
}

Symlink Directory::CreateSymlink(const String& path) const
{
    return Symlink(path, m_path);
}

Array<FileEntry> Directory::GetEntries() const
{
    if (!IsValid())
        return Array<FileEntry>();

    Array<FileEntry> entries;

    std::error_code error; 

    // 使用 directory_iterator 遍历目录
    for (const auto& entry : std::filesystem::directory_iterator(m_path, error))
    {
        if (error)
            break;  // 如果出现错误，停止遍历

        std::filesystem::path entryPath = entry.path();
        String path(entryPath.string());

        // 创建 FileEntry 对象并添加到数组
        entries.Add(FileEntry(path));
    }

    return entries;
}

Array<Directory> Directory::GetDirectories() const
{
    if (!IsValid())
        return Array<Directory>();

    Array<Directory> dirs;

    std::error_code error;

    // 使用 directory_iterator 遍历目录
    for (const auto& entry : std::filesystem::directory_iterator(m_path, error))
    {
        if (error)
            break;  // 如果出现错误，停止遍历

        std::filesystem::path entryPath = entry.path();
        String path(entryPath.string());

        // 创建 Directory 对象
        Directory dir(path);

        if (dir.IsValid())
        {
            dirs.Add(dir);
        }      
    }

    return dirs;
}

Array<File> Directory::GetFiles() const
{
    if (!IsValid())
        return Array<File>();

    Array<File> files;

    std::error_code error;

    // 使用 directory_iterator 遍历目录
    for (const auto& entry : std::filesystem::directory_iterator(m_path, error))
    {
        if (error)
            break;  // 如果出现错误，停止遍历

        std::filesystem::path entryPath = entry.path();
        String path(entryPath.string());

        // 创建 Directory 对象
        File file(path);

        if (file.IsValid())
        {
            files.Add(file);
        }
    }

    return files;
}

bool Directory::IsEmpty() const
{
    if (!IsValid())
        return true;
    auto entries = GetEntries();
    return entries.Size() == 0;
}
