#pragma once
#include "Core.h"
#include "FileSystem.h"
#include "String/String.h"
#include "Memory/ByteArray.h"

class DateTime;
enum class OpenMode
{
    ReadOnly,       
    ReadWrite,      
    Append,         
    Cover           
};

class File
{
public:
    ~File();

    File(const String& path);

    // 复制构造函数被删除
    File(const File& other) = delete;

    // 拷贝赋值运算符被删除
    File& operator=(const File& other) = delete;

    File(File&& other);

    File& operator=(File&& other);
public:
    // 在指定的模式中打开文件,以二进制方式进行读写
    bool Open(OpenMode mode);

    // 在指定的模式中打开文件,以文本方式进行读写
    bool OpenText(OpenMode mode);
  
    // 关闭文件
    void Close();
    
    bool Read(ByteArray& buffer, int64 size);

    bool ReadAll(ByteArray& buffer);

    bool ReadLine(ByteArray& buffer);

    bool Write(ByteArray& buffer, int64 size);
    
    bool WriteAll(ByteArray& buffer);

    bool WriteLine(ByteArray& buffer);
    
    void Flush();

    // 创建指向 FilePath 的符号链接
    bool CreateSymlink(const String& path);

    bool Resize(int64 size) const;

    int64 Size() const;

    String Stem() const;

    String EXT() const;

    bool IsOpen();

    bool IsEOF();  
private:
    // 文件的路径
    String m_path;

    // 文件流
    std::fstream m_file{};
};
