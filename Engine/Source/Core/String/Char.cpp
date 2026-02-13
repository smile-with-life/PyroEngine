#include "pch.h"
#include "Char.h"
#include <cctype>
#include <algorithm>

/* public */

Char::Char(const Char& other)
    : m_data(other.m_data)
{
}

Char& Char::operator=(const Char& other)
{
    if (this != &other)
    {
        m_data = other.m_data;
    }
    return *this;
}

Char::Char(Char&& other) noexcept
    : m_data(std::move(other.m_data))
{
}

Char& Char::operator=(Char&& other) noexcept
{
    if (this != &other)
    {
        m_data = std::move(other.m_data);
    }
    return *this;
}

Char::Char(char ch)
{
    // ASCII字符直接存储
    if (static_cast<unsigned char>(ch) <= 0x7F)
    {
        m_data = std::string(1, ch);
    }
    else
    {
        // 非ASCII char被视为扩展ASCII，转换为UTF-8
        m_data = _EncodeUTF8(static_cast<uint32>(static_cast<unsigned char>(ch)));
    }
}

Char::Char(const std::string& utf8Char)
{
    if (_IsValidUTF8(utf8Char))
    {
        m_data = utf8Char;
    }
    else
    {
        // 无效UTF-8，构造为空字符
        m_data.clear();
    }
}

Char::Char(uint32 unicode)
{
    if (_IsValidCodepoint(unicode))
    {
        m_data = _EncodeUTF8(unicode);
    }
    else
    {
        // 无效码点，构造为空字符
        m_data.clear();
    }
}


uint32 Char::Unicode() const
{
    return _DecodeUTF8(m_data);
}

int32 Char::Size() const
{
    return static_cast<int32>(m_data.size());
}

bool Char::IsASCII() const
{
    return m_data.size() == 1 && (static_cast<unsigned char>(m_data[0]) <= 0x7F);
}

bool Char::IsValid() const
{
    return _IsValidUTF8(m_data);
}

bool Char::IsNull() const
{
    return m_data.empty() || (m_data.size() == 1 && m_data[0] == '\0');
}

bool Char::IsNumber() const
{
    uint32 code = Unicode();

    // ASCII数字: 0-9
    if (code >= 0x0030 && code <= 0x0039)
        return true;

    // 阿拉伯数字
    if (code >= 0x0660 && code <= 0x0669) return true;  // 阿拉伯-印度数字
    if (code >= 0x06F0 && code <= 0x06F9) return true;  // 扩展阿拉伯-印度数字
    if (code >= 0x0966 && code <= 0x096F) return true;  // 天城文数字
    if (code >= 0x09E6 && code <= 0x09EF) return true;  // 孟加拉文数字
    if (code >= 0x0A66 && code <= 0x0A6F) return true;  // 古木基文数字
    if (code >= 0x0AE6 && code <= 0x0AEF) return true;  // 古吉拉特文数字
    if (code >= 0x0B66 && code <= 0x0B6F) return true;  // 奥里亚文数字
    if (code >= 0x0BE6 && code <= 0x0BEF) return true;  // 泰米尔文数字
    if (code >= 0x0C66 && code <= 0x0C6F) return true;  // 泰卢固文数字
    if (code >= 0x0CE6 && code <= 0x0CEF) return true;  // 卡纳达文数字
    if (code >= 0x0D66 && code <= 0x0D6F) return true;  // 马拉雅拉姆文数字
    if (code >= 0x0E50 && code <= 0x0E59) return true;  // 泰文数字
    if (code >= 0x0ED0 && code <= 0x0ED9) return true;  // 老挝文数字
    if (code >= 0x0F20 && code <= 0x0F29) return true;  // 藏文数字
    if (code >= 0x1040 && code <= 0x1049) return true;  // 缅甸文数字
    if (code >= 0x1090 && code <= 0x1099) return true;  // 缅甸文掸邦数字
    if (code >= 0x17E0 && code <= 0x17E9) return true;  // 高棉文数字
    if (code >= 0x1810 && code <= 0x1819) return true;  // 蒙古文数字
    if (code >= 0x1946 && code <= 0x194F) return true;  // 林布文数字
    if (code >= 0x19D0 && code <= 0x19D9) return true;  // 新傣文数字
    if (code >= 0x1A80 && code <= 0x1A89) return true;  // 泰文数字（新）
    if (code >= 0x1A90 && code <= 0x1A99) return true;  // 老挝文数字（新）
    if (code >= 0x1B50 && code <= 0x1B59) return true;  // 巴厘文数字
    if (code >= 0x1BB0 && code <= 0x1BB9) return true;  // 巽他文数字
    if (code >= 0x1C40 && code <= 0x1C49) return true;  // 雷布查文数字
    if (code >= 0x1C50 && code <= 0x1C59) return true;  // 奥里亚文数字（补充）

    // 全角数字
    if (code >= 0xFF10 && code <= 0xFF19) return true;  // 全角数字

    // 罗马数字
    if (code >= 0x2160 && code <= 0x2188) return true;

    // 带圈数字
    if (code >= 0x2460 && code <= 0x2473) return true;  // 1-20
    if (code >= 0x24F5 && code <= 0x24FE) return true;  // 21-30

    // 带括号数字
    if (code >= 0x2474 && code <= 0x2487) return true;

    // 带点数字
    if (code >= 0x2488 && code <= 0x249B) return true;

    return false;
}

