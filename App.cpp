



// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <iostream>
#include <string>

// Global variables

#define ID_BUTTON_ZERO    100 // CRITICAL THAT THESE NUMBERS REMAIN 100-109
#define ID_BUTTON_ONE     101 // 
#define ID_BUTTON_TWO     102 // 
#define ID_BUTTON_THREE   103 // 
#define ID_BUTTON_FOUR    104 // 
#define ID_BUTTON_FIVE    105 // 
#define ID_BUTTON_SIX     106 // 
#define ID_BUTTON_SEVEN   107 // 
#define ID_BUTTON_EIGHT   108 // 
#define ID_BUTTON_NINE    109 // CRITICAL THAT THESE NUMBERS REMAIN 100-109

#define ID_BUTTON_ADD     110
#define ID_BUTTON_MINUS   111
#define ID_BUTTON_DIVIDE  112
#define ID_BUTTON_MULTIP  113
#define ID_BUTTON_EQUALS  114
#define ID_BUTTON_CLEARE  115
#define ID_BUTTON_LPEREN  116
#define ID_BUTTON_RPEREN  117
#define ID_BUTTON_PERIOD  118
#define ID_BUTTON_PERCEN  119

#define ID_DIALOG_BOX     120



// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Calculator to be");

// Stored instance handle for use in Win32 API calls such as FindResource
HINSTANCE hInst;

// Forward declaration of containers
HWND buttonContainer = NULL;
HWND textContainer = NULL;
HWND textDisplay = NULL;

// Forward declaration of variables
int buttonConOffsetY = NULL;
int buttonConHeight = NULL;

// Stylization Settings
int margin = 20;
int gap = 20;
int textConHeight = 100;

int nFontSize = 36; // Pixels

int buttonHeight = 50;
// Width is 2x the height
int buttonWidth = buttonHeight * 2;
int buttonMarginX = 8;
int buttonMarginY = 8;
int buttonGapX = 4;
int buttonGapY = 4;

// Calculator Values

bool nextInputShouldClearText = true; // Set true because display initially "0"
// These should be the only sources of truth
double currentInput = NULL;
double previousValue = NULL;
int currentOperator = NULL;

std::string displayText;

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



// Global or class member to store the original procedure
WNDPROC g_pOriginalStaticProc = NULL;

// The new Window Procedure for the Static Control
LRESULT CALLBACK StaticContainerProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
    {
        // Check if the message is from a button inside this container
        int wmId = LOWORD(wParam);

        // Optional: Verify the button belongs to this container if you have multiple
        // if (wmId == IDC_BUTTON_IN_CONTAINER) 
        {
            // Get the handle to the MAIN window (Grandparent)
            // GA_ROOTOWNER gets the top-level window
            HWND hMainWnd = GetAncestor(hWnd, GA_ROOT);

            if (hMainWnd)
            {
                // Forward the message to the main window
                SendMessage(hMainWnd, WM_COMMAND, wParam, lParam);
                return 0; // Message handled
            }
        }
        break;
    }
    }

    // Pass all other messages to the original static control procedure
    return CallWindowProc(g_pOriginalStaticProc, hWnd, uMsg, wParam, lParam);
}


void ClearEverything() {

    SetDlgItemText(textContainer, ID_DIALOG_BOX, L"0");
    previousValue = 0.0;
    currentInput = 0.0;
    currentOperator = 0;
    nextInputShouldClearText = true;
}

