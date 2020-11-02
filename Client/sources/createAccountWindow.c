#include "../headers/client.h"


LRESULT CALLBACK createAccountWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    char   confirmation[SIZE_PASSWORD + 1] = { 0 };
    Packet packet                          = { 0 };
    static HWND hwndUsername                      = NULL;
    static HWND hwndPassword                      = NULL;
    static HWND hwndConfirmation                  = NULL;

    switch (uMsg)
    {
    case WM_CREATE:
        CreateWindowW(L"static", L"Création de compte", SS_CENTER | WS_VISIBLE | WS_CHILD, 130, 20, 200, 20, hWnd, NULL, NULL, NULL);
        CreateWindowW(L"static", L"Utilisateur", WS_BORDER | WS_VISIBLE | WS_CHILD, 10, 70, 180, 20, hWnd, NULL, NULL, NULL);
        hwndUsername = CreateWindowW(L"edit", NULL, WS_BORDER | WS_VISIBLE | WS_CHILD, 200, 70, 230, 20, hWnd, NULL, NULL, NULL);
        CreateWindowW(L"static", L"Mot de passe", WS_BORDER | WS_VISIBLE | WS_CHILD, 10, 100, 180, 20, hWnd, NULL, NULL, NULL);
        hwndPassword = CreateWindowA("edit", NULL, WS_BORDER | WS_VISIBLE | WS_CHILD, 200, 100, 230, 20, hWnd, NULL, NULL, NULL);
        CreateWindowW(L"static", L"Confirmer le mot de passe", WS_BORDER | WS_VISIBLE | WS_CHILD, 10, 130, 180, 20, hWnd, NULL, NULL, NULL);
        hwndConfirmation = CreateWindowA("edit", NULL, WS_BORDER | WS_VISIBLE | WS_CHILD, 200, 130, 230, 20, hWnd, NULL, NULL, NULL);
        CreateWindowW(L"button", L"Créer le compte", WS_BORDER | WS_VISIBLE | WS_CHILD, 160, 180, 150, 40, hWnd, (HMENU)ID_BUTTON_ACCOUNT_CREATE, NULL, NULL);
        break;
    case WM_CLOSE:
        EnableWindow(GetWindow(hWnd, GW_OWNER), TRUE);
        SetActiveWindow(GetWindow(hWnd, GW_OWNER));
        DestroyWindow(hWnd);
        break;
    case WM_COMMAND:
        switch (wParam)
        {
        case ID_BUTTON_ACCOUNT_CREATE:
            packet.messageType = MESSAGE_TYPE_ACCOUNT_CREATE_REQUEST;
            GetWindowTextA(hwndUsername, packet.account.username, SIZE_USERNAME);
            GetWindowTextA(hwndPassword, packet.account.password, SIZE_PASSWORD);
            GetWindowTextA(hwndConfirmation, confirmation, SIZE_PASSWORD);

            if (packet.account.username[0] == 0 || packet.account.password[0] == 0 || confirmation[0] == 0)
            {
                MessageBoxW(hWnd, L"Vérifier les champs", L"Impossible de créer le compte", MB_ICONERROR);
                break;
            }
            if (strncmp(packet.account.password, confirmation, SIZE_PASSWORD) != 0)
            {
                MessageBoxW(hWnd, L"Les mots de passe ne sont pas identiques", L"Impossible de créer le compte", MB_ICONERROR);
                break;
            }
            write_server(&packet);
            break;
        }
    case WM_USER:
    {
        switch (wParam)
        {
        case MESSAGE_TYPE_ACCOUNT_CREATE_ALREADY_EXIST:
            MessageBoxW(hWnd, L"L'utilisateur existe déjà", L"Impossible de créer le compte", MB_ICONERROR);
            break;
        case MESSAGE_TYPE_ACCOUNT_CREATE_OK:
            MessageBoxW(hWnd, L"Le compte a été créer", L"Création compte", MB_ICONINFORMATION);
            SendMessageW(hWnd, WM_CLOSE, 0, 0);
            break;
        case MESSAGE_TYPE_INTERNAL_ERROR:
            MessageBoxW(hWnd, L"Erreur interne", L"Une erreur est survenue", MB_ICONERROR);
            break;
        }
    }
    default:
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }
}