bool Char::IsLetter() const
{
    uint32 code = Unicode();

    // ASCII字母
    if ((code >= 0x0041 && code <= 0x005A) ||  // A-Z
        (code >= 0x0061 && code <= 0x007A))     // a-z
        return true;

    // Latin-1补充
    if ((code >= 0x00C0 && code <= 0x00D6) ||  // À-Ö
        (code >= 0x00D8 && code <= 0x00F6) ||  // Ø-ö
        (code >= 0x00F8 && code <= 0x00FF))    // ø-ÿ
        return true;

    // 希腊字母
    if ((code >= 0x0386 && code <= 0x03CE) &&
        code != 0x0387 && code != 0x038B && code != 0x038D)
        return true;

    // 西里尔字母
    if ((code >= 0x0400 && code <= 0x0481) ||
        (code >= 0x048A && code <= 0x04FF) ||
        (code >= 0x0500 && code <= 0x052F))
        return true;

    // 阿拉伯字母
    if ((code >= 0x0621 && code <= 0x064A) ||
        (code >= 0x066E && code <= 0x066F) ||
        (code >= 0x0671 && code <= 0x06D3) ||
        (code >= 0x06D5 && code <= 0x06D5))
        return true;

    // 希伯来字母
    if ((code >= 0x05D0 && code <= 0x05EA) ||
        (code >= 0x05EF && code <= 0x05F2))
        return true;

    // 中日韩表意字符
    if (IsCJK())
        return true;

    // 韩文
    if ((code >= 0xAC00 && code <= 0xD7A3) ||  // 韩文音节
        (code >= 0x1100 && code <= 0x11FF) ||  // 韩文字母
        (code >= 0x3131 && code <= 0x318E) ||  // 韩文兼容字母
        (code >= 0xFFA1 && code <= 0xFFDC))    // 半角韩文字母
        return true;

    return false;
}

bool Char::IsPunctuation() const
{
    uint32 code = Unicode();

    // ASCII标点
    if ((code >= 0x0021 && code <= 0x002F) ||  // ! " # $ % & ' ( ) * + , - . /
        (code >= 0x003A && code <= 0x0040) ||  // : ; < = > ? @
        (code >= 0x005B && code <= 0x0060) ||  // [ \ ] ^ _ `
        (code >= 0x007B && code <= 0x007E))    // { | } ~
        return true;

    // 全角标点
    if ((code >= 0x3000 && code <= 0x303F) ||  // CJK符号和标点
        (code >= 0xFF00 && code <= 0xFF0F) ||  // 全角ASCII变体
        (code >= 0xFF1A && code <= 0xFF1F) ||
        (code >= 0xFF3B && code <= 0xFF40) ||
        (code >= 0xFF5B && code <= 0xFF65))
        return true;

    // 通用标点
    if ((code >= 0x2000 && code <= 0x206F) ||  // 通用标点
        (code >= 0x2E00 && code <= 0x2E7F))    // 补充标点
        return true;

    return false;
}

