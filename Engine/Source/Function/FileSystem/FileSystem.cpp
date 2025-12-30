#include "pch.h"
#include "FileSystem.h"

#include <fstream>
#include <filesystem>

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

FileError FileSystem::Copy(const String& filePath, const String& newPath, FileOption option)
{
    // 检查源文件是否存在
    if (!IsExists(filePath))
    {
        return FileError(FileError::NotFound, "Source file does not exist");
    }

    // 检查目标文件是否已存在
    if (IsExists(newPath))
    {
        switch (option) 
        {
        case FileOption::Skip:
            return FileError(FileError::None, "");
        case FileOption::Cover:
            Delete(newPath);
            break;
        case FileOption::None:
            return FileError(FileError::AlreadyExists, "Destination file already exists");
        }
    }

    // 确保目标目录存在
    String targetDir;
    if (IsDirectory(newPath)) 
    {
        targetDir = newPath;
    }
    else 
    {
        targetDir = FileDir(newPath);
    }

    if (!targetDir.IsEmpty()) 
    {
        FileError dirError = CreateDirectory(targetDir);
        if (dirError) 
        {
            return dirError;
        }
    }

    std::error_code error;
    std::filesystem::copy(filePath, newPath,
        std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing,
        error);

    if (error) 
    {
        return FileError(FileError::PlatformErrorToType(error.value()), error.message());
    }

    return FileError(FileError::None, "");
}

FileError FileSystem::Move(const String& filePath, const String& newPath, FileOption option)
{
    // 检查源文件是否存在
    if (!IsExists(filePath)) 
    {
        return FileError(FileError::NotFound, "Source file does not exist");
    }

    // 检查目标文件是否已存在
    if (IsExists(newPath)) 
    {
        switch (option) 
        {
        case FileOption::Skip:
            return FileError(FileError::None, "");
        case FileOption::Cover:
            Delete(newPath);
            break;
        case FileOption::None:
            return FileError(FileError::AlreadyExists, "Destination file already exists");
        }
    }

    // 确保目标目录存在
    String targetDir = FileDir(newPath);
    if (!targetDir.IsEmpty()) 
    {
        auto dirError = CreateDirectory(targetDir);
        if (dirError) 
        {
            return dirError;
        }
    }

    std::error_code error;
    std::filesystem::rename(filePath, newPath, error);

    if (error) 
    {
        // 如果是跨设备移动，使用复制+删除
        if (error.value() == EXDEV) 
        {
            // 先复制
            auto copyError = Copy(filePath, newPath, FileOption::Cover);
            if (copyError) 
            {
                return copyError;
            }
            // 再删除源文件
            return Delete(filePath);
        }
        return FileError(FileError::PlatformErrorToType(error.value()), error.message());
    }

    return FileError(FileError::None, "");
}

FileError FileSystem::CreateFile(const String& path)
{
    if (!IsExists(path))
    {
        String dir = FileDir(path);
        if (!dir.IsEmpty() && !Exists(dir))
        {
            CreateDirectory(dir);
        }

        std::ofstream file(path.ToStdString());
        if (!file.is_open())
        {
            // 错误处理
        }
        file.close();
    }
    return FileError(FileError::AlreadyExists, "create file failed, file already exixts");
}

FileError FileSystem::CreateDirectory(const String& path)
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

FileError FileSystem::Delete(const String& path)
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

FileError::ErrorType FileError::PlatformErrorToType(int32 error)
{
#ifdef _WIN32
    switch (error)
    {
    case 2:  // ERROR_FILE_NOT_FOUND
    case 3:  // ERROR_PATH_NOT_FOUND
        return ErrorType::NotFound;
    case 5:   // ERROR_ACCESS_DENIED
    case 32:  // ERROR_SHARING_VIOLATION
    case 33:  // ERROR_LOCK_VIOLATION
        return ErrorType::Permission;
    case 80:  // ERROR_FILE_EXISTS
    case 183: // ERROR_ALREADY_EXISTS
        return ErrorType::AlreadyExists;
    case 112: // ERROR_DISK_FULL
    case 39:  // ERROR_HANDLE_DISK_FULL
        return ErrorType::OutOfSpace;
    case 161: // ERROR_BAD_PATHNAME
        return ErrorType::InvalidPath;
    case 23:  // ERROR_CRC
    case 27:  // ERROR_READ_FAULT
    case 29:  // ERROR_WRITE_FAULT
    case 31:  // ERROR_GEN_FAILURE
        return ErrorType::IOError;
    case 1460: // ERROR_TIMEOUT
        return ErrorType::Timeout;
    default:
        return ErrorType::Unknown;
    }
#else
    switch (error) 
    {
    case ENOENT:
        return ErrorType::NotFound;
    case EACCES:
    case EPERM:
    case EROFS:
        return ErrorType::Permission;
    case EEXIST:
        return ErrorType::AlreadyExists;
    case ENOSPC:
    case EDQUOT:
        return ErrorType::OutOfSpace;
    case EINVAL:
        return ErrorType::InvalidPath;
    case EIO:
        return ErrorType::IOError;
    case ETIMEDOUT:
        return ErrorType::Timeout;
    default:
        return ErrorType::Unknown;
    }
#endif
}
FileError::FileError(ErrorType type, const String& detail)
    : Type(type), Detail(detail)
{

}

FileError::operator bool() const
{
    return Type != ErrorType::None;
}