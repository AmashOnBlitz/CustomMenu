// Module.h by Amash Shafi Jami
#pragma once 

#include <Windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <strsafe.h>
#include <ctype.h>

#define COUNTMENUITEMS 7
#define CY_MENU_ITEM 18
#define MENU_ITEM_CHAR_SIZE 256
#define POPUP_MENU_ITEM_CHAR_SIZE MENU_ITEM_CHAR_SIZE
#define ERR_CHAR szErrBuff
#define ERR_CHAR_SIZE 256
#define ERR_CHAR_RESERVE static TCHAR ERR_CHAR[ERR_CHAR_SIZE];
#define ERR_CHAR_FILL_1_VALUE(format,y) StringCchPrintf( \
                                        ERR_CHAR,\
                                        ERR_CHAR_SIZE,\
                                        _TEXT(format) \
                                        ,y \
                                        );
typedef void (*OnClickCallback)(LPCSTR title);

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
    // INTERNALLY - HANDLED
    HWND hHoverBox;
    // INTERNALLY - HANDLED
    HWND hPopUp;
} MenuItemStruct;

typedef struct PopupMenuItemStuct{
    TCHAR szTitle[POPUP_MENU_ITEM_CHAR_SIZE];
    ColorSchemeStruct ColScheme;
    OnClickCallback OnClick;
} PopupMenuItemStruct;