#include "../headers/client.h"


LRESULT CALLBACK createGameWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Packet packet             = { 0 };
    static HWND hwndGameName  = NULL;
    static HWND hwndNbPlayers = NULL;
    static HWND hwndNbBoats   = NULL;
    static char nbPlayers[3]  = { 0 };
    static char nbBoats[3]    = { 0 };

    switch (uMsg)
    {
    case WM_CREATE:
        CreateWindowW(L"static", L"Créer une partie", SS_CENTER | WS_VISIBLE | WS_CHILD, 130, 20, 200, 20, hWnd, NULL, NULL, NULL);
        CreateWindowW(L"static", L"Nom de la partie", WS_BORDER | WS_VISIBLE | WS_CHILD, 10, 70, 130, 20, hWnd, NULL, NULL, NULL);
        hwndGameName = CreateWindowW(L"edit", NULL, WS_BORDER | WS_VISIBLE | WS_CHILD, 150, 70, 290, 20, hWnd, NULL, NULL, NULL);
        CreateWindowW(L"static", L"Nombre de joueurs : ", WS_BORDER | WS_VISIBLE | WS_CHILD, 10, 100, 130, 20, hWnd, NULL, NULL, NULL);
        hwndNbPlayers = CreateWindowA("edit", "2", WS_BORDER | WS_VISIBLE | WS_CHILD, 150, 100, 20, 20, hWnd, NULL, NULL, NULL);
        CreateWindowW(L"static", L"Nombre de bateaux : ", WS_BORDER | WS_VISIBLE | WS_CHILD, 200, 100, 130, 20, hWnd, NULL, NULL, NULL);
        hwndNbBoats = CreateWindowA("edit", "3", WS_BORDER | WS_VISIBLE | WS_CHILD, 340, 100, 15, 20, hWnd, NULL, NULL, NULL);
        CreateWindowW(L"button", L"Créer la partie", WS_BORDER | WS_VISIBLE | WS_CHILD, 160, 150, 150, 40, hWnd, (HMENU)ID_BUTTON_GAME_CREATE, NULL, NULL);
        break;
    case WM_CLOSE:
        EnableWindow(GetWindow(hWnd, GW_OWNER), TRUE);
        SetActiveWindow(GetWindow(hWnd, GW_OWNER));
        DestroyWindow(hWnd);
        break;
    case WM_COMMAND:
        switch (wParam)
        {
            case ID_BUTTON_GAME_CREATE:
                GetWindowTextA(hwndGameName, packet.game.gamename, SIZE_GAMENAME);
                GetWindowTextA(hwndNbPlayers, nbPlayers, 3);
                packet.game.nbPlayers = atoi(nbPlayers);
                GetWindowTextA(hwndNbBoats, nbBoats, 3);
                packet.game.nbBoats = atoi(nbBoats);

                if (packet.game.gamename[0] == 0 || packet.game.nbPlayers < 1 || packet.game.nbBoats < 1 || packet.game.nbBoats > 10)
                {
                    MessageBoxW(hWnd, L"Vérifier les champs", L"Impossible de créer la partie", MB_ICONERROR);
                    break;
                }
                packet.messageType = MESSAGE_TYPE_GAME_CREATE_REQUEST;
                write_server(&packet);
                break;
        }
    case WM_USER:
        switch (wParam)
        {
        case MESSAGE_TYPE_GAME_CREATE_ALREADY_EXIST:
            MessageBoxW(hWnd, L"Partie existe", L"La partie existe deja", MB_ICONERROR);
            break;
        case MESSAGE_TYPE_GAME_CREATE_SUCCESS:
            MessageBoxW(hWnd, L"Partie créee", L"La partie a été créee", MB_ICONINFORMATION);
            SendMessageW(hWnd, WM_CLOSE, 0, 0);
            break;
        }
    default:
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }
}