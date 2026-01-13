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
    // 检查文件是否已存在
    if (IsExists(path)) 
    {
        return FileError(FileError::AlreadyExists, "File already exists");
    }

    // 确保目录存在
    String dir = FileDir(path);
    if (!dir.IsEmpty()) 
    {
        auto dirError = CreateDirectory(dir);
        if (dirError) 
        {
            return dirError;
        }
    }

    // 创建文件
    std::ofstream file(path.ToStdString());
    if (!file.is_open())
    {
        return FileError(FileError::IOError, "Failed to create file");
    }
    file.close();

    return FileError(FileError::None, "");
}

FileError FileSystem::CreateDirectory(const String& path)
{
    // 如果目录已存在，直接返回成功
    if (!IsDirectory(path)) 
    {
        std::error_code error;
        std::filesystem::create_directories(path.ToStdString(), error);

        if (error)
        {
            return FileError(FileError::PlatformErrorToType(err.value()), err.message().c_str());
        }
    }

    return FileError(FileError::None, "");
}

String FileSystem::CurrentPath()
{
    return String(std::filesystem::current_path().string());
}

bool FileSystem::SetCurrentPath(const String& path)
{
    if (!IsExists(path) || !IsDirectory(path)) 
    {
        return false;
    }

    std::error_code error;
    std::filesystem::current_path(path.ToStdString(), error);

    return !error;
}

bool FileSystem::IsExists(const String& filePath)
{
    return std::filesystem::exists(filePath);
}

FileError FileSystem::Delete(const String& path)
{
    if (!IsExists(path)) 
    {
        return FileError(FileError::NotFound, "File or directory does not exist");
    }

    std::error_code error;
    uint64 count = std::filesystem::remove_all(path.ToStdString(), error);

    if (error) 
    {
        return FileError(FileError::PlatformErrorToType(error.value()), error.message());
    }

    if (count == 0)
    {
        return FileError(FileError::IOError, "Failed to delete file or directory");
    }

    return FileError(FileError::None, "");
}

String FileSystem::Rename(const String& path, const String& name)
{
    if (!IsExists(path)) 
    {
        return String();
    }

    String newPath = FileDir(path) + "/" + name;

    auto moveError = Move(path, newPath, FileOption::None);
    if (moveError) 
    {
        return String();
    }

    return newPath;
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
    std::error_code error;

    std::filesystem::space_info space = std::filesystem::space(path, error);

    if (error) [[unlikely]]
    {
        return SpaceInfo{ 0, 0, 0 };
    }
    
    return SpaceInfo{ space.capacity, space.free, space.available };
}

FilePermission FileSystem::Permission(const String& path)
{
    std::error_code error;
    auto status = std::filesystem::status(path, error);

    if (error) 
    {
        return FilePermission::None;
    }

    auto perms = status.permissions();
    uint32 result = 0;

    if ((perms & std::filesystem::perms::owner_read) != std::filesystem::perms::none)
        result |= static_cast<uint32>(FilePermission::OwnerRead);
    if ((perms & std::filesystem::perms::owner_write) != std::filesystem::perms::none)
        result |= static_cast<uint32>(FilePermission::OwnerWrite);
    if ((perms & std::filesystem::perms::owner_exec) != std::filesystem::perms::none)
        result |= static_cast<uint32>(FilePermission::OwnerExec);

    // Group 权限
    if ((perms & std::filesystem::perms::group_read) != std::filesystem::perms::none)
        result |= static_cast<uint32>(FilePermission::GroupRead);
    if ((perms & std::filesystem::perms::group_write) != std::filesystem::perms::none)
        result |= static_cast<uint32>(FilePermission::GroupWrite);
    if ((perms & std::filesystem::perms::group_exec) != std::filesystem::perms::none)
        result |= static_cast<uint32>(FilePermission::GroupExec);

    // Others 权限
    if ((perms & std::filesystem::perms::others_read) != std::filesystem::perms::none)
        result |= static_cast<uint32>(FilePermission::OthersRead);
    if ((perms & std::filesystem::perms::others_write) != std::filesystem::perms::none)
        result |= static_cast<uint32>(FilePermission::OthersWrite);
    if ((perms & std::filesystem::perms::others_exec) != std::filesystem::perms::none)
        result |= static_cast<uint32>(FilePermission::OthersExec);

    return static_cast<FilePermission>(result);
}

bool FileSystem::SetPermission(const String& path, FilePermission permission)
{
    std::error_code error;

    std::filesystem::perms perms = std::filesystem::perms::none;

    if ((permission & FilePermission::OwnerRead) == FilePermission::OwnerRead)
        perms |= std::filesystem::perms::owner_read;
    if ((permission & FilePermission::OwnerWrite) == FilePermission::OwnerWrite)
        perms |= std::filesystem::perms::owner_write;
    if ((permission & FilePermission::OwnerExec) == FilePermission::OwnerExec)
        perms |= std::filesystem::perms::owner_exec;

    if ((permission & FilePermission::GroupRead) == FilePermission::GroupRead)
        perms |= std::filesystem::perms::group_read;
    if ((permission & FilePermission::GroupWrite) == FilePermission::GroupWrite)
        perms |= std::filesystem::perms::group_write;
    if ((permission & FilePermission::GroupExec) == FilePermission::GroupExec)
        perms |= std::filesystem::perms::group_exec;

    if ((permission & FilePermission::OthersRead) == FilePermission::OthersRead)
        perms |= std::filesystem::perms::others_read;
    if ((permission & FilePermission::OthersWrite) == FilePermission::OthersWrite)
        perms |= std::filesystem::perms::others_write;
    if ((permission & FilePermission::OthersExec) == FilePermission::OthersExec)
        perms |= std::filesystem::perms::others_exec;

    std::filesystem::permissions(path.ToStdString(), perms, error);

    return !error;
}

uint64 FileSystem::FileSize(const String& path)
{
    if (!IsExists(path)) 
    {
        return 0;
    }

    try 
    {
        if (IsDirectory(path)) 
        {
            // 对于目录，可能需要递归计算所有文件大小
            uint64 size = 0;
            for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) 
            {
                if (entry.is_regular_file())
                {
                    try 
                    {
                        size += entry.file_size();
                    }
                    catch (...) 
                    {
                        // 跳过无法访问的文件
                    }
                }
            }
            return size;
        }
        else if (IsFile(path)) 
        {
            // 对于普通文件，直接获取大小
            std::error_code error;
            auto size = std::filesystem::file_size(path, error);
            if (error) 
            {
                return 0;
            }
            return static_cast<uint64>(size);
        }
    }
    catch (...) 
    {
        return 0;
    }

    return 0;
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