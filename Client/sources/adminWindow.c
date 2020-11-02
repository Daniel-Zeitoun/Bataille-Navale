#include "../headers/client.h"

LRESULT CALLBACK adminWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Packet packet = { 0 };
    RECT rect = { 0 };
    switch (uMsg)
    {
    case WM_CREATE:
        CreateWindowW(L"static", L"Bienvenue sur la console d'administration", BS_CENTER | SS_CENTER | WS_CHILD | WS_VISIBLE, 150, 20, 300, 20, hWnd, NULL, NULL, NULL);
        char bufferMessageBox[2048] = { 0 };
        sprintf(bufferMessageBox, "Vous �tes connect� au serveur %s:%d", server, port);
        CreateWindowA("static", bufferMessageBox, BS_CENTER | SS_CENTER | WS_CHILD | WS_VISIBLE, 50, 60, 500, 20, hWnd, NULL, NULL, NULL);
        CreateWindowW(L"button", L"Cr�er un compte", WS_BORDER | WS_VISIBLE | WS_CHILD, 200, 150, 200, 40, hWnd, (HMENU)ID_BUTTON_ACCOUNT_CREATE, NULL, NULL);
        CreateWindowW(L"button", L"Supprimer un compte", WS_BORDER | WS_VISIBLE | WS_CHILD, 200, 200, 200, 40, hWnd, (HMENU)ID_BUTTON_ACCOUNT_DELETE, NULL, NULL);
        CreateWindowW(L"button", L"Cr�er une partie", WS_BORDER | WS_VISIBLE | WS_CHILD, 200, 250, 200, 40, hWnd, (HMENU)ID_BUTTON_GAME_CREATE, NULL, NULL);
        CreateWindowW(L"button", L"Liste des parties", WS_BORDER | WS_VISIBLE | WS_CHILD, 200, 300, 200, 40, hWnd, (HMENU)ID_BUTTON_GAME_LIST, NULL, NULL);
        CreateWindowW(L"button", L"Se d�connecter", WS_BORDER | WS_VISIBLE | WS_CHILD, 200, 350, 200, 40, hWnd, (HMENU)ID_BUTTON_DISCONNECT, NULL, NULL);
        break;
    case WM_CLOSE:
        packet.messageType = MESSAGE_TYPE_CLOSE_CONNECTION;
        send(sock, &packet, sizeof(Packet), 0);
        PostQuitMessage(0);
        break;

    case WM_COMMAND:
        switch (wParam)
        {
        case ID_BUTTON_DISCONNECT:
            packet.messageType = MESSAGE_TYPE_CLOSE_CONNECTION;
            send(sock, &packet, sizeof(Packet), 0);
            TerminateThread(threadKeepAlive, EXIT_SUCCESS);
            TerminateThread(threadRecvFromServer, EXIT_SUCCESS);
            GetWindowRect(hWnd, &rect);
            DestroyWindow(hWnd);
            
            login.hwnd = CreateWindowW(login.className, titleBar, WS_VISIBLE | WS_SYSMENU, rect.left, rect.top, 400, 300, NULL, NULL, NULL, NULL);
            break;
        case ID_BUTTON_ACCOUNT_CREATE:
            GetWindowRect(hWnd, &rect);
            createAccount.hwnd = CreateWindowW(createAccount.className, titleBar, WS_VISIBLE | WS_SYSMENU, rect.left, rect.top, 460, 300, hWnd, NULL, NULL, NULL);
            EnableWindow(hWnd, FALSE);
            break;
        case ID_BUTTON_ACCOUNT_DELETE:
            GetWindowRect(hWnd, &rect);
            deleteAccount.hwnd = CreateWindowW(deleteAccount.className, titleBar, WS_VISIBLE | WS_SYSMENU, rect.left, rect.top, 300, 380, hWnd, NULL, NULL, NULL);
            EnableWindow(hWnd, FALSE);
            SetForegroundWindow(hWnd);
            break;
        case ID_BUTTON_GAME_CREATE:
            GetWindowRect(hWnd, &rect);
            createGame.hwnd = CreateWindowW(createGame.className, titleBar, WS_VISIBLE | WS_SYSMENU, rect.left, rect.top, 460, 280, hWnd, NULL, NULL, NULL);
            EnableWindow(hWnd, FALSE);
            break;
        case ID_BUTTON_GAME_LIST:
            GetWindowRect(hWnd, &rect);
            listGame.hwnd = CreateWindowW(listGame.className, titleBar, WS_VISIBLE | WS_SYSMENU, rect.left, rect.top, 300, 380, hWnd, NULL, NULL, NULL);
            EnableWindow(hWnd, FALSE);
            break;
        }
    default:
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }
}