// Custom Menu Program by Amash Shafi Jami
// main.c 
#include "CustomMenu.h"

LRESULT CALLBACK fnWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow){
    TCHAR szAppName[] = _TEXT("Custom Menu Program 1");
    HWND hWnd;
    MSG msg;
    WNDCLASS wndClass;

    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
    wndClass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
    wndClass.hInstance = hInstance;
    wndClass.lpfnWndProc = fnWinProc;
    wndClass.lpszClassName = szAppName;
    wndClass.lpszMenuName = NULL;
    wndClass.style = CS_VREDRAW | CS_HREDRAW;

    if (!RegisterClass(&wndClass)){
        ERR_CHAR_RESERVE;
        ERR_CHAR_FILL_1_VALUE(
            "Cannot Register Window Class Of name \"%s\"",
            wndClass.lpszClassName
        );
        MessageBox(
            NULL,
            ERR_CHAR,
            _TEXT("ERROR : Cant Register Window Class"),
            MB_ICONERROR | MB_APPLMODAL
        );
        return 0;
    }

    hWnd = CreateWindow(
        szAppName,
        szAppName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hWnd == NULL){
        ERR_CHAR_RESERVE;
        ERR_CHAR_FILL_1_VALUE(
            "Cannot Create Window Of Window Name \"%s\"",
            szAppName
        )
        MessageBox(NULL,ERR_CHAR,_TEXT("ERROR : Window Creation Failed"),MB_ICONERROR | MB_APPLMODAL);
        return 0;
    }

    ShowWindow(hWnd,iCmdShow);
    UpdateWindow(hWnd);

    while(GetMessage(&msg, NULL, 0, 0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 1;
}

LRESULT CALLBACK fnWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
    static int cxClient, cyClient;
    HDC hdc;
    PAINTSTRUCT ps;
    switch (msg)
    {
    case WM_CREATE:{
        fnCreateMenu(hWnd);
        return 0;
    }
    case WM_SIZE:
    cxClient = GET_X_LPARAM(lParam);
    cyClient = GET_Y_LPARAM(lParam);
    return 0;
    case WM_PAINT:{
        hdc = BeginPaint(hWnd,&ps);
        const int iBuff = 256;
        const int iPaddingCoord = 5;
        TCHAR szBuff[iBuff];
        StringCchPrintf(
            szBuff,
            iBuff,
            _TEXT("Window Size (x : y) : %i : %i"),
            cxClient,
            cyClient
        );
        int cyPadding = iPaddingCoord + CY_MENU_ITEM;
        int cxPadding = iPaddingCoord;
        TextOut(hdc,cxPadding,cyPadding,szBuff,lstrlen(szBuff));
        EndPaint(hWnd,&ps);
        return 0;
    }
    case WM_DESTROY:
    PostQuitMessage(0);
    return 0;

    default:
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}