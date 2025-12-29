#include "pch.h"
#include "FileSystem.h"

String FileSystem::AbsolutePath(const String& path)
{
    return String(std::filesystem::absolute(path).string());
}

String FileSystem::FileDir(const String& path)
{
    return String(std::filesystem::path(std::filesystem::absolute(path)).parent_path().string());
}

String FileSystem::FileName(const String& path)
{
    return String(std::filesystem::path(path).filename().string());
}

void FileSystem::Copy(const String& filePath, const String& newPath, bool overwrite)
{

    std::error_code err;

    if (!Exists(newPath))
    {
        String newDir;
        if (IsDirectory(newPath))
        {
            newDir = newPath;
        }
        else
        {
            newDir = FileDir(newPath);
        }
        std::filesystem::create_directories(newDir, err);

        if (err.value()) [[unlikely]]
        {
            // @ log err.message();
            return;
        }
    }

    std::filesystem::copy(filePath, newPath, err);

    if (err.value()) [[unlikely]]
    {
        // @ log->err.message();
    }
}

void FileSystem::Move(const String& filePath, const String& newPath, bool overwrite)
{

}

void FileSystem::CreateFile(const String& path)
{

}

void FileSystem::CreateDirectory(const String& path)
{
    if (IsDirectory(path))
    {
        std::error_code err;

        std::filesystem::create_directories(path, err);

        if (err.value()) [[unlikely]]
        {
            // @log err.message();
        }
    }
}

String FileSystem::CurrentPath()
{
    return String();
}

bool FileSystem::SetCurrentPath(const String& path)
{
    return false;
}

bool FileSystem::Exists(const String& filePath)
{
    return std::filesystem::exists(filePath);
}

void FileSystem::Delete(const String& path)
{
    std::error_code err;

    std::filesystem::remove_all(path, err);

    if (err.value()) [[unlikely]]
    {
        // @log err.message();
    }
}

String FileSystem::Rename(const String& path, const String& name)
{
    if (Exists(path))
    {
        std::error_code err;
        String newPath = FileDir(path) + name;

        std::filesystem::rename(path, newPath, err);

        if (err.value()) [[unlikely]]
        {
            // @log err.message();
        }
        return newPath;
    }   
    return String();
}

bool FileSystem::IsDirectory(const String& path)
{
    return std::filesystem::is_directory(path);
}

bool FileSystem::IsFile(const String& path)
{
    return std::filesystem::is_regular_file(path);
}

bool FileSystem::IsOther(const String& path)
{
    return std::filesystem::is_other(path);
}

bool FileSystem::IsDevice(const String& path)
{
    return std::filesystem::is_character_file(path);
}

bool FileSystem::IsFIFO(const String& path)
{
    return std::filesystem::is_fifo(path);
}

bool FileSystem::IsSocket(const String& path)
{
    return std::filesystem::is_socket(path);
}

bool FileSystem::IsSymlink(const String& path)
{
    return std::filesystem::is_symlink(path);
}

FileSystem::SpaceInfo FileSystem::Space(const String& path)
{
    std::error_code err;

    std::filesystem::space_info space = std::filesystem::space(path, err);

    if (err.value()) [[unlikely]]
    {
        // @log err.message();
    }
    
    return SpaceInfo{ space.capacity, space.free, space.available };
}

FilePermission FileSystem::Permission()
{
    return FilePermission();
}

bool FileSystem::SetPermission(const String& path, FilePermission permission)
{
    return false;
}
