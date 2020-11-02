#include "../headers/client.h"

LRESULT CALLBACK joinGameWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    RECT rect = { 0 };
    char   textBuffer[SIZE_USERNAME + 1]   = { 0 };
    Packet packet                          = { 0 };
    int itemIndex = 0;

    switch (uMsg)
    {
    case WM_CREATE:
        CreateWindowW(L"button", L"Rejoindre la partie", WS_VISIBLE | WS_CHILD | WS_BORDER, 40, 250, 200, 40, hWnd, (HMENU)ID_BUTTON_GAME_JOIN, NULL, NULL);
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
            case ID_BUTTON_GAME_JOIN:
                if ((itemIndex = (int)SendMessage(listboxGame, LB_GETCURSEL, 0, 0)) == LB_ERR)
                    break;
                SendMessageA(listboxGame, LB_GETTEXT, (WPARAM)itemIndex, (LPARAM)textBuffer);

                packet.messageType = MESSAGE_TYPE_GAME_JOIN_REQUEST;
                strncpy(packet.game.gamename, textBuffer, SIZE_GAMENAME);
                strncpy(packet.account.username, username, SIZE_USERNAME);

                write_server(&packet);
                break;
        }
    case WM_USER:
        switch (wParam)
        {
        case MESSAGE_TYPE_GAME_JOIN_OK:
            MessageBoxW(hWnd, L"Partie jointe", L"La partie commencera lorsque l'admin la lancera", MB_ICONINFORMATION);
            GetWindowRect(hWnd, &rect);
            waiting.hwnd = CreateWindowW(waiting.className, titleBar, WS_VISIBLE | WS_CAPTION | WS_SYSMENU, rect.left, rect.top, 400, 200, NULL, NULL, NULL, NULL);
            DestroyWindow(GetWindow(hWnd, GW_OWNER));
            break;
        case MESSAGE_TYPE_GAME_JOIN_FULL:
            MessageBoxW(hWnd, L"Impossible de rejoindre la partie", L"La partie est complète", MB_ICONERROR);
            break;
        case MESSAGE_TYPE_GAME_JOIN_ERROR:
            MessageBoxW(hWnd, L"Erreur", L"Vous etes deja dans la partie", MB_ICONERROR);
            break;
        }
    default:
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }
}