bool Char::IsControl() const
{
    uint32 code = Unicode();

    // ASCII控制字符
    if (code < 0x0020 || code == 0x007F)
        return true;

    // C1控制字符
    if (code >= 0x0080 && code <= 0x009F)
        return true;

    return false;
}

bool Char::IsCJK() const
{
    uint32 code = Unicode();

    // CJK统一表意字符
    if (code >= 0x4E00 && code <= 0x9FFF)
        return true;

    // CJK扩展A
    if (code >= 0x3400 && code <= 0x4DBF)
        return true;

    // CJK扩展B
    if (code >= 0x20000 && code <= 0x2A6DF)
        return true;

    // CJK扩展C
    if (code >= 0x2A700 && code <= 0x2B73F)
        return true;

    // CJK扩展D
    if (code >= 0x2B740 && code <= 0x2B81F)
        return true;

    // CJK扩展E
    if (code >= 0x2B820 && code <= 0x2CEAF)
        return true;

    // CJK扩展F
    if (code >= 0x2CEB0 && code <= 0x2EBEF)
        return true;

    // CJK扩展G
    if (code >= 0x30000 && code <= 0x3134F)
        return true;

    // CJK兼容表意字符
    if ((code >= 0xF900 && code <= 0xFAFF) ||
        (code >= 0x2F800 && code <= 0x2FA1F))
        return true;

    // CJK部首
    if ((code >= 0x2E80 && code <= 0x2EFF) ||  // CJK部首补充
        (code >= 0x2F00 && code <= 0x2FDF) ||  // 康熙部首
        (code >= 0x31C0 && code <= 0x31EF))    // CJK笔画
        return true;

    // CJK符号和标点
    if (code >= 0x3000 && code <= 0x303F)
        return true;

    return false;
}

bool Char::IsEmoji() const
{
    uint32 code = Unicode();

    // 表情符号
    if (code >= 0x1F600 && code <= 0x1F64F)  // 表情符号
        return true;

    // 杂项符号和象形文字
    if (code >= 0x1F300 && code <= 0x1F5FF)
        return true;

    // 交通和地图符号
    if (code >= 0x1F680 && code <= 0x1F6FF)
        return true;

    // 补充符号和象形文字
    if (code >= 0x1F900 && code <= 0x1F9FF)
        return true;

    // 杂项符号
    if ((code >= 0x2600 && code <= 0x26FF) ||  // 杂项符号
        (code >= 0x2700 && code <= 0x27BF))    // 装饰符号
        return true;

    // 变体选择器
    if (code == 0xFE0F || code == 0x200D)
        return true;

    // 区域指示符（国旗）
    if (code >= 0x1F1E6 && code <= 0x1F1FF)
        return true;

    // 其他表情符号相关
    if (code == 0x203C || code == 0x2049 ||    // !!、!?
        code == 0x2122 || code == 0x2139 ||    // TM、ℹ
        code == 0x2194 || code == 0x2195 ||    // 箭头
        code == 0x21A9 || code == 0x21AA ||    // 返回箭头
        code == 0x231A || code == 0x231B ||    // 手表、闹钟
        code == 0x2328 || code == 0x23CF ||    // 键盘、喷气机
        code == 0x23E9 || code == 0x23EA ||    // 快进、快退
        code == 0x23EB || code == 0x23EC ||    // 快上、快下
        code == 0x23ED || code == 0x23EF ||    // 下一曲、播放/暂停
        code == 0x23F0 || code == 0x23F3 ||    // 闹钟、计时器
        code == 0x24C2 || code == 0x25AA ||    // Ⓜ、◻
        code == 0x25AB || code == 0x25B6 ||    // ◻、▶
        code == 0x25C0 || code == 0x25FB ||    // ◀、◻
        code == 0x25FC || code == 0x25FD ||    // ◻、◽
        code == 0x25FE || code == 0x2600)      // ◾、☀
        return true;

    return false;
}

