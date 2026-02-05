// helpers.h by Amash Shafi Jami
#pragma once 
#include "module.h"

void MenuItemAssignTitle(MenuItemStruct* mis, LPCSTR szTitle);
void PopUpMenuItemAssignTitle(PopupMenuItemStruct* pmis, LPCSTR szTitle);
int getPMISAItemCount(PopupMenuItemStructArray* PMISA);
MenuItemStruct* GetBaseMIS(void);
PopupMenuItemStruct* GetBasePMIS(void);
PopupMenuItemStructArray* getBasePMISA(void);
void PMISA_AddEx(PopupMenuItemStructArray* pmisa, LPCSTR title, OnClickCallback cb, ColorSchemeStruct scheme);
BOOL fnIsPsuedoStringRequired(int maxChars, LPCSTR src);
LPCSTR fnGetPseudoString(int maxChars, LPCSTR src);
int fnGetCountCaps(LPCSTR string);
int fnGetCountAve(LPCSTR string);