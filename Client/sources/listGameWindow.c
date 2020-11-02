#include "../headers/client.h"

LRESULT CALLBACK listGameWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    RECT rect = { 0 };
    Packet packet = { 0 };
    int itemIndex = 0;

    switch (uMsg)
    {
    case WM_CREATE:
        CreateWindowW(L"button", L"Gérer la partie", WS_VISIBLE | WS_CHILD | WS_BORDER, 40, 250, 200, 40, hWnd, (HMENU)ID_BUTTON_GAME_MANAGE, NULL, NULL);
        listboxGame = CreateWindowW(L"listbox", NULL, WS_BORDER|WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL, 40, 40, 200, 200, hWnd, NULL, NULL, NULL);
        packet.messageType = MESSAGE_TYPE_GAME_ENUMERATE_REQUEST;
        write_server(&packet);
        break;

    case WM_CLOSE:
        EnableWindow(GetWindow(hWnd, GW_OWNER), TRUE);
        SetActiveWindow(GetWindow(hWnd, GW_OWNER));
        DestroyWindow(hWnd);
        break;
    case WM_COMMAND:
        switch (wParam)
        {
            case ID_BUTTON_GAME_MANAGE:
                memset(gamename, 0, SIZE_GAMENAME + 1);
                if ((itemIndex = (int)SendMessage(listboxGame, LB_GETCURSEL, 0, 0)) == LB_ERR)
                    break;
                SendMessageA(listboxGame, LB_GETTEXT, (WPARAM)itemIndex, (LPARAM)gamename);
                //packet.messageType = MESSAGE_TYPE_GAME_RUNNING_REQUEST;
                memcpy(packet.game.gamename, gamename, SIZE_GAMENAME);
                GetWindowRect(hWnd, &rect);
                manageGame.hwnd = CreateWindowW(manageGame.className, titleBar, WS_VISIBLE | WS_SYSMENU, rect.left, rect.top, 300, 380, hWnd, NULL, NULL, NULL);
                EnableWindow(hWnd, FALSE);
                break;
        }
    case WM_USER:
    {
        switch (wParam)
        {
        case 0:
            GetWindowRect(hWnd, &rect);
            manageGame.hwnd = CreateWindowW(manageGame.className, titleBar, WS_VISIBLE | WS_SYSMENU, rect.left, rect.top, 300, 380, hWnd, NULL, NULL, NULL);
            EnableWindow(hWnd, FALSE);
            break;
        case 1:
            GetWindowRect(hWnd, &rect);
            manageGame.hwnd = CreateWindowW(manageGame.className, titleBar, WS_VISIBLE | WS_SYSMENU, rect.left, rect.top, 300, 380, hWnd, NULL, NULL, NULL);
            EnableWindow(hWnd, FALSE);
            break;
        }
    }
    default:
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }
}