#include <Windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <strsafe.h>
#include <ctype.h>

#define COUNTMENUITEMS 7
#define CY_MENU_ITEM 18
#define MENU_ITEM_CHAR_SIZE 256
#define ERR_CHAR szErrBuff
#define ERR_CHAR_SIZE 256
#define ERR_CHAR_RESERVE static TCHAR ERR_CHAR[ERR_CHAR_SIZE];
#define ERR_CHAR_FILL_1_VALUE(format,y) StringCchPrintf( \
                                        ERR_CHAR,\
                                        ERR_CHAR_SIZE,\
                                        _TEXT(format) \
                                        ,y \
                                        );

typedef struct ColorSchemeStruct{
    COLORREF Active;
    COLORREF Passive;
    COLORREF Border;
    int BorderWidth;
} ColorSchemeStruct;

typedef struct MenuItemStruct {
    TCHAR szTitle[MENU_ITEM_CHAR_SIZE];
    int cxOffset;
    int cxTextPadding;
    BOOL isHovered;
    BOOL isRightMost;
    ColorSchemeStruct ColScheme;

    // INTERNALLY - HANDLED
    BOOL isStrLong;
} MenuItemStruct;

void MenuItemAssignTitle(MenuItemStruct* mis, LPCSTR szTitle){
    StringCchPrintf(mis->szTitle,MENU_ITEM_CHAR_SIZE,_TEXT("%s"),szTitle);
}
MenuItemStruct* GetBaseMIS(void)
{
    MenuItemStruct* baseMIS = (MenuItemStruct*)malloc(sizeof(MenuItemStruct));
    baseMIS->cxOffset               = 0;
    baseMIS->isHovered              = FALSE;
    baseMIS->isRightMost            = FALSE;
    baseMIS->isStrLong              = FALSE;
    baseMIS->ColScheme.BorderWidth  = 1;
    baseMIS->cxTextPadding          = 5;
    baseMIS->ColScheme.Active       = RGB(220,220,220);
    baseMIS->ColScheme.Border       = baseMIS->ColScheme.Active;
    baseMIS->ColScheme.Passive      = RGB(255,255,255);
    MenuItemAssignTitle(baseMIS,"Menu Item");
    return baseMIS;
}

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

void fnCreateMenuPopUp(){

}

HWND fnCreateMenuItem(HWND parent, MenuItemStruct* mis, LPCSTR szMenuItemClass, LPCSTR title, int WidthFactor){
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

MenuItemStruct* fnMenuWinProcGetMIS(HWND hWnd)
{
    MenuItemStruct* mis =
        (MenuItemStruct*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    if (!mis)
        return GetBaseMIS(); 

    return mis;
}

BOOL fnIsPsuedoStringRequired(int maxChars, LPCSTR src){
    int len = lstrlenA(src);
    if (len <= maxChars) return FALSE;
    return TRUE;
}

LPCSTR fnGetPseudoString(int maxChars, LPCSTR src)
{
    static char out[256];
    int len = lstrlenA(src);

    if (len <= maxChars)
        return src;

    int left  = (maxChars - 3) / 2;
    int right = (maxChars - 3) - left;

    CopyMemory(out, src, left);

    out[left] = '.';
    out[left+1] = '.';
    out[left+2] = '.';

    CopyMemory(out + left + 3, src + len - right, right);

    out[maxChars] = '\0';
    return out;
}

int fnGetCountCaps(LPCSTR string){
    int iCaps = 0;
    int iLen = lstrlenA(string);
    for (int i = 0; i < iLen; i++)
    {
        if (isupper((unsigned char)string[i]))
            iCaps++;
    }
    return iCaps;
}
int fnGetCountAve(LPCSTR string){
    int iAve = 0;
    int iLen = lstrlenA(string);
    for (int i = 0; i < iLen; i++)
    {
        if (!isupper((unsigned char)string[i]))
            iAve++;
    }
    return iAve;
}



LRESULT CALLBACK fnMenuWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
    MenuItemStruct* mis;
    PAINTSTRUCT ps;
    TEXTMETRIC tm;
    HDC hdc;
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
        mis = fnMenuWinProcGetMIS(hWnd);
        if(mis->isHovered != TRUE){
            mis->isHovered = TRUE;
            InvalidateRect(hWnd,NULL,TRUE);
            TRACKMOUSEEVENT tme = {0};
            tme.cbSize = sizeof(TRACKMOUSEEVENT);
            tme.dwFlags = TME_LEAVE;
            tme.dwHoverTime = HOVER_DEFAULT;
            tme.hwndTrack = hWnd;
            TrackMouseEvent(&tme);
        }
        return 0;
        case WM_MOUSELEAVE:
        mis = fnMenuWinProcGetMIS(hWnd);
        if(mis->isHovered != FALSE){
            mis->isHovered = FALSE;
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