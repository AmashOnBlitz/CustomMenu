// CustomMenu.c by Amash Shafi Jami
#include "CustomMenu.h"
#include "hoverBox.h"


HWND fnCreateMenuItem(
    HWND parent,
    MenuItemStruct* mis,
    LPCSTR szMenuItemClass,
    LPCSTR title,
    int WidthFactor
){
    HWND hMenuItem = CreateWindow(
            szMenuItemClass,
            title,
            WS_CHILD | WS_VISIBLE,
            mis->cxOffset,
            0,
            WidthFactor,
            CY_MENU_ITEM,
            parent,
            NULL,
            GetModuleHandle(NULL),
            mis
        );
    return hMenuItem;
}

int fnGetHoverBoxWidth(HDC hdc, MenuItemStruct* mis){
    SIZE sz;
    int len = lstrlenA(mis->szTitle);
    if (len == 0) return 0;
    GetTextExtentPoint32A(hdc, mis->szTitle, len, &sz);
    return sz.cx + 10; 
}


MenuItemStruct* fnMenuWinProcGetMIS(HWND hWnd)
{
    MenuItemStruct* mis =
        (MenuItemStruct*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    if (!mis)
        return GetBaseMIS(); 

    return mis;
}

LRESULT CALLBACK fnMenuWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
    MenuItemStruct* mis;
    PAINTSTRUCT ps;
    TEXTMETRIC tm;
    HDC hdc;
    static int cxHBox;
    static HFONT hFont = NULL;
    static int cxClient, cyClient;
    static int cxChar, CxCaps, cyChar;
    switch (msg)
    {
        case WM_CREATE:{
        CREATESTRUCT *cs = (CREATESTRUCT*)(lParam);
        mis = (MenuItemStruct*)cs->lpCreateParams;
        SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG_PTR)mis);
        hdc = GetDC(hWnd);
        GetTextMetrics(hdc,&tm);
        cxChar = tm.tmAveCharWidth;
        CxCaps = (tm.tmPitchAndFamily & 1? 3 : 2)* cxChar /2;
        cyChar = tm.tmHeight + tm.tmExternalLeading;
            if (!hFont) {
            hFont = CreateFont(
                -16,              
                0, 0, 0,
                FW_MEDIUM,      
                FALSE, FALSE, FALSE,
                DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, 
                DEFAULT_PITCH | FF_SWISS,
                TEXT("Segoe UI")
            );
            }
        ReleaseDC(hWnd,hdc);
        return 0;
        }
        case WM_SIZE:{
            cxClient = GET_X_LPARAM(lParam);
            cyClient = GET_Y_LPARAM(lParam);
            mis = fnMenuWinProcGetMIS(hWnd);
            SetWindowPos(hWnd,NULL,mis->cxOffset,0,cxClient,cyClient,0);
            return 1;
        }
        case WM_PAINT: {
            mis = fnMenuWinProcGetMIS(hWnd);
            hdc = BeginPaint(hWnd, &ps);
            HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(40, 40, 40));

            RECT rc;
            GetClientRect(hWnd, &rc);
            int availableWidth = rc.right - rc.left - (2*mis->cxTextPadding);
            SIZE sz;
            int len = lstrlenA(mis->szTitle);

            int charsThatFit = 0;
            for (int i = 1; i <= len; i++)
            {
                GetTextExtentPoint32A(
                    hdc,
                    mis->szTitle,
                    i,
                    &sz
                );

                if (sz.cx > availableWidth)
                    break;

                charsThatFit = i;
            }
            if(fnIsPsuedoStringRequired(charsThatFit,mis->szTitle)){
                mis->isStrLong = TRUE;
                cxHBox = fnGetHoverBoxWidth(hdc,mis);
            }else{
                mis->isStrLong = FALSE;
            }

            DrawText(
                hdc,
                fnGetPseudoString(charsThatFit,mis->szTitle),
                -1,
                &rc,
                DT_CENTER | DT_VCENTER | DT_SINGLELINE
            );
            SelectObject(hdc, hOldFont);

            HPEN hPen = CreatePen(
            PS_SOLID,
            mis->ColScheme.BorderWidth,
            mis->ColScheme.Border
            );

            HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
            MoveToEx(hdc, 0, cyClient - 1, NULL);
            LineTo(hdc, cxClient, cyClient - 1);
            if (mis->isRightMost == TRUE){
                MoveToEx(
                    hdc,
                    cxClient-mis->ColScheme.BorderWidth,
                    cyClient,
                    NULL
                );
                LineTo(
                    hdc,
                    cxClient-mis->ColScheme.BorderWidth,
                    0
                );
            }
            SelectObject(hdc, hOldPen);
            DeleteObject(hPen);
            EndPaint(hWnd, &ps);
            return 0;
        }
        
        case WM_MOUSEMOVE:
        {
            mis = fnMenuWinProcGetMIS(hWnd);
            POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            
            ClientToScreen(hWnd, &pt);
            if (!mis->isHovered) {
                mis->isHovered = TRUE;
                InvalidateRect(hWnd, NULL, TRUE);

                if (mis->isStrLong) {
                    mis->hHoverBox = fnCreateHoverBox(
                        NULL,
                        pt.x,
                        pt.y + CY_MENU_ITEM,
                        mis->szTitle
                    );
                }
                TRACKMOUSEEVENT tme = { sizeof(tme) };
                tme.dwFlags = TME_LEAVE;
                tme.hwndTrack = hWnd;
                TrackMouseEvent(&tme);
            }
            if (mis->hHoverBox) {
                SetWindowPos(
                    mis->hHoverBox,
                    HWND_TOPMOST,
                    pt.x,
                    pt.y + CY_MENU_ITEM,
                    0, 0,
                    SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW
                );
            }
            return 0;
        }
        return 0;
        case WM_MOUSELEAVE:
        mis = fnMenuWinProcGetMIS(hWnd);
        if(mis->isHovered != FALSE){
            mis->isHovered = FALSE;
            if (mis->hHoverBox) {
                DestroyWindow(mis->hHoverBox);
                mis->hHoverBox = NULL;
            }
            InvalidateRect(hWnd,NULL,TRUE);
        }
        return 0;
        case WM_ERASEBKGND:
        mis = fnMenuWinProcGetMIS(hWnd);
        hdc = (HDC)(wParam);
        RECT rect;
        GetClientRect(hWnd,&rect);
        COLORREF cRef = (mis->isHovered == TRUE) ? (mis->ColScheme.Active) : (mis->ColScheme.Passive); 
        HBRUSH br = (HBRUSH)CreateSolidBrush(cRef);
        FillRect(hdc,&rect,br);
        DeleteObject(br);
        return 1;
    default:
        break;
    }
    return DefWindowProc(hWnd,msg,wParam,lParam);
}

