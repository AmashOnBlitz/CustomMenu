// helpers.h by Amash Shafi Jami
#pragma once 
#include "module.h"

void MenuItemAssignTitle(MenuItemStruct* mis, LPCSTR szTitle);
void PopUpMenuItemAssignTitle(PopupMenuItemStruct* pmis, LPCSTR szTitle);
MenuItemStruct* GetBaseMIS(void);
PopupMenuItemStruct* GetBasePMIS(void);
PopupMenuItemStuctArray* getBasePMISA(void);
BOOL fnIsPsuedoStringRequired(int maxChars, LPCSTR src);
LPCSTR fnGetPseudoString(int maxChars, LPCSTR src);
int fnGetCountCaps(LPCSTR string);
int fnGetCountAve(LPCSTR string);