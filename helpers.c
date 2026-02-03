// helpers.c by Amash Shafi Jami
#include "helpers.h"

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
