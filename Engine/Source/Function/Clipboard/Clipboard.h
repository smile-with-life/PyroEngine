#pragma once
#include "Core.h"
#include "String/String.h"
/// <summary>
/// 剪切板
/// </summary>
class Clipboard
{
public:
    Clipboard(const Clipboard& other) = delete;

    Clipboard& operator=(const Clipboard& other) = delete;

    Clipboard(Clipboard&& other) noexcept = delete;

    Clipboard& operator=(Clipboard&& other) noexcept = delete;
public:
    void CopyToClipboard(const String& text);

    String PasteFromClipboard();
public:
    // 获取单例实例
    static Clipboard& GetInstance();
protected:
    Clipboard() = default;
    virtual ~Clipboard() = default;
};