bool Char::IsSpace() const
{
    uint32 code = Unicode();

    // ASCII空白字符
    if (code == 0x0020 ||  // 空格
        code == 0x0009 ||  // 水平制表符
        code == 0x000A ||  // 换行
        code == 0x000D ||  // 回车
        code == 0x000B ||  // 垂直制表符
        code == 0x000C)    // 换页
        return true;

    // Unicode空白字符
    if (code == 0x00A0 ||  // 不换行空格
        code == 0x1680 ||  // 欧甘空格
        code == 0x2000 ||  // 全身空格
        code == 0x2001 ||  // 全身空格
        code == 0x2002 ||  // 半身空格
        code == 0x2003 ||  // 全身空格
        code == 0x2004 ||  // 三分之一空格
        code == 0x2005 ||  // 四分之一空格
        code == 0x2006 ||  // 六分之一空格
        code == 0x2007 ||  // 数字空格
        code == 0x2008 ||  // 标点空格
        code == 0x2009 ||  // 细空格
        code == 0x200A ||  // 发丝空格
        code == 0x2028 ||  // 行分隔符
        code == 0x2029 ||  // 段分隔符
        code == 0x202F ||  // 窄不换行空格
        code == 0x205F ||  // 中数学空格
        code == 0x3000)    // 表意空格
        return true;

    return false;
}

bool Char::IsPrint() const
{
    uint32 code = Unicode();

    // 空字符和控制字符不可打印
    if (IsNull() || IsControl())
        return false;

    // ASCII可打印字符范围
    if (code >= 0x0020 && code <= 0x007E)
        return true;

    // 其他字符通常可打印
    return true;
}

bool Char::IsLower() const
{
    uint32 code = Unicode();

    // ASCII小写字母
    if (code >= 0x0061 && code <= 0x007A)
        return true;

    // Latin-1补充小写字母
    if ((code >= 0x00E0 && code <= 0x00F6) ||
        (code >= 0x00F8 && code <= 0x00FF))
        return true;

    // 其他常见小写字母范围
    if ((code >= 0x03B1 && code <= 0x03C1) ||  // 希腊小写
        (code >= 0x03C3 && code <= 0x03CB) ||  // 希腊小写
        (code >= 0x0430 && code <= 0x044F) ||  // 西里尔小写
        (code >= 0x0451 && code <= 0x045F))    // 西里尔小写
        return true;

    return false;
}

bool Char::IsUpper() const
{
    uint32 code = Unicode();

    // ASCII大写字母
    if (code >= 0x0041 && code <= 0x005A)
        return true;

    // Latin-1补充大写字母
    if ((code >= 0x00C0 && code <= 0x00D6) ||
        (code >= 0x00D8 && code <= 0x00DF))
        return true;

    // 其他常见大写字母范围
    if ((code >= 0x0391 && code <= 0x03A1) ||  // 希腊大写
        (code >= 0x03A3 && code <= 0x03AB) ||  // 希腊大写
        (code >= 0x0410 && code <= 0x042F) ||  // 西里尔大写
        (code >= 0x0401 && code <= 0x040F))    // 西里尔大写
        return true;

    return false;
}

Char Char::ToLower() const
{
    uint32 code = Unicode();

    // ASCII大写转小写
    if (code >= 0x0041 && code <= 0x005A)
    {
        return Char(code + 0x20);
    }

    // Latin-1补充大写转小写
    if (code >= 0x00C0 && code <= 0x00D6)
    {
        return Char(code + 0x20);
    }
    if (code >= 0x00D8 && code <= 0x00DE)
    {
        return Char(code + 0x20);
    }

    // 希腊字母大写转小写
    if (code >= 0x0391 && code <= 0x03A1)
    {
        return Char(code + 0x20);
    }
    if (code >= 0x03A3 && code <= 0x03AB)
    {
        return Char(code + 0x20);
    }

    // 西里尔字母大写转小写
    if (code >= 0x0410 && code <= 0x042F)
    {
        return Char(code + 0x20);
    }
    if (code >= 0x0401 && code <= 0x040F)
    {
        // Ё例外，需要特殊处理
        if (code == 0x0401) return Char(0x0451);
        return Char(code + 0x50);
    }

    // 其他字符保持不变
    return *this;
}

