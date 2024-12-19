#include "ui_manager.h"

HWND hwndGroup, hwndLabel, hwndEdit, hwndButton;

void CreateUI(HWND hwnd) {
    GetClientRect(hwnd, &clientRect);

    appFont = CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, VARIABLE_PITCH | FF_SWISS, "MS Sans Serif");

    hwndGroup = CreateWindow(
        "BUTTON", "Resultado", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
        clientRect.left + 10, clientRect.top + 10,
        clientRect.right - 20, 100,
        hwnd, HMENU(GROUPBOX_ID), hInstanceApp, NULL);
    SendMessage(hwndGroup, WM_SETFONT, (WPARAM)appFont, TRUE);

    hwndLabel = CreateWindowEx(
        NULL, "STATIC", "Palavra:", WS_CHILD | WS_VISIBLE,
        40, 140, 140, 24,
        hwnd, HMENU(LABEL_TEXT_ID), hInstanceApp, NULL);
    SendMessage(hwndLabel, WM_SETFONT, (WPARAM)appFont, TRUE);

    hwndEdit = CreateWindowEx(
        WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
        100, 138, 130, 20,
        hwnd, HMENU(EDIT_TEXT_ID), hInstanceApp, NULL);
    SendMessage(hwndEdit, WM_SETFONT, (WPARAM)appFont, TRUE);

    hwndButton = CreateWindow(
        "BUTTON", "Verificar", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        255, 135, 100, 24,
        hwnd, HMENU(BUTTON_ID), hInstanceApp, NULL);
    SendMessage(hwndButton, WM_SETFONT, (WPARAM)appFont, TRUE);
}

void ClearResultArea(HWND hwnd) {
    RECT strRect = { clientRect.left + 20, clientRect.top + 40, clientRect.right - 20, clientRect.top + 90 };
    InvalidateRect(hwnd, &strRect, TRUE);
    UpdateWindow(hwnd);
}
