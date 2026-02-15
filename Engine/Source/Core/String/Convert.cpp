#include "pch.h"

#include "Convert.h"

namespace Convert
{
std::u16string UTF8ToUTF16(const std::string& u8str)
{
    std::u16string u16str;
    u16str.reserve(u8str.size()); // 预分配内存，减少扩容

    const uint8* ptr = reinterpret_cast<const uint8_t*>(u8str.data());
    const uint8* end = ptr + u8str.size();

    while (ptr < end)
    {
        uint32 codepoint = 0;
        uint64 bytes_needed = 0;

        // 第一步：判断 UTF-8 字节数并验证首字节
        if (*ptr <= 0x7F)
        {
            codepoint = *ptr;
            bytes_needed = 1;
        }
        else if (*ptr <= 0xDF)
        {
            codepoint = *ptr & 0x1F;
            bytes_needed = 2;
        }
        else if (*ptr <= 0xEF)
        {
            codepoint = *ptr & 0x0F;
            bytes_needed = 3;
        }
        else if (*ptr <= 0xF7)
        {
            codepoint = *ptr & 0x07;
            bytes_needed = 4;
        }
        else
        {
            // @ throw 无效 UTF-8 编码
        }

        // 第二步：检查剩余字节是否足够
        if (ptr + bytes_needed > end)
        {
            // @ throw 无效 UTF-8 编码
            return {};
        }

        // 第三步：解析后续字节（10xxxxxx 格式）
        for (uint64 i = 1; i < bytes_needed; ++i)
        {
            if ((ptr[i] & 0xC0) != 0x80)
            {
                // @ throw 无效 UTF-8 编码
                return {};
            }
            codepoint = (codepoint << 6) | (ptr[i] & 0x3F);
        }

        // 第四步：验证码点合法性
        // 检查过长编码（如 0x7F 用 2 字节表示）
        if ((bytes_needed == 2 && codepoint <= 0x7F) ||
            (bytes_needed == 3 && codepoint <= 0x7FF) ||
            (bytes_needed == 4 && codepoint <= 0xFFFF)) {
            // @ throw 过长编码
            return {};
        }
        // 检查码点范围（UTF-16 仅支持 0x0000 ~ 0x10FFFF）
        if (codepoint > 0x10FFFF)
        {
            // @ throw 码点超出 UTF - 16 范围
            return {};
        }
        // 排除 UTF-16 代理区（0xD800 ~ 0xDFFF）
        if (codepoint >= 0xD800 && codepoint <= 0xDFFF)
        {
            // @ throw 无效 UTF-8 编码
            return {};
        }

        // 第五步：转换为 UTF-16
        if (codepoint <= 0xFFFF)
        { // 单个 UTF-16 单元
            u16str.push_back(static_cast<char16_t>(codepoint));
        }
        else { // 代理对（2 个 UTF-16 单元）
            codepoint -= 0x10000;
            char16_t high_surrogate = static_cast<char16_t>(0xD800 + (codepoint >> 10));
            char16_t low_surrogate = static_cast<char16_t>(0xDC00 + (codepoint & 0x3FF));
            u16str.push_back(high_surrogate);
            u16str.push_back(low_surrogate);
        }

        // 移动指针到下一个字符
        ptr += bytes_needed;
    }

    return u16str;
}

std::u32string UTF8ToUTF32(const std::string& u8str)
{
    std::u32string u32str;
    u32str.reserve(u8str.size()); // 预分配，UTF-8 字节数 >= 字符数

    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(u8str.data());
    const uint8_t* end = ptr + u8str.size();

    while (ptr < end)
    {
        uint32_t codepoint = 0;
        size_t bytes_needed = 0;

        // 1. 根据首字节判断 UTF-8 序列长度
        if (*ptr <= 0x7F)
        {
            codepoint = *ptr;
            bytes_needed = 1;
        }
        else if (*ptr <= 0xDF)
        {
            codepoint = *ptr & 0x1F;
            bytes_needed = 2;
        }
        else if (*ptr <= 0xEF)
        {
            codepoint = *ptr & 0x0F;
            bytes_needed = 3;
        }
        else if (*ptr <= 0xF7)
        {
            codepoint = *ptr & 0x07;
            bytes_needed = 4;
        }
        else
        {
            // 非法的 UTF-8 首字节
            // 可以选择抛出异常或返回空字符串，这里采用静默失败返回空
            return {};
        }

        // 2. 检查剩余字节是否足够
        if (ptr + bytes_needed > end)
        {
            return {};
        }

        // 3. 解析后续字节（必须为 10xxxxxx 格式）
        for (size_t i = 1; i < bytes_needed; ++i)
        {
            if ((ptr[i] & 0xC0) != 0x80)
            {
                return {};
            }
            codepoint = (codepoint << 6) | (ptr[i] & 0x3F);
        }

        // 4. 验证码点合法性
        // 4.1 检查过长编码
        if ((bytes_needed == 2 && codepoint <= 0x7F) ||
            (bytes_needed == 3 && codepoint <= 0x7FF) ||
            (bytes_needed == 4 && codepoint <= 0xFFFF))
        {
            return {};
        }
        // 4.2 检查码点范围（Unicode 最大码点 0x10FFFF）
        if (codepoint > 0x10FFFF)
        {
            return {};
        }
        // 4.3 排除 UTF-16 代理区（这些在 UTF-32 中也是非法字符）
        if (codepoint >= 0xD800 && codepoint <= 0xDFFF)
        {
            return {};
        }

        // 5. 将合法码点存入 UTF-32 字符串
        u32str.push_back(static_cast<char32_t>(codepoint));

        // 移动指针到下一个字符
        ptr += bytes_needed;
    }

    return u32str;
}

std::string UTF16ToUTF8(const std::u16string& u16str)
{
    std::string u8str;
    // 预分配内存：UTF-8 最多 4 字节/字符，预留 2 倍空间（平衡内存占用和扩容）
    u8str.reserve(u16str.size() * 2);

    const char16_t* ptr = u16str.data();
    const char16_t* end = ptr + u16str.size();

    while (ptr < end)
    {
        uint32 codepoint = 0;
        char16_t current = *ptr;

        // 第一步：解析 UTF-16 字符（处理代理对）
        if (current >= 0xD800 && current <= 0xDBFF)
        { // 高代理（Surrogate High）
            // 检查是否有低代理跟随
            if (ptr + 1 >= end)
            {
                throw std::invalid_argument("Invalid UTF-16: incomplete surrogate pair");
            }
            char16_t low_surrogate = *(ptr + 1);
            // 验证低代理合法性
            if (low_surrogate < 0xDC00 || low_surrogate > 0xDFFF)
            {
                throw std::invalid_argument("Invalid UTF-16: invalid low surrogate");
            }
            // 计算完整码点
            codepoint = ((current - 0xD800) << 10) + (low_surrogate - 0xDC00) + 0x10000;
            ptr += 2; // 跳过代理对（2 个 UTF-16 单元）
        }
        else if (current >= 0xDC00 && current <= 0xDFFF)
        { // 孤立低代理（非法）
            throw std::invalid_argument("Invalid UTF-16: isolated low surrogate");
        }
        else
        { // 普通 UTF-16 单元（单字符）
            codepoint = current;
            ptr += 1;
        }

        // 第二步：验证码点范围（UTF-8 仅支持 0x0000 ~ 0x10FFFF）
        if (codepoint > 0x10FFFF)
        {
            throw std::invalid_argument("Invalid Unicode codepoint: out of range (0x10FFFF max)");
        }

        // 第三步：转换为 UTF-8 字节序列
        if (codepoint <= 0x7F)
        { // 1 字节：0xxxxxxx
            u8str.push_back(static_cast<char>(codepoint));
        }
        else if (codepoint <= 0x7FF)
        { // 2 字节：110xxxxx 10xxxxxx
            u8str.push_back(static_cast<char>(0xC0 | (codepoint >> 6)));
            u8str.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
        }
        else if (codepoint <= 0xFFFF)
        { // 3 字节：1110xxxx 10xxxxxx 10xxxxxx
            u8str.push_back(static_cast<char>(0xE0 | (codepoint >> 12)));
            u8str.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
            u8str.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
        }
        else
        { // 4 字节：11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            u8str.push_back(static_cast<char>(0xF0 | (codepoint >> 18)));
            u8str.push_back(static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F)));
            u8str.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
            u8str.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
        }
    }

    return u8str;
}

