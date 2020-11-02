#include "../headers/client.h"


LRESULT CALLBACK manageGameWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Packet packet        = { 0 };
    static HWND hWndList = NULL;
    static int nbPlayers = 0;
    int itemIndex        = 0;

    switch (uMsg)
    {
    case WM_CREATE:
        CreateWindowW(L"button", L"Expulser", WS_VISIBLE | WS_CHILD | WS_BORDER, 40, 120, 200, 40, hWnd, (HMENU)ID_BUTTON_EJECT_PLAYER, NULL, NULL);
        CreateWindowW(L"button", L"Lancer la partie", WS_VISIBLE | WS_CHILD | WS_BORDER, 40, 180, 200, 50, hWnd, (HMENU)ID_BUTTON_START_GAME, NULL, NULL);
        CreateWindowW(L"button", L"Voir la partie", WS_VISIBLE | WS_CHILD | WS_BORDER, 40, 250, 200, 50, hWnd, (HMENU)ID_BUTTON_GAME_SHOW, NULL, NULL);
        listboxPlayers = CreateWindowW(L"listbox", NULL, WS_BORDER|WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL, 40, 40, 200, 40, hWnd, NULL, NULL, NULL);
        packet.messageType = MESSAGE_TYPE_GAME_PLAYER_NUMBER_REQUEST;
        strncpy(packet.game.gamename, gamename, SIZE_GAMENAME);
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
            case ID_BUTTON_EJECT_PLAYER:
                if ((itemIndex = (int)SendMessage(listboxPlayers, LB_GETCURSEL, 0, 0)) == LB_ERR)
                    break;
                memset(&packet, 0, sizeof(Packet));
                SendMessageA(listboxPlayers, LB_GETTEXT, (WPARAM)itemIndex, (LPARAM)packet.account.username);
                packet.messageType = MESSAGE_TYPE_GAME_PLAYER_EJECT;
                memcpy(packet.game.gamename, gamename, SIZE_GAMENAME);
                write_server(&packet);
                SendMessage(listboxPlayers, LB_RESETCONTENT, 0, 0);
                packet.messageType = MESSAGE_TYPE_GAME_PLAYER_NUMBER_REQUEST;
                write_server(&packet);
                break;
            case ID_BUTTON_START_GAME:
                packet.messageType = MESSAGE_TYPE_GAME_START_REQUEST;
                memcpy(packet.game.gamename, gamename, SIZE_GAMENAME);
                write_server(&packet);
                break;
            case ID_BUTTON_GAME_SHOW:
                packet.messageType = MESSAGE_TYPE_GAME_SHOW_ADMIN;
                memcpy(packet.game.gamename, gamename, SIZE_GAMENAME);
                write_server(&packet);
                break;
        }
        break;
    case WM_USER:
        switch (wParam)
        {
        case MESSAGE_TYPE_GAME_START_SUCCESS:
            MessageBoxW(hWnd, L"La game demarre", L"GAME", MB_ICONINFORMATION);
            break;
        case MESSAGE_TYPE_GAME_START_NOT_FULL:
            MessageBoxW(hWnd, L"La game n'est pas encore pleine", L"GAME", MB_ICONERROR);
            break;
        case MESSAGE_TYPE_SHOW_ADMIN_REPLY:
            EnableWindow(hWnd, FALSE);
            RECT rect = { 0 };
            GetWindowRect(hWnd, &rect);
            game.hwnd = CreateWindowW(game.className, titleBar, WS_VISIBLE | WS_SYSMENU, rect.left, rect.top, 800, 600, hWnd, NULL, NULL, NULL);
            break;
        }
    default:
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }
}