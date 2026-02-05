#include "MenuPopUp.h"
#include "helpers.h"

BOOL isMenuPopUpClassinit = FALSE;
LRESULT CALLBACK fnMenuPopUp(HWND, UINT, WPARAM, LPARAM);
extern HWND hCurrentPopup;

HWND fnCreateMenuPopUp(POINT p, int WidthFactor, PopupMenuItemStructArray* PMISA)
{
    TCHAR szClassName[] = _TEXT("Window Popup Menu");
    WNDCLASS wc = {0};

    int totalItems = getPMISAItemCount(PMISA);
    int finalHeight = totalItems * CY_MENU_ITEM + 8;

    wc.lpfnWndProc   = fnMenuPopUp;
    wc.hInstance     = GetModuleHandle(NULL);
    wc.lpszClassName = szClassName;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);

    if (!isMenuPopUpClassinit)
    {
        RegisterClass(&wc);
        isMenuPopUpClassinit = TRUE;
    }

    HWND hWnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_NOACTIVATE,
        szClassName,
        NULL,
        WS_POPUP,
        p.x,
        p.y,
        WidthFactor,
        finalHeight,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)PMISA);

    HRGN hRgn = CreateRoundRectRgn(0, 0, WidthFactor + 1, finalHeight + 1, 10, 10);
    SetWindowRgn(hWnd, hRgn, TRUE);

    SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);
    ShowWindow(hWnd, SW_SHOWNOACTIVATE);

    return hWnd;
}

LRESULT CALLBACK fnMenuPopUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int hoveredIndex = -1;
    PopupMenuItemStructArray* pmisa = (PopupMenuItemStructArray*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    switch (msg)
    {
        case WM_MOUSEMOVE:
        {
            int y = GET_Y_LPARAM(lParam);
            int newHover = -1;
            for (int i = 0; i < pmisa->count; i++)
            {
                int top = 4 + i * CY_MENU_ITEM;
                int bottom = top + CY_MENU_ITEM;
                if (y >= top && y < bottom)
                {
                    newHover = i;
                    break;
                }
            }
            if (newHover != hoveredIndex)
            {
                hoveredIndex = newHover;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            return 0;
        }

        case WM_LBUTTONDOWN:
        {
            if (hoveredIndex >= 0 && hoveredIndex < pmisa->count)
            {
                if (pmisa->array[hoveredIndex]->OnClick)
                    pmisa->array[hoveredIndex]->OnClick(pmisa->array[hoveredIndex]->szTitle);

                ShowWindow(hWnd, SW_HIDE);
                hCurrentPopup = NULL;
            }
            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            RECT rc;
            GetClientRect(hWnd, &rc);

            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBmp = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
            HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, memBmp);

            HBRUSH bgBrush = CreateSolidBrush(RGB(248, 248, 248));
            FillRect(memDC, &rc, bgBrush);
            DeleteObject(bgBrush);

            HFONT hFont = CreateFont(
                -16, 0, 0, 0, FW_MEDIUM,
                FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                DEFAULT_PITCH | FF_SWISS,
                _TEXT("Segoe UI")
            );
            HFONT oldFont = (HFONT)SelectObject(memDC, hFont);
            SetBkMode(memDC, TRANSPARENT);
            SetTextColor(memDC, RGB(40, 40, 40));

            int y = 4;
            for (int i = 0; i < pmisa->count; i++)
            {
                RECT itemRc = { 4, y, rc.right - 4, y + CY_MENU_ITEM };
                COLORREF bg = (i == hoveredIndex) ? pmisa->array[i]->ColScheme.Active
                                                  : pmisa->array[i]->ColScheme.Passive;
                HBRUSH br = CreateSolidBrush(bg);
                FillRect(memDC, &itemRc, br);
                DeleteObject(br);

                DrawTextA(memDC, pmisa->array[i]->szTitle, -1, &itemRc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                y += CY_MENU_ITEM;
            }

            HPEN hPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
            HPEN oldPen = (HPEN)SelectObject(memDC, hPen);
            SelectObject(memDC, GetStockObject(NULL_BRUSH));
            RoundRect(memDC, 0, 0, rc.right, rc.bottom, 10, 10);
            SelectObject(memDC, oldPen);
            DeleteObject(hPen);

            BitBlt(hdc, 0, 0, rc.right, rc.bottom, memDC, 0, 0, SRCCOPY);

            SelectObject(memDC, oldFont);
            DeleteObject(hFont);
            SelectObject(memDC, oldBmp);
            DeleteObject(memBmp);
            DeleteDC(memDC);

            EndPaint(hWnd, &ps);
            return 0;
        }

        case WM_KILLFOCUS:
        case WM_ACTIVATE:
            if (LOWORD(wParam) == WA_INACTIVE)
            {
                ShowWindow(hWnd, SW_HIDE);
                hCurrentPopup = NULL;
            }
            return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}
