#pragma once
#include "Core.h"

enum class KeyCode : uint16
{
	None = 0,
	// 字符键
	A, B, C, D, E, F, G, H, I, J, K, L, M,
	N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
	// 数字键
	D0, D1, D2, D3, D4, D5, D6, D7, D8, D9,
	// 功能键
	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,
	// 导航键
	Enter, Escape, Backspace, Tab, Space,
	// 编辑键
	Insert, Delete, Home, End, PageUp, PageDown, 
	// 多媒体键
	Print, Scroll, Pause,
	// 修饰键
	CapsLock, LeftShift, RightShift, LeftControl, RightControl, LeftAlt, RightAlt, LeftSystem, RightSystem,
	// 方向键
	Left, Right, Up, Down,
	// 符号键
	Grave,					/* ` */
	Minus,					/* - */
	Equals,					/* = */
	LeftBracket,			/* [ */
	RightBracket,			/* ] */
	Backslash,				/* \ */
	Slash,					/* / */
	Semicolon,				/* ; */
	Apostrophe,				/* ' */
	Comma,					/* , */
	Period,				    /* . */
	// 小键盘
	NumPad0, NumPad1, NumPad2, NumPad3, NumPad4, NumPad5, NumPad6, NumPad7, NumPad8, NumPad9,
	NumLock, NumPadAdd, NumPadSubtract, NumPadMultiply, NumPadDivide, NumPadDecimal, NumPadEnter,
};
static inline constexpr int32 KeyCount = 115;
enum class KeyState
{
	Up,			// 松开
	Down,		// 按下
	Repeat,		// 重复
	Toggled		// 切换
};

class Keyboard
{
public:
	KeyState GetKeyState(KeyCode code)
	{

	}

	bool IsKeyDown(KeyCode code);

	bool IsKeyUp(KeyCode code);

	bool IsKeyRepeat(KeyCode code);

	bool IsKeyToggled(KeyCode code);

	std::array<KeyCode, KeyCount> m_keyStates;
};

enum class MouseCode
{
	None,
	LeftButton,
	RightButton,
	MiddleButton,
	ExpandButton1,
	ExpandButton2,
	Scroll
};

enum class MouseState
{
	Up,				// 松开
	Down,			// 按下
	DoubleClick,	// 双击
	Move			// 移动
};