void AppendCharToTextBuffer(TCHAR* textBuffer, size_t size, TCHAR charToAppend) {

    size_t textLength = _tcslen(textBuffer);
    
    if (textLength + 1 < size) {
        textBuffer[textLength] = charToAppend;
        textBuffer[textLength + 1] = _T('\0');
    }
    else {
        MessageBox(textContainer, L"Maximum string length!", L"WARNING", MB_OK);
    }
}
// Calculates sum, 
// Sets previousValue to sum
// Sets the dialog text to sum
// Sets currentInput 0.0
// Sets nextInputShouldClearText to true
void ProcessCalculation() {

    double sum;

    switch (currentOperator) {
    case ID_BUTTON_ADD:
        sum = previousValue + currentInput;
        break;
    case ID_BUTTON_MINUS:
        sum = previousValue - currentInput;
        break;
    case ID_BUTTON_DIVIDE:
    {
        sum = previousValue / currentInput;
        break;
    }
    case ID_BUTTON_MULTIP:
        sum = previousValue * currentInput;
        break;
    case ID_BUTTON_PERCEN:
        sum = std::fmod( previousValue, currentInput );
        break;
    }

    TCHAR numkeymsg[256];
    swprintf_s(numkeymsg, 256, L"%.15g", sum);
    SetDlgItemText(textContainer, ID_DIALOG_BOX, numkeymsg);
    OutputDebugString(numkeymsg);

    previousValue = sum;

    nextInputShouldClearText = true;
    currentInput = 0.0;
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
    {
        // Create a new font
        HDC hDC = GetDC(NULL);
        int dpiY = GetDeviceCaps(hDC, LOGPIXELSY);
        ReleaseDC(NULL, hDC);

        int height = -MulDiv(nFontSize, dpiY, 72);

        HFONT font = CreateFont(
            height,
            0,
            0,
            0,
            FW_BOLD,
            FALSE, FALSE, FALSE,
            DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            DEFAULT_PITCH || FF_DONTCARE,
            L"Arial"
        );

        buttonContainer = CreateWindowEx(0, TEXT("STATIC"), TEXT(""), WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN, 40, 360, 640, 320, hWnd, NULL, hInst, NULL);
        textContainer = CreateWindowEx(0, TEXT("STATIC"), TEXT(""), WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN, 40, 40, 640, textConHeight, hWnd, NULL, hInst, NULL);

        textDisplay = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT("0"), WS_VISIBLE | WS_CHILD | ES_READONLY | ES_RIGHT, 0, 0, 640, textConHeight, textContainer, (HMENU)ID_DIALOG_BOX, hInst, NULL);

        // Set Font of textDisplay
        SendMessage(textDisplay, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));



