#include "../headers/client.h"




LRESULT CALLBACK loginWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HMENU menu;
    RECT rect = { 0 };
    switch (uMsg)
    {
    case WM_CREATE:
        menu = CreateMenu();
        AppendMenuW(menu, MF_STRING, ID_MENU_TOOLS, L"Paramètres réseau");
        SetMenu(hWnd, menu);

        CreateWindowW(L"static", L"Utilisateur", WS_BORDER | WS_VISIBLE | WS_CHILD, 70, 50, 100, 20, hWnd, NULL, NULL, NULL);
        hwndUsername = CreateWindowW(L"edit", L"admin", WS_BORDER | WS_VISIBLE | WS_CHILD, 180, 50, 150, 20, hWnd, NULL, NULL, NULL);

        CreateWindowW(L"static", L"Mot de passe", WS_BORDER | WS_VISIBLE | WS_CHILD, 70, 80, 100, 20, hWnd, NULL, NULL, NULL);
        hwndPassword = CreateWindowW(L"edit", L"ESGI", WS_BORDER | WS_VISIBLE | WS_CHILD, 180, 80, 150, 20, hWnd, NULL, NULL, NULL);

        CreateWindowW(L"button", L"Se connecter", WS_BORDER | WS_VISIBLE | WS_CHILD, 150, 140, 100, 40, hWnd, (HMENU)ID_BUTTON_CONNECT, NULL, NULL);
        break;

    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    case WM_COMMAND:
        switch (wParam)
        {
        case ID_MENU_TOOLS:
            GetWindowRect(hWnd, &rect);
            CreateWindowW(tools.className, titleBar, WS_VISIBLE|WS_CAPTION|WS_SYSMENU, rect.left, rect.top, 380, 200, hWnd, NULL, NULL, NULL);
            EnableWindow(hWnd, FALSE);
            break;
        case ID_BUTTON_CONNECT:
            {
                Packet packet = { 0 };
                switch (connect_server(hWnd).messageType)
                {
                case MESSAGE_TYPE_LOGIN_REPLY_ADMIN_SUCCESS:
                    threadKeepAlive = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)keep_alive_thread_proc, sock, 0, NULL);
                    threadRecvFromServer = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)recv_from_server_thread_proc, sock, 0, NULL);
                    GetWindowRect(hWnd, &rect);
                    admin.hwnd = CreateWindowW(admin.className, titleBar, WS_VISIBLE | WS_CAPTION | WS_SYSMENU, rect.left, rect.top, 600, 500, NULL, NULL, NULL, NULL);
                    DestroyWindow(hWnd);
                    break;
                case MESSAGE_TYPE_LOGIN_REPLY_SUCCESS:
                    /*threadKeepAlive = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)keep_alive_thread_proc, sock, 0, NULL);
                    threadRecvFromServer = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)recv_from_server_thread_proc, sock, 0, NULL);
                    strcat(packet.gamecreate.gamename, "Partie 1");
                    packet.gamecreate.nbBoats = 1;
                    packet.gamecreate.nbPlayers = 1;
                    packet.messageType = MESSAGE_TYPE_GAME_CREATE_REQUEST;
                    write_server(&packet);
                    packet.messageType = MESSAGE_TYPE_GAME_JOIN_REQUEST;
                    strncpy(packet.gamename, "Partie 1", SIZE_GAMENAME);
                    write_server(&packet);
                    packet.messageType = MESSAGE_TYPE_GAME_START;
                    strcat(packet.gamename, "Partie 1");
                    write_server(&packet);
                    game.hwnd = CreateWindowW(game.className, titleBar, WS_VISIBLE | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, NULL, NULL);
                    DestroyWindow(hWnd);*/
                    //DestroyWindow(hWnd);
                    //Debug
                    /*
                    packet.messageType = MESSAGE_TYPE_GAME_JOIN_REQUEST;
                    strncpy(packet.gamename, "Partie 1", SIZE_GAMENAME);
                    strncpy(packet.user.username, username, SIZE_USERNAME);
                    request_reply_server(&packet);
                    threadKeepAlive = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)keep_alive_thread_proc, sock, 0, NULL);
                    CreateWindowW(waitingClassName, L"Bataille Navale", WS_VISIBLE | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 400, 200, NULL, NULL, NULL, NULL);
                    DestroyWindow(hWnd);
                    break;*/


                    /*DWORD ThreadKeepAlive;
                    threadKeepAlive = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)keep_alive_thread_proc, sock, 0, &ThreadKeepAlive);
                    TerminateThread(threadKeepAlive, EXIT_SUCCESS);*/

                    threadKeepAlive = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)keep_alive_thread_proc, sock, 0, NULL);
                    threadRecvFromServer = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)recv_from_server_thread_proc, sock, 0, NULL);
                    GetWindowRect(hWnd, &rect);
                    user.hwnd = CreateWindowW(user.className, L"Bataille Navale", WS_VISIBLE | WS_CAPTION | WS_SYSMENU, rect.left, rect.top, 600, 500, NULL, NULL, NULL, NULL);
                    DestroyWindow(hWnd);
                    break;
                case MESSAGE_TYPE_PLAYER_IN_GAME:
                    threadKeepAlive = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)keep_alive_thread_proc, sock, 0, NULL);
                    threadRecvFromServer = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)recv_from_server_thread_proc, sock, 0, NULL);
                    GetWindowRect(hWnd, &rect);
                    waiting.hwnd = CreateWindowW(waiting.className, titleBar, WS_VISIBLE | WS_CAPTION | WS_SYSMENU, rect.left, rect.top, 400, 200, NULL, NULL, NULL, NULL);
                    DestroyWindow(hWnd);
                    break;
                case MESSAGE_TYPE_PLAYER_IN_GAME_RUNNING:
                    threadKeepAlive = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)keep_alive_thread_proc, sock, 0, NULL);
                    threadRecvFromServer = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)recv_from_server_thread_proc, sock, 0, NULL);
                    GetWindowRect(hWnd, &rect);
                    game.hwnd = CreateWindowW(game.className, titleBar, WS_VISIBLE | WS_SYSMENU, rect.left, rect.top, 800, 600, NULL, NULL, NULL, NULL);
                    DestroyWindow(hWnd);
                    break;
                case MESSAGE_TYPE_LOGIN_REPLY_BAD_USERNAME:
                    memset(&packet, 0, sizeof(Packet));
                    close_connection();
                    MessageBoxW(hWnd, L"Cet utilisateur n'éxiste pas", L"Connexion échouée", MB_ICONERROR);
                    break;
                case MESSAGE_TYPE_LOGIN_REPLY_BAD_PASSWORD:
                    close_connection();
                    MessageBoxW(hWnd, L"Le mot de passe est incorect", L"Connexion échouée", MB_ICONERROR);
                    break;
                case MESSAGE_TYPE_INTERNAL_ERROR:
                    close_connection();
                    MessageBoxW(hWnd, L"Une erreur interne est survenue", L"Erreur", MB_ICONERROR);
                    break;
                }
            }
        }
    default:
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }
}