#include "../headers/client.h"

void recv_from_server_thread_proc(int socket)
{

    while (1)
    {
        char buffer[2048] = { 0 };
        Packet recipient = { 0 };
        recv(socket, &recipient, sizeof(Packet), 0);
        char* pointer = NULL;
        switch (recipient.messageType)
        {
        case MESSAGE_TYPE_INTERNAL_ERROR:
            SendMessage(NULL, WM_USER, MESSAGE_TYPE_INTERNAL_ERROR, 0);
            break;
        case MESSAGE_TYPE_ACCOUNT_CREATE_ALREADY_EXIST:
            SendMessage(createAccount.hwnd, WM_USER, MESSAGE_TYPE_ACCOUNT_CREATE_ALREADY_EXIST, 0);
            break;
        case MESSAGE_TYPE_ACCOUNT_CREATE_OK:
            SendMessage(createAccount.hwnd, WM_USER, MESSAGE_TYPE_ACCOUNT_CREATE_OK, 0);
            break;

        case MESSAGE_TYPE_USER_ENUMERATE_REPLY:
            SendMessageA(listboxDeleteAccount, LB_ADDSTRING, 0, (LPARAM)recipient.account.username);
            break;
        case MESSAGE_TYPE_ACCOUNT_DELETE_SUCCES:
            SendMessage(deleteAccount.hwnd, WM_USER, MESSAGE_TYPE_ACCOUNT_DELETE_SUCCES, 0);
            break;

        case MESSAGE_TYPE_GAME_CREATE_ALREADY_EXIST:
            SendMessage(createGame.hwnd, WM_USER, MESSAGE_TYPE_GAME_CREATE_ALREADY_EXIST, 0);
            break;
        case MESSAGE_TYPE_GAME_CREATE_SUCCESS:
            SendMessage(createGame.hwnd, WM_USER, MESSAGE_TYPE_GAME_CREATE_SUCCESS, 0);
            break;

        case MESSAGE_TYPE_GAME_NUMBER_REPLY:
            SendMessageA(listboxGame, LB_ADDSTRING, 0, (LPARAM)recipient.game.gamename);
            break;

        case MESSAGE_TYPE_GAME_PLAYER_NUMBER_REPLY:
            Sleep(100);
            SendMessageA(listboxPlayers, LB_ADDSTRING, 0, (LPARAM)recipient.account.username);

            break;
        case MESSAGE_TYPE_GAME_START_SUCCESS:
            SendMessage(manageGame.hwnd, WM_USER, MESSAGE_TYPE_GAME_START_SUCCESS, 0);
            SendMessageW(waiting.hwnd, WM_COMMAND, MESSAGE_TYPE_GAME_START_SUCCESS, 0);
            break;
        case MESSAGE_TYPE_GAME_START_NOT_FULL:
            SendMessage(manageGame.hwnd, WM_USER, MESSAGE_TYPE_GAME_START_NOT_FULL, 0);
            break;


        case MESSAGE_TYPE_GAME_JOIN_OK:
            SendMessage(joinGame.hwnd, WM_USER, MESSAGE_TYPE_GAME_JOIN_OK, 0);
            break;
        case MESSAGE_TYPE_GAME_JOIN_FULL:
            SendMessage(joinGame.hwnd, WM_USER, MESSAGE_TYPE_GAME_JOIN_FULL, 0);
            break;
        case MESSAGE_TYPE_GAME_JOIN_ERROR:
            SendMessage(joinGame.hwnd, WM_USER, MESSAGE_TYPE_GAME_JOIN_ERROR, 0);
            break;

        case MESSAGE_TYPE_GAME_PLAYER_EJECT:
            SendMessageW(waiting.hwnd, WM_COMMAND, MESSAGE_TYPE_GAME_PLAYER_EJECT, 0);
            break;


        case MESSAGE_TYPE_GRID_REPLY:
            Sleep(100);
            SendMessage(game.hwnd, WM_USER, MESSAGE_TYPE_GRID_REPLY, &recipient);
            break;
        case MESSAGE_TYPE_ITS_YOUR_TURN:
            SendMessage(game.hwnd, WM_USER, MESSAGE_TYPE_ITS_YOUR_TURN, 0);
            break;
        case MESSAGE_TYPE_ITS_NOT_YOUR_TURN:
            SendMessage(game.hwnd, WM_USER, MESSAGE_TYPE_ITS_NOT_YOUR_TURN, 0);
            break;
        case MESSAGE_TYPE_YOU_WIN:
            SendMessage(game.hwnd, WM_USER, MESSAGE_TYPE_YOU_WIN, 0);
            break;
        case MESSAGE_TYPE_YOU_LOSE:
            SendMessage(game.hwnd, WM_USER, MESSAGE_TYPE_YOU_LOSE, 0);
            break;
        //case MESSAGE_TYPE_GAME_RUNNING_REPLY:
         //   SendMessage(game.hwnd, WM_USER, recipient.game.isRunnig, 0);
        //    break;
        case MESSAGE_TYPE_SHOW_ADMIN_REPLY:
            SendMessage(manageGame.hwnd, WM_USER, MESSAGE_TYPE_SHOW_ADMIN_REPLY, 0);
            //system("pause");
            break;
        case MESSAGE_TYPE_GRID_ADMIN_REPLY:
            Sleep(100);
            SendMessage(game.hwnd, WM_USER, MESSAGE_TYPE_GRID_ADMIN_REPLY, &recipient);
            break;
        case MESSAGE_TYPE_FIRST_SCORE:
            sprintf(buffer, "%s : %d\r\n", recipient.account.username, recipient.game.score);
            SetWindowTextA(scores, buffer);
            UpdateWindow(scores);
            break;
        case MESSAGE_TYPE_SCORE:
            sprintf(buffer, "%s : %d\r\n", recipient.account.username, recipient.game.score);
            int ndx;
            ndx = GetWindowTextLengthA(scores);
            SendMessageA(scores, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
            SendMessageA(scores, EM_REPLACESEL, 0, (LPARAM)(LPCTSTR)buffer);
            break;
        }
    }
}