Char Char::ToUpper() const
{
    uint32 code = Unicode();

    // ASCII小写转大写
    if (code >= 0x0061 && code <= 0x007A)
    {
        return Char(code - 0x20);
    }

    // Latin-1补充小写转大写
    if (code >= 0x00E0 && code <= 0x00F6)
    {
        return Char(code - 0x20);
    }
    if (code >= 0x00F8 && code <= 0x00FE)
    {
        return Char(code - 0x20);
    }

    // 希腊字母小写转大写
    if (code >= 0x03B1 && code <= 0x03C1)
    {
        return Char(code - 0x20);
    }
    if (code >= 0x03C3 && code <= 0x03CB)
    {
        return Char(code - 0x20);
    }

    // 西里尔字母小写转大写
    if (code >= 0x0430 && code <= 0x044F)
    {
        return Char(code - 0x20);
    }
    if (code >= 0x0451 && code <= 0x045F)
    {
        // ё例外，需要特殊处理
        if (code == 0x0451) return Char(0x0401);
        return Char(code - 0x50);
    }

    // 其他字符保持不变
    return *this;
}

/* private */

bool Char::_IsValidCodepoint(uint32 cp) const
{
    // Unicode有效范围: 0x0000-0x10FFFF, 排除代理对范围 0xD800-0xDFFF
    return cp <= 0x10FFFF && (cp < 0xD800 || cp > 0xDFFF);
}

uint32 Char::_DecodeUTF8(const std::string& str) const
{
    if (str.empty()) return 0;

    const unsigned char* data = reinterpret_cast<const unsigned char*>(str.data());
    size_t len = str.size();

    if (len == 1)
    {
        // 单字节ASCII
        return data[0];
    }
    else if (len == 2)
    {
        // 2字节UTF-8: 110xxxxx 10xxxxxx
        return ((data[0] & 0x1F) << 6) | (data[1] & 0x3F);
    }
    else if (len == 3)
    {
        // 3字节UTF-8: 1110xxxx 10xxxxxx 10xxxxxx
        return ((data[0] & 0x0F) << 12) |
            ((data[1] & 0x3F) << 6) |
            (data[2] & 0x3F);
    }
    else if (len == 4)
    {
        // 4字节UTF-8: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        return ((data[0] & 0x07) << 18) |
            ((data[1] & 0x3F) << 12) |
            ((data[2] & 0x3F) << 6) |
            (data[3] & 0x3F);
    }
    return 0;
}

std::string Char::_EncodeUTF8(uint32 cp) const
{
    if (!_IsValidCodepoint(cp)) return "";

    std::string result;
    if (cp <= 0x7F)
    {
        // 1字节: 0xxxxxxx
        result += static_cast<char>(cp);
    }
    else if (cp <= 0x7FF)
    {
        // 2字节: 110xxxxx 10xxxxxx
        result += static_cast<char>(0xC0 | ((cp >> 6) & 0x1F));
        result += static_cast<char>(0x80 | (cp & 0x3F));
    }
    else if (cp <= 0xFFFF)
    {
        // 3字节: 1110xxxx 10xxxxxx 10xxxxxx
        result += static_cast<char>(0xE0 | ((cp >> 12) & 0x0F));
        result += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (cp & 0x3F));
    }
    else
    {
        // 4字节: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        result += static_cast<char>(0xF0 | ((cp >> 18) & 0x07));
        result += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
        result += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (cp & 0x3F));
    }
    return result;
}

bool Char::_IsValidUTF8(const std::string& str) const
{
    if (str.empty()) return false;

    const unsigned char* data = reinterpret_cast<const unsigned char*>(str.data());
    size_t len = str.size();

    // 单字节ASCII字符
    if (len == 1)
    {
        return (data[0] <= 0x7F);
    }

    // UTF-8最大4字节
    if (len < 2 || len > 4) return false;

    // 验证首字节格式
    if (len == 2 && (data[0] & 0xE0) != 0xC0) return false;
    if (len == 3 && (data[0] & 0xF0) != 0xE0) return false;
    if (len == 4 && (data[0] & 0xF8) != 0xF0) return false;

    // 验证后续字节格式（所有后续字节必须以10开头）
    for (size_t i = 1; i < len; ++i)
    {
        if ((data[i] & 0xC0) != 0x80)
        {
            return false; // 不是有效的10xxxxxx格式
        }
    }

    // 解码并验证码点有效性
    uint32 code = _DecodeUTF8(str);
    if (!_IsValidCodepoint(code)) return false;

    // 验证是否使用了最小字节数（防止过长编码）
    if (len == 2 && code < 0x0080) return false;
    if (len == 3 && code < 0x0800) return false;
    if (len == 4 && code < 0x10000) return false;

    return true;
}