#include "../headers/client.h"

LRESULT CALLBACK waitingWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    RECT rect = { 0 };
    switch (uMsg)
    {
    case WM_CREATE:
        CreateWindowW(L"static", L"En attente du serveur...", SS_CENTER | BS_CENTER | WS_VISIBLE | WS_CHILD, 100, 50, 200, 30, hWnd, NULL, NULL, NULL);
        break;
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    case WM_COMMAND:
        switch (wParam)
        {
            case MESSAGE_TYPE_GAME_PLAYER_EJECT:
                MessageBoxW(hWnd, L"Partie quitée", L"Vous avez été éjecté de la partie", MB_ICONERROR);
                GetWindowRect(hWnd, &rect);
                user.hwnd = CreateWindowW(user.className, titleBar, WS_VISIBLE | WS_CAPTION | WS_SYSMENU, rect.left, rect.top, 600, 500, NULL, NULL, NULL, NULL);
                DestroyWindow(hWnd);
                break;
            case MESSAGE_TYPE_GAME_START_SUCCESS:
                GetWindowRect(hWnd, &rect);
                game.hwnd = CreateWindowW(game.className, titleBar, WS_VISIBLE | WS_SYSMENU, rect.left, rect.top, 800, 600, NULL, NULL, NULL, NULL);
                DestroyWindow(hWnd);
                break;
        }
    default:
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }
}