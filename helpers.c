// helpers.c by Amash Shafi Jami
#include "helpers.h"

void MenuItemAssignTitle(MenuItemStruct* mis, LPCSTR szTitle){
    StringCchPrintf(mis->szTitle,MENU_ITEM_CHAR_SIZE,_TEXT("%s"),szTitle);
}

void PopUpMenuItemAssignTitle(PopupMenuItemStruct* pmis, LPCSTR szTitle){
    StringCchPrintf(pmis->szTitle,MENU_ITEM_CHAR_SIZE,_TEXT("%s"),szTitle);
}

void fndefPMISClickCallback(LPCSTR title){
    printf("Menu Item %s Clicked!");
}

int getPMISAItemCount(PopupMenuItemStructArray* PMISA){
    return PMISA->count;
}

PopupMenuItemStruct* GetBasePMIS(void){
    PopupMenuItemStruct* basePMIS   = (PopupMenuItemStruct*)malloc(sizeof(PopupMenuItemStruct));
    basePMIS->ColScheme.Active      = RGB(220,220,220);
    basePMIS->ColScheme.Border      = basePMIS->ColScheme.Active;
    basePMIS->ColScheme.Passive     = RGB(255,255,255);
    basePMIS->OnClick               = fndefPMISClickCallback;
    PopUpMenuItemAssignTitle(basePMIS,"Popup Menu Item");
    return basePMIS;
}

PopupMenuItemStructArray* getBasePMISA(void)
{
    PopupMenuItemStructArray* pmisa =
        (PopupMenuItemStructArray*)calloc(1, sizeof(PopupMenuItemStructArray));
    pmisa->count = 0;
    return pmisa;
}

void PMISA_AddEx(
    PopupMenuItemStructArray* pmisa,
    LPCSTR title,
    OnClickCallback cb,
    ColorSchemeStruct scheme
){
    if (pmisa->count >= MAX_POPUP_ITEMS) return;

    PopupMenuItemStruct* item =
        (PopupMenuItemStruct*)malloc(sizeof(PopupMenuItemStruct));

    PopUpMenuItemAssignTitle(item, title);
    item->OnClick   = cb;
    item->ColScheme = scheme;

    pmisa->array[pmisa->count++] = item;
}

MenuItemStruct* GetBaseMIS(void)
{
    MenuItemStruct* baseMIS         = (MenuItemStruct*)malloc(sizeof(MenuItemStruct));
    baseMIS->cxOffset               = 0;
    baseMIS->isHovered              = FALSE;
    baseMIS->isRightMost            = FALSE;
    baseMIS->isStrLong              = FALSE;
    baseMIS->ColScheme.BorderWidth  = 1;
    baseMIS->cxTextPadding          = 5;
    baseMIS->ColScheme.Active       = RGB(220,220,220);
    baseMIS->ColScheme.Border       = baseMIS->ColScheme.Active;
    baseMIS->ColScheme.Passive      = RGB(255,255,255);
    baseMIS->hPopUp                 = NULL;
    baseMIS->WidthFactor            = 100;
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
