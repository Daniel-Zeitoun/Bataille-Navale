#include "../headers/client.h"

LRESULT CALLBACK toolsWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static char portString[10] = { 0 };
    switch (uMsg)
    {
    case WM_CREATE:
        itoa(port, portString, 10);

        CreateWindowW(L"static", L"Serveur", WS_BORDER | WS_VISIBLE | WS_CHILD, 10, 20, 100, 20, hWnd, NULL, NULL, NULL);
        hwndServer = CreateWindowA("edit", server, WS_BORDER | WS_VISIBLE | WS_CHILD, 120, 20, 230, 20, hWnd, NULL, NULL, NULL);

        CreateWindowW(L"static", L"Port", WS_BORDER | WS_VISIBLE | WS_CHILD, 10, 50, 100, 20, hWnd, NULL, NULL, NULL);
        hwndPort = CreateWindowA("edit", portString, WS_BORDER | WS_VISIBLE | WS_CHILD, 120, 50, 50, 20, hWnd, NULL, NULL, NULL);

        CreateWindowW(L"button", L"OK", WS_BORDER | WS_VISIBLE | WS_CHILD, 165, 90, 50, 40, hWnd, (HMENU)ID_BUTTON_OK, NULL, NULL);
        break;
    case WM_CLOSE:
        EnableWindow(GetWindow(hWnd, GW_OWNER), TRUE);
        SetActiveWindow(GetWindow(hWnd, GW_OWNER));
        DestroyWindow(hWnd);
        break;
    case WM_COMMAND:
        switch (wParam)
        {
        case ID_BUTTON_OK:
            {
                char tmp[10] = { 0 };
                memset(server, 0, 2048);
                GetWindowTextA(hwndServer, server, 2048);
                GetWindowTextA(hwndPort, tmp, 5);
                port = atoi(tmp);
                EnableWindow(GetWindow(hWnd, GW_OWNER), TRUE);
                DestroyWindow(hWnd);
                break;
            }
        }
    default:
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }
}