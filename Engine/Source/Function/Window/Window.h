#pragma once
#include "Core.h"
#include "String/String.h"

/// <summary>
/// 窗口状态
/// </summary>
enum class WindowState
{
	Normal,
	Minimized,
	Maximized,
	Fullscreen
};

//窗口属性
struct WindowProps
{
	String Title = "Default Window";//窗口的标题
	int32 Width = 800;//窗口的宽度
	int32 Height = 400;//窗口的高度
	float PositionX = 100; //窗口在屏幕上的水平位置
	float PositionY = 100;//窗口在屏幕上垂直水平位置
	float Opacity = 1.0;// 窗口透明度
	bool IsVSync = false;// 是否启用垂直同步
	bool IsDisplayTaskbar = true;// 窗口是否显示在任务栏中
	bool IsTopmost = false;//是否应该位于所有其他窗口之上
	bool IsVisible = true;// 是否是可见的
	bool IsAcceptInput = true;//是否接收输入
	bool IsHasResizeBorder = true;//是否有可用于调整大小的边框
	bool IsSupportDragFile = false;//是否支持拖拽文件
	bool IsHasTitlebar = true;//是否有标题栏
	bool IsHasWindowMenu = true;// 是否有窗口菜单
	bool IsHasCloseButton = true;//是否有关闭按钮
	bool IsHasMinimizeButton = true;//是否有最小化按钮
	bool IsHasMaximizeButton = true;//是否有最大化按钮	
};

//窗口类
class Window
{
public:
	Window();

	virtual ~Window();

	explicit Window(WindowProps props);
public:
	// 设置窗口标题
	virtual void SetTitle(const String& title);

	// 设置窗口的透明度
	virtual void SetOpacity(const float opacity);

	// 设置垂直同步
	virtual void SetVSync(bool enabled);

	// 将窗口置于最前面
	virtual void BringFront();

	// 改变窗口大小
	virtual void ResizeWindow(int32 Width, int32 Height);

	// 移动窗口到指定位置
	virtual void MoveWindow(int32 x, int32 y);

	// 使窗口获得焦点
	virtual void FocusWindow();

	// 关闭窗口
	virtual void CloseWindow();

	// 最小化
	virtual void MinimizeWindow();

	// 最大化
	virtual void MaximizeWindow();

	// 恢复原状
	virtual void RestoreWindow();

	// 显示窗口
	virtual void ShowWindow();

	// 隐藏窗口
	virtual void HideWindow();

	// 启用或禁用窗口
	virtual void EnableWindow(bool enable);

	// 判断窗口是否在前台
	virtual bool IsFront() const;

	// 判断平台是否支持全屏模式
	virtual bool IsSupportFullscreen() const;

	// 判断窗口是否最大化
	virtual bool IsMax() const;

	// 判断窗口是否最小化
	virtual bool IsMin() const;

	// 判断窗口是否可见
	virtual bool IsVisible() const;

	// 判断指定坐标是否在窗口内
	virtual bool IsPointInWindow(int32 x, int32 y) const;

	// 判断是否是垂直同步
	virtual bool IsVSync() const;

	// 获取窗口标题
	virtual String GetTitle() const;

	// 获取窗口宽度
	virtual int32 GetWidth() const;

	// 获取窗口高度
	virtual int32 GetHeight() const;

	// 获取窗口水平位置
	virtual int32 GetPositionX() const;

	// 获取窗口垂直位置
	virtual int32 GetPositionY() const;

	// 获取与窗口关联的原生操作系统句柄
	virtual void* GetNativeHandle() const;
public:
	static Window* Create();

	static Window* Create(WindowProps props);
protected:
	WindowProps m_props;
};