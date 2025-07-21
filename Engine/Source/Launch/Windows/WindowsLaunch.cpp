#include "pch.h"

#include "Core/Core.h"
#include "Application/Application.h"

void SetWindowsEnvironment()
{

}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{ 
    
    SetWindowsEnvironment();
    int32 error = 0;
    GWindowsInstance = hInstance;
    Application app;
    error = app.Run();
    return 0;
}