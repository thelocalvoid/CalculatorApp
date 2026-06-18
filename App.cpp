


#include <windows.h>
#include <tchar.h>

/// <summary>
/// Info: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-winmain
/// </summary>
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
) {};


/// <summary>
/// Handles window events.
/// 
/// Info here: https://learn.microsoft.com/en-us/windows/win32/winmsg/window-procedures
/// </summary>
/// <param name="hWnd"></param>
/// <param name="message"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
LRESULT CALLBACK WndProc(
	_In_ HWND   hWnd,
	_In_ UINT   message,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {};