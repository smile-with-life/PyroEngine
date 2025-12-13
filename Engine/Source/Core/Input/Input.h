#pragma once
#include "Core.h"

enum class KeyCode : uint16
{
	None = 0,

	// 字符键
	A, B, C, D, E, F, G, H, I, J, K, L, M,
	N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
	// 数字键
	Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
	// 空格键
	Space,
	// 符号键
	Tilde,					/* ~ */
	Minus,					/* - */
	Equals,					/* = */
	LeftBracket,			/* [ */
	RightBracket,			/* ] */
	Backslash = 0xDC,		/* \ */
	Slash,					/* / */
	Semicolon,				/* ; */
	Apostrophe,				/* ' */
	Comma,					/* , */
	Period,				    /* . */
	GraveAccent,			/* ` */
};