@echo off

set SRC=helpers.c CustomMenu.c main.c hoverBox.c MenuPopUp.c
set OUT=CustomMenu

gcc ^
    %SRC% ^
    -o %OUT% ^
    -lgdi32 ^
    && %OUT%.exe
