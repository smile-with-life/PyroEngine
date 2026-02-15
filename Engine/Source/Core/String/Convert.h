#pragma once

#include "Core.h"
#include "String.h"
#include "TString.h"
#include "Platform.h"

namespace Convert
{
/// <summary>
/// 将 UTF-8 编码的字符串转换为 UTF-16 编码的字符串。
/// </summary>
/// <param name="u8str">要转换的 UTF-8 字符串。</param>
/// <returns>转换后的 UTF-16 字符串。如果输入无效，返回空字符串。</returns>
std::u16string UTF8ToUTF16(const std::string& u8str);

/// <summary>
/// 将 UTF-8 编码的字符串转换为 UTF-32 编码的字符串。
/// </summary>
/// <param name="u8str">要转换的 UTF-8 字符串。</param>
/// <returns>转换后的 UTF-32 字符串。如果输入无效，返回空字符串。</returns>
std::u32string UTF8ToUTF32(const std::string& u8str);

/// <summary>
/// 将 UTF-16 编码的字符串转换为 UTF-8 编码的字符串。
/// </summary>
/// <param name="u16str">要转换的 UTF-16 字符串。</param>
/// <returns>转换后的 UTF-8 字符串。如果输入无效，返回空字符串。</returns>
std::string UTF16ToUTF8(const std::u16string& u16str);

/// <summary>
/// 将 UTF-32 编码的字符串转换为 UTF-8 编码的字符串。
/// </summary>
/// <param name="u32str">要转换的 UTF-32 字符串。</param>
/// <returns>转换后的 UTF-8 字符串。如果输入无效，返回空字符串。</returns>
std::string UTF32ToUTF8(const std::u32string& u32str);

/// <summary>
/// 将 UTF-8 编码的引擎字符串（String）转换为平台原生字符串（TString）。
/// <para>平台原生编码由 PLATFORM_NATIVE_UTF8/16/32 宏决定。</para>
/// </summary>
/// <param name="str">要转换的 UTF-8 字符串。</param>
/// <returns>平台原生编码的 TString 对象。</returns>
TString ToNativeString(const String& str);

/// <summary>
/// 将平台原生字符串（TString）转换为 UTF-8 编码的引擎字符串（String）。
/// <para>平台原生编码由 PLATFORM_NATIVE_UTF8/16/32 宏决定。</para>
/// </summary>
/// <param name="str">要转换的平台原生字符串。</param>
/// <returns>UTF-8 编码的 String 对象。</returns>
String ToString(const TString& str);
}// namespace Convert