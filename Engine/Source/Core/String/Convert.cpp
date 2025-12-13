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
}// namespace Convert