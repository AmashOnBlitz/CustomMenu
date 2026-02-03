// CustomMenu.h by Amash Shafi Jami
#pragma once 
#include "module.h"
#include "helpers.h"

void fnCreateMenuPopUp();

HWND fnCreateMenuItem(
    HWND parent, 
    MenuItemStruct* mis, 
    LPCSTR szMenuItemClass, 
    LPCSTR title,
    int WidthFactor
);

MenuItemStruct* fnMenuWinProcGetMIS(HWND hWnd);

LRESULT CALLBACK fnMenuWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void fnCreateMenu(HWND parent);