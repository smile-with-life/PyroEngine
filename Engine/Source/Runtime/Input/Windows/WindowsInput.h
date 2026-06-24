#pragma once

#include "Core.h"
#include "Platform.h"
#include "Input/Input.h"

inline KeyCode Win32KeyToKeyCode(WPARAM wParam)
{
    const UINT vk = (UINT)wParam;

    switch (vk)
    {
    case 'A': return KeyCode::A;
    case 'B': return KeyCode::B;
    case 'C': return KeyCode::C;
    case 'D': return KeyCode::D;
    case 'E': return KeyCode::E;
    case 'F': return KeyCode::F;
    case 'G': return KeyCode::G;
    case 'H': return KeyCode::H;
    case 'I': return KeyCode::I;
    case 'J': return KeyCode::J;
    case 'K': return KeyCode::K;
    case 'L': return KeyCode::L;
    case 'M': return KeyCode::M;
    case 'N': return KeyCode::N;
    case 'O': return KeyCode::O;
    case 'P': return KeyCode::P;
    case 'Q': return KeyCode::Q;
    case 'R': return KeyCode::R;
    case 'S': return KeyCode::S;
    case 'T': return KeyCode::T;
    case 'U': return KeyCode::U;
    case 'V': return KeyCode::V;
    case 'W': return KeyCode::W;
    case 'X': return KeyCode::X;
    case 'Y': return KeyCode::Y;
    case 'Z': return KeyCode::Z;

    case '0': return KeyCode::D0;
    case '1': return KeyCode::D1;
    case '2': return KeyCode::D2;
    case '3': return KeyCode::D3;
    case '4': return KeyCode::D4;
    case '5': return KeyCode::D5;
    case '6': return KeyCode::D6;
    case '7': return KeyCode::D7;
    case '8': return KeyCode::D8;
    case '9': return KeyCode::D9;

    case VK_F1:    return KeyCode::F1;
    case VK_F2:    return KeyCode::F2;
    case VK_F3:    return KeyCode::F3;
    case VK_F4:    return KeyCode::F4;
    case VK_F5:    return KeyCode::F5;
    case VK_F6:    return KeyCode::F6;
    case VK_F7:    return KeyCode::F7;
    case VK_F8:    return KeyCode::F8;
    case VK_F9:    return KeyCode::F9;
    case VK_F10:   return KeyCode::F10;
    case VK_F11:   return KeyCode::F11;
    case VK_F12:   return KeyCode::F12;

    case VK_RETURN:    return KeyCode::Enter;
    case VK_ESCAPE:    return KeyCode::Escape;
    case VK_BACK:      return KeyCode::Backspace;
    case VK_TAB:       return KeyCode::Tab;
    case VK_SPACE:     return KeyCode::Space;

    case VK_INSERT:    return KeyCode::Insert;
    case VK_DELETE:    return KeyCode::Delete;
    case VK_HOME:      return KeyCode::Home;
    case VK_END:       return KeyCode::End;
    case VK_PRIOR:     return KeyCode::PageUp;
    case VK_NEXT:      return KeyCode::PageDown;

    case VK_CAPITAL:   return KeyCode::CapsLock;
    case VK_LSHIFT:    return KeyCode::LeftShift;
    case VK_RSHIFT:    return KeyCode::RightShift;
    case VK_LCONTROL:  return KeyCode::LeftControl;
    case VK_RCONTROL:  return KeyCode::RightControl;
    case VK_LMENU:     return KeyCode::LeftAlt;
    case VK_RMENU:     return KeyCode::RightAlt;
    case VK_LWIN:      return KeyCode::LeftSystem;
    case VK_RWIN:      return KeyCode::RightSystem;

    case VK_LEFT:      return KeyCode::Left;
    case VK_RIGHT:     return KeyCode::Right;
    case VK_UP:        return KeyCode::Up;
    case VK_DOWN:      return KeyCode::Down;

    case VK_OEM_3:     return KeyCode::Grave;
    case VK_OEM_MINUS: return KeyCode::Minus;
    case VK_OEM_PLUS:  return KeyCode::Equals;
    case VK_OEM_4:     return KeyCode::LeftBracket;
    case VK_OEM_6:     return KeyCode::RightBracket;
    case VK_OEM_5:     return KeyCode::Backslash;
    case VK_OEM_2:     return KeyCode::Slash;
    case VK_OEM_1:     return KeyCode::Semicolon;
    case VK_OEM_7:     return KeyCode::Apostrophe;
    case VK_OEM_COMMA: return KeyCode::Comma;
    case VK_OEM_PERIOD:return KeyCode::Period;

    case VK_NUMPAD0:   return KeyCode::NumPad0;
    case VK_NUMPAD1:   return KeyCode::NumPad1;
    case VK_NUMPAD2:   return KeyCode::NumPad2;
    case VK_NUMPAD3:   return KeyCode::NumPad3;
    case VK_NUMPAD4:   return KeyCode::NumPad4;
    case VK_NUMPAD5:   return KeyCode::NumPad5;
    case VK_NUMPAD6:   return KeyCode::NumPad6;
    case VK_NUMPAD7:   return KeyCode::NumPad7;
    case VK_NUMPAD8:   return KeyCode::NumPad8;
    case VK_NUMPAD9:   return KeyCode::NumPad9;

    case VK_NUMLOCK:   return KeyCode::NumLock;
    case VK_ADD:       return KeyCode::NumPadAdd;
    case VK_SUBTRACT:  return KeyCode::NumPadSubtract;
    case VK_MULTIPLY:  return KeyCode::NumPadMultiply;
    case VK_DIVIDE:    return KeyCode::NumPadDivide;
    case VK_DECIMAL:   return KeyCode::NumPadDecimal;
    case VK_SEPARATOR: return KeyCode::NumPadEnter;
    }

    return KeyCode::None;
}

inline MouseCode Win32MouseToMouseCode(UINT msg)
{
    switch (msg)
    {
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_LBUTTONDBLCLK: return MouseCode::LeftButton;

    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_RBUTTONDBLCLK: return MouseCode::RightButton;

    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MBUTTONDBLCLK: return MouseCode::MiddleButton;

    case WM_MOUSEWHEEL:    return MouseCode::Scroll;
    }

    return MouseCode::None;
}