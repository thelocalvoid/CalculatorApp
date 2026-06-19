



// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <iostream>
#include <string>

// Global variables

// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Calculator to be");

// Stored instance handle for use in Win32 API calls such as FindResource
HINSTANCE hInst;

// Forward declaration of containers
HWND buttonContainer = NULL;
HWND textContainer = NULL;

// Forward declaration of variables
int buttonConOffsetY = NULL;
int buttonConHeight = NULL;

// Stylization Settings
int margin = 20;
int gap = 20;
int textConHeight = 100; 

int buttonHeight = 50;
// Width is 2x the height
int buttonWidth = buttonHeight * 2;
int buttonMarginX = 8;
int buttonMarginY = 8;
int buttonGapX = 4;
int buttonGapY = 4;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    // Store instance handle in our global variable
    hInst = hInstance;

    // The parameters to CreateWindowEx explained:
    // WS_EX_OVERLAPPEDWINDOW : An optional extended window style.
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, height)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        720, 720,
        NULL,
        NULL,
        hInstance,
        NULL
    );


    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindowEx failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindowEx
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = _T("Hello, Windows desktop!");



    switch (message)
    {
    case WM_CREATE:

        buttonContainer = CreateWindowEx(0, TEXT("STATIC"), TEXT(""), WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN, 40, 360, 640, 320, hWnd, NULL, hInst, NULL);
        textContainer = CreateWindowEx(0, TEXT("STATIC"), TEXT(""), WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN, 40, 40, 640, textConHeight, hWnd, NULL, hInst, NULL);

#define CREATE_BTN(text, collumn, row) \
        do { \
            int x = buttonMarginX + (collumn * (buttonGapX + buttonWidth)); \
            int y = buttonMarginY + (row * (buttonGapY + buttonHeight)); \
            CreateWindowEx(0, TEXT("BUTTON"), text, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, x, y, buttonWidth, buttonHeight, buttonContainer, NULL, hInst, NULL); \
        } while(0)


        CREATE_BTN(TEXT("("),  0, 0);
        CREATE_BTN(TEXT(")"),  1, 0);
        CREATE_BTN(TEXT("%"),  2, 0);
        CREATE_BTN(TEXT("CE"), 3, 0);

        CREATE_BTN(TEXT("7"), 0, 1);
        CREATE_BTN(TEXT("8"), 1, 1);
        CREATE_BTN(TEXT("9"), 2, 1);
        CREATE_BTN(TEXT("/"), 3, 1);

        CREATE_BTN(TEXT("4"), 0, 2);
        CREATE_BTN(TEXT("5"), 1, 2);
        CREATE_BTN(TEXT("6"), 2, 2);
        CREATE_BTN(TEXT("x"), 3, 2);

        CREATE_BTN(TEXT("1"), 0, 3);
        CREATE_BTN(TEXT("2"), 1, 3);
        CREATE_BTN(TEXT("3"), 2, 3);
        CREATE_BTN(TEXT("-"), 3, 3);

        CREATE_BTN(TEXT("0"), 0, 4);
        CREATE_BTN(TEXT("."), 1, 4);
        CREATE_BTN(TEXT("="), 2, 4);
        CREATE_BTN(TEXT("+"), 3, 4);

        return 0;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        // Here your application is laid out.
        // 
        // End application-specific layout section.


        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_SIZE:

        RECT rc;
        GetClientRect(hWnd, &rc);

        buttonConOffsetY = textConHeight + margin + gap;
        buttonConHeight = rc.bottom - (textConHeight + (margin * 2) + gap);

        MoveWindow(buttonContainer, margin, buttonConOffsetY, rc.right - (margin * 2), buttonConHeight, TRUE);
        MoveWindow(textContainer,   margin, margin,           rc.right - (margin * 2), textConHeight,                        TRUE);

        break;
    case WM_KEYDOWN:
        TCHAR msg[32];

        wsprintf(msg, _T("Key Pressed: %u\n"), wParam);

        OutputDebugString(msg);
        break;
    case WM_KEYUP:

        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}