#define CREATE_BTN(id, text, collumn, row) \
        do { \
            int x = buttonMarginX + (collumn * (buttonGapX + buttonWidth)); \
            int y = buttonMarginY + (row * (buttonGapY + buttonHeight)); \
            CreateWindowEx(0, TEXT("BUTTON"), text, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, x, y, buttonWidth, buttonHeight, buttonContainer, (HMENU)id, hInst, NULL); \
        } while(0)



        //CREATE_BTN(ID_BUTTON_LPEREN, TEXT("("), 0, 0);
        //CREATE_BTN(ID_BUTTON_RPEREN, TEXT(")"), 1, 0);
        CREATE_BTN(ID_BUTTON_PERCEN, TEXT("%"), 2, 0);
        CREATE_BTN(ID_BUTTON_CLEARE, TEXT("CE"), 3, 0);

        CREATE_BTN(ID_BUTTON_SEVEN, TEXT("7"), 0, 1);
        CREATE_BTN(ID_BUTTON_EIGHT, TEXT("8"), 1, 1);
        CREATE_BTN(ID_BUTTON_NINE, TEXT("9"), 2, 1);
        CREATE_BTN(ID_BUTTON_DIVIDE, TEXT("/"), 3, 1);

        CREATE_BTN(ID_BUTTON_FOUR, TEXT("4"), 0, 2);
        CREATE_BTN(ID_BUTTON_FIVE, TEXT("5"), 1, 2);
        CREATE_BTN(ID_BUTTON_SIX, TEXT("6"), 2, 2);
        CREATE_BTN(ID_BUTTON_MULTIP, TEXT("x"), 3, 2);

        CREATE_BTN(ID_BUTTON_ONE, TEXT("1"), 0, 3);
        CREATE_BTN(ID_BUTTON_TWO, TEXT("2"), 1, 3);
        CREATE_BTN(ID_BUTTON_THREE, TEXT("3"), 2, 3);
        CREATE_BTN(ID_BUTTON_MINUS, TEXT("-"), 3, 3);

        CREATE_BTN(ID_BUTTON_ZERO, TEXT("0"), 0, 4);
        CREATE_BTN(ID_BUTTON_PERIOD, TEXT("."), 1, 4);
        CREATE_BTN(ID_BUTTON_EQUALS, TEXT("="), 2, 4);
        CREATE_BTN(ID_BUTTON_ADD, TEXT("+"), 3, 4);

        // SUBCLASS THE STATIC CONTROL
        // Replace its window procedure with our custom one
        g_pOriginalStaticProc = (WNDPROC)SetWindowLongPtr(
            buttonContainer,
            GWLP_WNDPROC,
            (LONG_PTR)StaticContainerProc
        );

        return 0;
    }
    case WM_COMMAND:
    {

        int id = LOWORD(wParam);

        // Operator keys [+-*/]
        if (id >= ID_BUTTON_ADD && id <= ID_BUTTON_MULTIP || id == ID_BUTTON_PERCEN) {
            // Operator pressed
            if (currentOperator != NULL) {
                if (currentOperator >= ID_BUTTON_ADD && currentOperator <= ID_BUTTON_MULTIP) {

                    ProcessCalculation();
                }
            }
            else {
                previousValue = currentInput;
            }

            currentOperator = id;
            currentInput = 0.0;

            nextInputShouldClearText = true;

        }
        // Number buttons [0-9]
        else if (id >= ID_BUTTON_ZERO && id <= ID_BUTTON_NINE) {

            if (nextInputShouldClearText) {
                SetDlgItemText(textContainer, ID_DIALOG_BOX, L"");
                nextInputShouldClearText = false;
            }

            //append

            TCHAR textBuffer[256];
            int numKeyPressed = LOWORD(wParam) - 100; // this works because our key ids are 100-109 for number keys, probably not smart

            // Get text from dialog box
            GetDlgItemText(textContainer, ID_DIALOG_BOX, textBuffer, 256);

            AppendCharToTextBuffer(textBuffer, _countof(textBuffer), (TCHAR)(48 + numKeyPressed)); // Add 48 for Ascii

            // Update dialog box
            SetDlgItemText(textContainer, ID_DIALOG_BOX, textBuffer);

            TCHAR* endPtr;
            currentInput = _tcstod(textBuffer, &endPtr);


            TCHAR numkeymsg[32];
            wsprintf(numkeymsg, _T("Key Pressed: %d\n"), LOWORD(wParam) - 100);
            //MessageBox(hWnd, numkeymsg, L"COMMAND", MB_OK);
            OutputDebugString(numkeymsg);
        }
        // Period button
        else if (id == ID_BUTTON_PERIOD) {

            TCHAR textBuffer[256];

            // Get text from dialog box
            GetDlgItemText(textContainer, ID_DIALOG_BOX, textBuffer, 256);

            AppendCharToTextBuffer(textBuffer, _countof(textBuffer), 46); // Add 48 for Ascii

            // Update dialog box
            SetDlgItemText(textContainer, ID_DIALOG_BOX, textBuffer);

            TCHAR* endPtr;
            currentInput = _tcstod(textBuffer, &endPtr);
        }
        // Clear button
        else if (id == ID_BUTTON_CLEARE) {
            ClearEverything();
        }
        // Equals button
        else if (id == ID_BUTTON_EQUALS) {
            // Calculate sum

            ProcessCalculation();
            currentOperator = NULL;

        }

        break;
    }
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
        MoveWindow(textContainer, margin, margin, rc.right - (margin * 2), textConHeight, TRUE);


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