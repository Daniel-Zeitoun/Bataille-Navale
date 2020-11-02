#include "../headers/client.h"


LRESULT CALLBACK deleteAccountWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Packet packet = { 0 };
    static HWND hWndList;
    int itemIndex = 0;

    switch (uMsg)
    {
    case WM_CREATE:
        CreateWindowW(L"button", L"Supprimer le compte", WS_VISIBLE | WS_CHILD | WS_BORDER, 40, 250, 200, 40, hWnd, (HMENU)ID_BUTTON_ACCOUNT_DELETE, NULL, NULL);
        listboxDeleteAccount = CreateWindowW(L"listbox", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL, 40, 40, 200, 200, hWnd, NULL, NULL, NULL);
        SendMessage(listboxDeleteAccount, LB_RESETCONTENT, 0, 0);
        packet.messageType = MESSAGE_TYPE_USER_ENUMERATE_REQUEST;
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
        case ID_BUTTON_ACCOUNT_DELETE:
            if ((itemIndex = (int)SendMessage(listboxDeleteAccount, LB_GETCURSEL, 0, 0)) == LB_ERR)
                break;

            memset(&packet, 0, sizeof(Packet));
            //SendMessageA(listboxDeleteAccount, LB_GETTEXT, (WPARAM)itemIndex, (LPARAM)packet.user.username);
            SendMessageA(listboxDeleteAccount, LB_GETTEXT, (WPARAM)itemIndex, (LPARAM)packet.account.username);
            packet.messageType = MESSAGE_TYPE_ACCOUNT_DELETE_REQUEST;
            write_server(&packet);
            break;
        }
    case WM_USER:
        switch (wParam)
        {
        case MESSAGE_TYPE_ACCOUNT_DELETE_SUCCES:
            MessageBoxW(hWnd, L"Le compte a été supprmé", L"Suppression compte", MB_ICONINFORMATION);
            SendMessage(listboxDeleteAccount, LB_RESETCONTENT, 0, 0);
            packet.messageType = MESSAGE_TYPE_USER_ENUMERATE_REQUEST;
            write_server(&packet);
            break;
        }

    default:
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }
}