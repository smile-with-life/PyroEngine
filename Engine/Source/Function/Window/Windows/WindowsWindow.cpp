#include "pch.h"
#include "WindowsWindow.h"
#include "String/Convert.h"
/* static */
Window* Window::Create()
{
    return new WindowsWindow();
}

Window* Window::Create(WindowProps props)
{
    return new WindowsWindow(props);
}

/* public */
WindowsWindow::WindowsWindow()
    : Window()
{
    _CreatePlatformWindow();
}

WindowsWindow::~WindowsWindow()
{

}

WindowsWindow::WindowsWindow(WindowProps props)
    : Window(props)
{
    _CreatePlatformWindow();
}

void WindowsWindow::SetTitle(const String& title)
{
    m_props.Title = title;
    SetWindowTextW(m_hWnd, Convert::ToNativeString(m_props.Title));
}


void WindowsWindow::SetOpacity(const float opacity)
{
    m_props.Opacity = opacity;
    SetLayeredWindowAttributes(m_hWnd, 0, static_cast<BYTE>(m_props.Opacity * 255.0f), LWA_ALPHA);
}

void WindowsWindow::SetVSync(bool enabled)
{
    m_props.IsVSync = enabled;
    // @ 实现
}

void WindowsWindow::BringFront()
{
    ::BringWindowToTop(m_hWnd);
}

void WindowsWindow::ResizeWindow(int32 width, int32 height)
{
    m_props.Width = width;
    m_props.Height = height;

    ::MoveWindow(m_hWnd, m_props.PositionX, m_props.PositionY, m_props.Width, m_props.Height, true);
}

void WindowsWindow::MoveWindow(int32 x, int32 y)
{
    m_props.PositionX = x;
    m_props.PositionY = y;

    ::MoveWindow(m_hWnd, m_props.PositionX, m_props.PositionY, m_props.Width, m_props.Height, false);
}

void WindowsWindow::FocusWindow()
{

}

void WindowsWindow::CloseWindow()
{
    ::DestroyWindow(m_hWnd);
}

void WindowsWindow::MinimizeWindow()
{
    ::ShowWindow(m_hWnd, SW_MINIMIZE);
}

void WindowsWindow::MaximizeWindow()
{
    ::ShowWindow(m_hWnd, SW_MAXIMIZE);
}

void WindowsWindow::RestoreWindow()
{
    ::ShowWindow(m_hWnd, SW_RESTORE);
}

void WindowsWindow::ShowWindow()
{
    ::ShowWindow(m_hWnd, SW_SHOW);
}

void WindowsWindow::HideWindow()
{
    ::ShowWindow(m_hWnd, SW_HIDE);
}

void WindowsWindow::EnableWindow(bool enabled)
{
    m_props.IsAcceptInput = enabled;
    ::EnableWindow(m_hWnd, m_props.IsAcceptInput);
}

bool WindowsWindow::IsFront() const
{
    return ::GetForegroundWindow() == m_hWnd;
}

bool WindowsWindow::IsSupportFullscreen() const
{
    return false;
}

bool WindowsWindow::IsMax() const
{
    return ::IsZoomed(m_hWnd);
}

bool WindowsWindow::IsMin() const
{
    return ::IsIconic(m_hWnd);
}

bool WindowsWindow::IsVisible() const
{
    return ::IsWindowVisible(m_hWnd);
}

bool WindowsWindow::IsPointInWindow(int32 x, int32 y) const
{
    return false;
}

bool WindowsWindow::IsVSync() const
{
    return false;
}

String WindowsWindow::GetTitle() const
{
    return "";
}

int32 WindowsWindow::GetWidth() const
{
    return int32();
}

int32 WindowsWindow::GetHeight() const
{
    return int32();
}

int32 WindowsWindow::GetPositionX() const
{
    return int32();
}

int32 WindowsWindow::GetPositionY() const
{
    return int32();
}

void* WindowsWindow::GetNativeHandle() const
{
    return m_hWnd;
}

/* private */
void WindowsWindow::_CreatePlatformWindow()
{
    uint32 WindowExStyle = 0;
    uint32 WindowStyle = 0;
    WindowExStyle |= WS_EX_WINDOWEDGE;
    //WindowExStyle |= WS_EX_LAYERED;// 窗口支持透明度
    //WindowExStyle |= WS_EX_COMPOSITED;

    if (m_props.IsDisplayTaskbar)
    {
        WindowExStyle |= WS_EX_APPWINDOW;// 当窗口可见时，将窗口强制显示到任务栏上
    }
    else
    {
        WindowExStyle |= WS_EX_TOOLWINDOW;// 窗口不会显示到任务栏上
    }

    if (m_props.IsSupportDragFile)
    {
        WindowExStyle |= WS_EX_ACCEPTFILES;// 窗口接受拖放文件
    }

    if (m_props.IsTopmost)
    {
        WindowExStyle |= WS_EX_TOPMOST;// 窗口应放置在所有非最顶层的窗口之上，并且应保持其上方
    }

    WindowStyle = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

    if (!m_props.IsAcceptInput)
    {
        WindowStyle |= WS_DISABLED;// 窗口最初处于禁用状态
    }

    if (m_props.IsHasResizeBorder)
    {
        WindowStyle |= WS_SIZEBOX;// 窗口具有调整大小的边框
    }
    else
    {
        WindowStyle |= WS_BORDER;// 窗口具有细线边框
    }

    if (m_props.IsHasTitlebar)
    {
        WindowStyle |= WS_CAPTION;// 窗口具有标题栏
    }

    if (m_props.IsHasWindowMenu)
    {
        WindowStyle |= WS_SYSMENU;// 窗口的标题栏上有一个窗口菜单
        if (m_props.IsHasMinimizeButton)
        {
            WindowStyle |= WS_MINIMIZEBOX;// 窗口具有最小化按钮
        }
        if (m_props.IsHasMaximizeButton)
        {
            WindowStyle |= WS_MAXIMIZEBOX;// 窗口具有最大化按钮
        }
    }
    
    // 创建窗口
    m_hWnd = ::CreateWindowExW(WindowExStyle,   // 窗口扩展样式
        TEXT("PyroEngineWindow"),               // 窗口类名
        Convert::ToNativeString(m_props.Title), // 窗口标题
        WindowStyle,                            // 窗口样式
        m_props.PositionX,                      // 窗口的初始水平位置
        m_props.PositionY,                      // 窗口的初始垂直位置
        m_props.Width,                          // 窗口的宽度
        m_props.Height,                         // 窗口的高度
        NULL,                                   // 父窗口句柄
        NULL,                                   // 菜单的句柄
        GWindowsInstance,                       // 应用程序句柄
        NULL);                                  // 传给窗口过程函数的参数

    if (!m_props.IsHasCloseButton) // 窗口是否具有关闭按钮
    {
        EnableMenuItem(GetSystemMenu(m_hWnd, false), SC_CLOSE, MF_GRAYED);
    }
    // 设置透明度
    SetOpacity(m_props.Opacity);
    
    if (m_props.IsVisible)
    {
        ShowWindow();
    }
    

    //if ()
    //{
    //    // Tell OLE that we are opting into drag and drop.
    //    // Only makes sense for regular windows (windows that last a while.)
    //    RegisterDragDrop(m_hWnd, this);

    //    // Listing to clipboard change event
    //    ::AddClipboardFormatListener(m_hWnd);
    //} 文件拖放 @ 实现
}
