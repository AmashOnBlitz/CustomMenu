#include "hoverBox.h"

BOOL isHoverBoxClassInit = FALSE; 

LRESULT CALLBACK fnWinProcHoverBox(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND fnCreateHoverBox(HWND parent, int x, int y, LPCSTR string, int Width, int Height)
{
    TCHAR szHoverBoxClass[] = _TEXT("Hover Hint Box");
    WNDCLASS wndClass;
    if (isHoverBoxClassInit == FALSE){
        wndClass.cbClsExtra = 0;
        wndClass.cbWndExtra = 0;
        wndClass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
        wndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
        wndClass.hIcon = NULL;
        wndClass.hInstance = GetModuleHandle(NULL);
        wndClass.lpfnWndProc = fnWinProcHoverBox;
        wndClass.lpszClassName = szHoverBoxClass;
        wndClass.lpszMenuName = NULL;
        wndClass.style = CS_HREDRAW | CS_VREDRAW;

        if(!RegisterClass(&wndClass)){
            ERR_CHAR_RESERVE;
            ERR_CHAR_FILL_1_VALUE(
                "Cannot Register Hover Box Class",
                NULL
            );
            
            MessageBox(
                parent,
                ERR_CHAR,_TEXT("ERROR : Cannot Register Hover Box Class!"),
                MB_APPLMODAL | MB_ICONERROR
            );
            return;
        }

    }

}

LRESULT CALLBACK fnWinProcHoverBox(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
    return DefWindowProc(hWnd,msg,wParam,lParam);
}