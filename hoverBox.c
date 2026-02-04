#include "hoverBox.h"
#include <windows.h>

BOOL isHoverBoxClassInit = FALSE; 

LRESULT CALLBACK fnWinProcHoverBox(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND fnCreateHoverBox(HWND parent, int x, int y, LPCSTR string)
{
    TCHAR szHoverBoxClass[] = _TEXT("Hover Hint Box");
    HWND hWnd;
    WNDCLASS wndClass;

    if (!isHoverBoxClassInit) {
        wndClass.cbClsExtra = 0;
        wndClass.cbWndExtra = sizeof(LPCSTR);
        wndClass.hbrBackground = NULL;
        wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        wndClass.hIcon = NULL;
        wndClass.hInstance = GetModuleHandle(NULL);
        wndClass.lpfnWndProc = fnWinProcHoverBox;
        wndClass.lpszClassName = szHoverBoxClass;
        wndClass.lpszMenuName = NULL;
        wndClass.style = CS_HREDRAW | CS_VREDRAW;

        if (!RegisterClass(&wndClass)) {
            MessageBox(parent, _TEXT("Cannot register hover box class!"), _TEXT("ERROR"), MB_ICONERROR);
            return NULL;
        }
        isHoverBoxClassInit = TRUE;
    }

    HDC hdc = GetDC(NULL);
    HFONT hFont = CreateFontA(
        -16,
        0,
        0,
        0,
        FW_MEDIUM,
        FALSE,
        FALSE,
        FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH|FF_SWISS,
        "Segoe UI"
    );
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
    SIZE sz;
    GetTextExtentPoint32A(hdc, string, lstrlenA(string), &sz);
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
    ReleaseDC(NULL, hdc);

    int paddingX = 12;
    int paddingY = 6;
    int width = sz.cx + 2 * paddingX;
    int height = sz.cy + 2 * paddingY;

    hWnd = CreateWindowEx(
        WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_NOACTIVATE,
        szHoverBoxClass,
        NULL,
        WS_POPUP,
        x, y,
        width, height,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        (LPVOID)string
    );

    HRGN hRgn = CreateRoundRectRgn(0, 0, width, height, 12, 12);
    SetWindowRgn(hWnd, hRgn, TRUE);

    ShowWindow(hWnd, SW_SHOWNOACTIVATE);
    UpdateWindow(hWnd);

    return hWnd;
}

LRESULT CALLBACK fnWinProcHoverBox(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_CREATE: {
        CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
        LPCSTR text = (LPCSTR)cs->lpCreateParams;
        SetWindowLongPtr(hWnd, 0, (LONG_PTR)text);
        return 0;
    }

    case WM_PAINT: {
        LPCSTR text = (LPCSTR)GetWindowLongPtr(hWnd, 0);
        if (!text) break;

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        RECT rc;
        GetClientRect(hWnd, &rc);
        HBRUSH br = CreateSolidBrush(RGB(20,20,20));
        FillRect(hdc, &rc, br);
        DeleteObject(br);

        HFONT hFont = CreateFontA(
            -16,
            0,
            0,
            0,
            FW_MEDIUM,
            FALSE,
            FALSE,
            FALSE,
            DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY,
            DEFAULT_PITCH|FF_SWISS,
            "Segoe UI"
        );
        HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(255,255,255));

        int paddingX = 12;
        int paddingY = 6;
        RECT textRect = rc;
        textRect.left += paddingX;
        textRect.right -= paddingX;
        textRect.top += paddingY;
        textRect.bottom -= paddingY;

        DrawTextA(hdc, text, -1, &textRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

        SelectObject(hdc, hOldFont);
        DeleteObject(hFont);

        EndPaint(hWnd, &ps);
        return 0;
    }

    default:
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