std::string UTF32ToUTF8(const std::u32string& u32str)
{
    std::string u8str;
    u8str.reserve(u32str.size() * 4);  // 最坏情况每个字符 4 字节

    for (char32_t cp : u32str)
    {
        if (cp > 0x10FFFF)
            throw std::invalid_argument("UTF-32 code point out of range");

        if (cp <= 0x7F)
            u8str.push_back(static_cast<char>(cp));
        else if (cp <= 0x7FF)
        {
            u8str.push_back(static_cast<char>(0xC0 | (cp >> 6)));
            u8str.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        }
        else if (cp <= 0xFFFF)
        {
            u8str.push_back(static_cast<char>(0xE0 | (cp >> 12)));
            u8str.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
            u8str.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        }
        else
        {
            u8str.push_back(static_cast<char>(0xF0 | (cp >> 18)));
            u8str.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
            u8str.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
            u8str.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        }
    }
    return u8str;
}

TString ToNativeString(const String& str)
{
#ifdef PLATFORM_NATIVE_UTF8
    return TString(reinterpret_cast<const tchar*>(str.ToCString()))
#elif PLATFORM_NATIVE_UTF16
    std::u16string u16str = UTF8ToUTF16(str);
    return TString(reinterpret_cast<const tchar*>(u16str.c_str()));
#elif PLATFORM_NATIVE_UTF32
    std::u32string u32str = UTF8ToUTF32(str);
    return TString(reinterpret_cast<const tchar*>(u32str.c_str()));
#endif
}

String ToString(const TString& str)
{
#ifdef PLATFORM_NATIVE_UTF8
    // tchar 为 UTF-8 编码
    const tchar* data = str.Data();
    if (!data) return String(); 
    return String(reinterpret_cast<const char*>(data));
#elif PLATFORM_NATIVE_UTF16
    // tchar 为 UTF-16 编码
    const tchar* data = str.Data();
    if (!data) return String();
    std::u16string u16str(reinterpret_cast<const char16*>(data));
    return UTF16ToUTF8(u16str);
#elif PLATFORM_NATIVE_UTF32
    // tchar 为 UTF-32 编码
    const tchar* data = str.Data();
    if (!data) return String();
    std::u32string u32str(reinterpret_cast<const char32*>(data));
    return UTF32ToUTF8(u32str); 
#endif
}
}// namespace Convert