void fnCreateMenu(HWND parent){
    int WidthFactor = 100;
    TCHAR szMenuItemClass[] = _TEXT("Menu Item About");
    WNDCLASS wndClassMenu;
    wndClassMenu.cbClsExtra = 0;
    wndClassMenu.cbWndExtra = 0;
    // wndClassMenu.hbrBackground = (HBRUSH) GetStockObject(GRAY_BRUSH);
    wndClassMenu.hbrBackground = (HBRUSH) CreateSolidBrush(RGB(255,255,255));
    // wndClassMenu.hbrBackground = (HBRUSH) CreateSolidBrush(RGB(220,220,220));
    wndClassMenu.hCursor = LoadCursor(NULL,IDC_ARROW);
    wndClassMenu.hIcon = NULL;
    wndClassMenu.hInstance = GetModuleHandle(NULL);
    wndClassMenu.lpfnWndProc = fnMenuWinProc;
    wndClassMenu.lpszClassName = szMenuItemClass;
    wndClassMenu.lpszMenuName = NULL;
    wndClassMenu.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClass(&wndClassMenu)){
        ERR_CHAR_RESERVE;
        ERR_CHAR_FILL_1_VALUE(
            "Cannot Register Menu Class Of name \"%s\"",
            wndClassMenu.lpszClassName
        );
        MessageBox(
            NULL,
            ERR_CHAR,
            _TEXT("ERROR : Cant Register Window Class"),
            MB_ICONERROR | MB_APPLMODAL
        );
        return;
    }

    LPCTSTR arrMenuItemNames[COUNTMENUITEMS] = {
        _TEXT("About"),
        _TEXT("File"),
        _TEXT("Edit"),
        _TEXT("Help"),
        _TEXT("Info"),
        _TEXT("Testing Long Long Texts"),
        _TEXT("STRThisIsAnotherLongLineOfTextHeheHeHeHeEND"),
    };
    HWND MENUITEMS[COUNTMENUITEMS];
    for(int i = 0; i < COUNTMENUITEMS; i++){
        MenuItemStruct* mis = GetBaseMIS();
        mis->ColScheme.Active = RGB(220,20,20);
        mis->ColScheme.Border = RGB(220,20,220);
        if (i == (COUNTMENUITEMS-1)) mis->isRightMost = TRUE;
        MenuItemAssignTitle(mis,arrMenuItemNames[i]);
        mis->cxOffset = i * WidthFactor;
        mis->isHovered = FALSE;
        MENUITEMS[i] = fnCreateMenuItem(parent,mis,szMenuItemClass,"About",WidthFactor);
    }
}

