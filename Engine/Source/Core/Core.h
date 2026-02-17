#pragma once

#include "CoreType.h"

/// <summary>
/// 回调绑定宏
/// <para>仅绑定类成员函数</para>
/// </summary>
#define MemberFuncBind(func) [this](auto&&... args) -> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }

/// <summary>
/// 回调绑定宏
/// <para>绑定静态成员函数或普通函数</para>
/// </summary>
#define FuncBind(func) [](auto&&... args) -> decltype(auto) { return func(std::forward<decltype(args)>(args)...); }
