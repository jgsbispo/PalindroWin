#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vector>
#include <string>

#pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// Definições
#define GROUPBOX_ID 100
#define LABEL_TEXT_ID 200
#define EDIT_TEXT_ID 300
#define BUTTON_ID 400

// Variáveis globais
bool resultClean = true;
HINSTANCE hInstanceApp;
RECT clientRect;
HWND hwndGroup;
HWND hwndLabel;
HWND hwndEdit;
HWND hwndButton;
HFONT appFont;

// Protótipo da janela principal
LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

// Função para criar controles com fonte atribuída
template<typename... Args>
HWND CreateControl(const char* className, const char* title, DWORD style, HWND parent, int id, HFONT font, Args... args) {
    HWND hwnd = CreateWindowEx(
        0, className, title, style, args..., parent, (HMENU)id, hInstanceApp, NULL);
    SendMessage(hwnd, WM_SETFONT, (WPARAM)font, TRUE);
    return hwnd;
}

// Função principal
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    hInstanceApp = hInstance;

    WNDCLASS wndclass = {
        CS_HREDRAW | CS_VREDRAW, WinProc, 0, 0, hInstance, LoadIcon(NULL, IDI_APPLICATION),
        LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_BTNFACE + 1), NULL, "BasicWindow"
    };

    if (!RegisterClass(&wndclass)) {
        MessageBox(NULL, "Erro na criação da janela!", "Palíndromo", MB_ICONERROR);
        return 0;
    }

    int xWndPos = GetSystemMetrics(SM_CXSCREEN) / 2 - 200;
    int yWndPos = GetSystemMetrics(SM_CYSCREEN) / 2 - 120;

    HWND hwnd = CreateWindow("BasicWindow", "Palíndromo", WS_OVERLAPPED | WS_SYSMENU,
        xWndPos, yWndPos, 400, 220, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// Gerenciador de mensagens
LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        GetClientRect(hwnd, &clientRect);
        appFont = CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, VARIABLE_PITCH | FF_SWISS, "MS Sans Serif");

        hwndGroup = CreateControl("BUTTON", "Resultado", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, hwnd, GROUPBOX_ID, appFont,
            clientRect.left + 10, clientRect.top + 10, clientRect.right - 20, 100);

        hwndLabel = CreateControl("STATIC", "Palavra:", WS_CHILD | WS_VISIBLE, hwnd, LABEL_TEXT_ID, appFont,
            40, 140, 140, 24);

        hwndEdit = CreateControl("EDIT", "", WS_CHILD | WS_VISIBLE | WS_EX_CLIENTEDGE | ES_AUTOHSCROLL,
            hwnd, EDIT_TEXT_ID, appFont, 100, 138, 130, 20);

        hwndButton = CreateControl("BUTTON", "Verificar", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, hwnd, BUTTON_ID, appFont,
            255, 135, 100, 24);
        break;
    }

    case WM_COMMAND: {
        if (HIWORD(wParam) == EN_CHANGE && !resultClean) {
            RECT strRect = { clientRect.left + 20, clientRect.top + 40, clientRect.right - 20, clientRect.top + 90 };
            InvalidateRect(hwnd, &strRect, TRUE);
            UpdateWindow(hwnd);
            resultClean = true;
        }

        if (LOWORD(wParam) == BUTTON_ID) {
            int strLen = GetWindowTextLength(hwndEdit);
            if (strLen == 0) {
                MessageBox(hwnd, "Digite uma palavra.", "Erro", MB_OK | MB_ICONEXCLAMATION);
                return 0;
            }

            std::vector<char> buffer(strLen + 1);
            GetWindowText(hwndEdit, buffer.data(), strLen + 1);
            std::string palavra(buffer.data());

            bool isPalindrome = true;
            for (size_t i = 0, j = palavra.length() - 1; i < j; ++i, --j) {
                if (palavra[i] != palavra[j]) {
                    isPalindrome = false;
                    break;
                }
            }

            HDC hdc = GetDC(hwnd);
            SelectObject(hdc, appFont);
            SetBkMode(hdc, TRANSPARENT);
            RECT strRect = { clientRect.left + 20, clientRect.top + 40, clientRect.right - 20, clientRect.top + 90 };

            DrawText(hdc, isPalindrome ? "A palavra é um palíndromo" : "Não é um palíndromo",
                -1, &strRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

            ReleaseDC(hwnd, hdc);
            resultClean = false;
        }
        break;
    }

    case WM_DESTROY:
        DeleteObject(appFont);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
