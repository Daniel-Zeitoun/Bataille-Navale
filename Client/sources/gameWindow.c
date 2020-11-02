#include "../headers/client.h"


HGDIOBJ color_grid(WPARAM wParam, LPARAM lParam, ControlGame* controlGame)
{
    for (int i = 0; i < SIZE_GRID; i++)
    {
        for (int j = 0; j < SIZE_GRID; j++)
        {
            if ((HWND)lParam == controlGame->controlGrid[j][i].hwnd)
            {
                SetBkMode((HDC)wParam, TRANSPARENT);

                switch (controlGame->controlGrid[j][i].status)
                {
                case WATER:
                    SetDCPenColor((HDC)wParam, RGB(0, 255, 255));
                    break;
                case BOAT:
                    SetDCPenColor((HDC)wParam, RGB(168, 168, 168));
                    break;
                case BOAT_HIT:
                    SetDCPenColor((HDC)wParam, RGB(255, 0, 0));
                    break;
                case BOAT_SUNK:
                    SetDCPenColor((HDC)wParam, RGB(105, 105, 105));
                    break;
                }

                return GetStockObject(DC_PEN);
            }
        }
    }
}
void init_game_window(ControlGame* controlGame)
{
    Packet packet = { 0 };
    char buffer[128] = { 0 };

    CreateWindowA("static", username, BS_CENTER | SS_CENTER | WS_VISIBLE | WS_CHILD, 20, 10, 750, 60, controlGame->gameWindow, NULL, NULL, NULL);
    scores = CreateWindow(L"edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | WS_HSCROLL | WS_VSCROLL, 600, 100, 170, 400, controlGame->gameWindow, NULL, NULL, NULL);
    //EnableWindow(scores, FALSE);
    for (int i = 0; i < SIZE_GRID; i++)
    {
        sprintf(buffer, "%c\0", 'A' + i);
        CreateWindowA("static", buffer, BS_CENTER | SS_CENTER | WS_VISIBLE | WS_CHILD, START_GRID_X + SIZE_SPACE * (i + 1), START_GRID_Y, SIZE_SQUARE, SIZE_SQUARE, controlGame->gameWindow, NULL, NULL, NULL);

        for (int j = 0; j < SIZE_GRID; j++)
        {
            sprintf(buffer, "%d\0", j);
            CreateWindowA("static", buffer, BS_CENTER | SS_CENTER | WS_VISIBLE | WS_CHILD, START_GRID_X, START_GRID_Y + SIZE_SPACE * (j + 1), SIZE_SQUARE, SIZE_SQUARE, controlGame->gameWindow, NULL, NULL, NULL);
        }
    }

    for (int i = 0; i < SIZE_GRID; i++)
    {
        for (int j = 0; j < SIZE_GRID; j++)
        {
            grid[j][i] = 255;
        }
    }

    if (strncmp(username, "admin", strlen("admin")))
    {
        controlGame->staticLetter = CreateWindowW(L"static", L"", WS_BORDER | BS_CENTER | SS_CENTER | WS_VISIBLE | WS_CHILD, 375, 480, 20, 20, controlGame->gameWindow, NULL, NULL, NULL);
        controlGame->staticNumber = CreateWindowW(L"static", L"", WS_BORDER | BS_CENTER | SS_CENTER | WS_VISIBLE | WS_CHILD, 405, 480, 20, 20, controlGame->gameWindow, NULL, NULL, NULL);

        controlGame->shotButton = CreateWindowW(L"button", L"TIRER", BS_CENTER | SS_CENTER | WS_VISIBLE | WS_CHILD, 350, 510, 100, 40, controlGame->gameWindow, (HMENU)ID_BUTTON_SHOT, NULL, NULL);
        EnableWindow(controlGame->shotButton, FALSE);
    }
        
    memcpy(packet.account.username, username, SIZE_USERNAME);

    if (!strncmp(username, "admin", strlen("admin")))
    {
        strncpy(packet.game.gamename, gamename, SIZE_GAMENAME);
        packet.messageType = MESSAGE_TYPE_GRID_ADMIN_REQUEST;
    }
    else
        packet.messageType = MESSAGE_TYPE_GRID_REQUEST;

    write_server(&packet);

    if (strncmp(username, "admin", strlen("admin")))
    {
        packet.messageType = MESSAGE_TYPE_TURN_REQUEST;
        write_server(&packet);
    }
        
}
void send_shot(ControlGame* controlGame)
{
    Packet packet = { 0 };
    char   buffer[1024] = { 0 };

    packet.messageType = MESSAGE_TYPE_SEND_SHOT;

    GetWindowTextA(controlGame->staticLetter, buffer, 2);
    packet.game.point.x = buffer[0] - 'A';

    GetWindowTextA(controlGame->staticNumber, buffer, 3);
    packet.game.point.y = atoi(buffer);

    prevShot.x = packet.game.point.x;
    prevShot.y = packet.game.point.y;


    if (buffer[0] == 0)
        return;
    write_server(&packet);
    EnableWindow(controlGame->shotButton, FALSE);
    SetWindowTextA(controlGame->staticNumber, NULL);
    SetWindowTextA(controlGame->staticLetter, NULL);
    
    DestroyWindow(controlGame->controlGrid[packet.game.point.y][packet.game.point.x].hwnd);
    memset(&packet, 0, sizeof(Packet));
}

LRESULT CALLBACK gameWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    RECT rect = { 0 };
    Packet packet = { 0 };
    char   buffer[1024] = { 0 };
    static ControlGame controlGame = { 0 };
    int id = 0;
    static int ok = 0;
    
    switch (uMsg)
    {
    case WM_CREATE:
        controlGame.gameWindow = hWnd;
        init_game_window(&controlGame);
        break;
    case WM_CLOSE:
        if (!strncmp(username, "admin", strlen("admin")))
        {
            packet.messageType = MESSAGE_TYPE_GAME_STOP_SHOW_ADMIN;
            write_server(&packet);

            EnableWindow(GetWindow(hWnd, GW_OWNER), TRUE);
            SetActiveWindow(GetWindow(hWnd, GW_OWNER));
            DestroyWindow(hWnd);
            break;
        }
        PostQuitMessage(0);
        break;
     case WM_PAINT:
        PAINTSTRUCT ps;
        HDC         hdc;
        hdc = BeginPaint(hWnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hWnd, &ps);
        break;
    case WM_CTLCOLORSTATIC:
        return color_grid(wParam, lParam, &controlGame);
        break;
    case WM_COMMAND:
        for (int i = 1; i < 226; i++)
        {
            if (wParam == i)
            {
                sprintf(buffer, "%c\0", 'A' + (i - 1) / 15);
                SetWindowTextA(controlGame.staticLetter, buffer);

                sprintf(buffer, "%d\0",(i - 1) % 15);
                SetWindowTextA(controlGame.staticNumber, buffer);
            }
        }

        switch (wParam)
        {
        case ID_BUTTON_SHOT:

            send_shot(&controlGame);
            break;
        }

        break;
    case WM_USER:
        switch (wParam)
        {
        case MESSAGE_TYPE_GRID_ADMIN_REPLY:
        case MESSAGE_TYPE_GRID_REPLY:
            for (int i = 0; i < SIZE_GRID; i++)
            {
                for (int j = 0; j < SIZE_GRID; j++)
                {
                    if (((Packet*)lParam)->game.grid[j][i] == grid[j][i] && (prevShot.x != i || prevShot.y != j))
                    {
                        id++;
                        continue;
                    }

                    if (isdigit(((Packet*)lParam)->game.grid[j][i]))
                    {
                        memset(buffer, 0, sizeof(buffer));
                        sprintf(buffer, "%c\0", ((Packet*)lParam)->game.grid[j][i]);

                        controlGame.controlGrid[j][i].hwnd = CreateWindowA("static", "B", BS_CENTER | SS_CENTER | WS_VISIBLE | WS_CHILD, START_GRID_X + SIZE_SPACE * (i + 1), START_GRID_Y + SIZE_SPACE * (j + 1), SIZE_SQUARE, SIZE_SQUARE, hWnd, NULL, NULL, NULL);
                        controlGame.controlGrid[j][i].status = BOAT;
                    }
                    else
                    {
                        switch (((Packet*)lParam)->game.grid[j][i])
                        {
                        case 0:
                            if (strncmp(username, "admin", strlen("admin")))
                            {
                                controlGame.controlGrid[j][i].hwnd = CreateWindowA("button", "", BS_CENTER | SS_CENTER | WS_VISIBLE | WS_CHILD, START_GRID_X + SIZE_SPACE * (i + 1), START_GRID_Y + SIZE_SPACE * (j + 1), SIZE_SQUARE, SIZE_SQUARE, hWnd, id + 1, NULL, NULL);
                                controlGame.controlGrid[j][i].type = BUTTON;
                            }
                            
                            break;
                        case '~':
                            controlGame.controlGrid[j][i].hwnd = CreateWindowA("static", "", BS_CENTER | SS_CENTER | WS_VISIBLE | WS_CHILD, START_GRID_X + SIZE_SPACE * (i + 1), START_GRID_Y + SIZE_SPACE * (j + 1), SIZE_SQUARE, SIZE_SQUARE, hWnd, NULL, NULL, NULL);
                            controlGame.controlGrid[j][i].status = WATER;
                            break;
                        case 'X':
                            if (!strncmp(username, "admin", strlen("admin")))
                            {
                                controlGame.controlGrid[j][i].hwnd = CreateWindowA("static", "X", BS_CENTER | SS_CENTER | WS_VISIBLE | WS_CHILD, START_GRID_X + SIZE_SPACE * (i + 1), START_GRID_Y + SIZE_SPACE * (j + 1), SIZE_SQUARE, SIZE_SQUARE, hWnd, NULL, NULL, NULL);
                                controlGame.controlGrid[j][i].status = WATER;
                            }
                            else
                            {
                                controlGame.controlGrid[j][i].hwnd = CreateWindowA("button", "", BS_CENTER | SS_CENTER | WS_VISIBLE | WS_CHILD, START_GRID_X + SIZE_SPACE * (i + 1), START_GRID_Y + SIZE_SPACE * (j + 1), SIZE_SQUARE, SIZE_SQUARE, hWnd, id + 1, NULL, NULL);
                                controlGame.controlGrid[j][i].type = BUTTON;
                            }
                            break;
                        case 'T':
                            if (grid[j][i] == '~' || grid[j][i] == 'X' || grid[j][i] == 0)
                                DestroyWindow(controlGame.controlGrid[j][i].hwnd);
                            controlGame.controlGrid[j][i].hwnd = CreateWindowA("static", "T", BS_CENTER | SS_CENTER | WS_VISIBLE | WS_CHILD, START_GRID_X + SIZE_SPACE * (i + 1), START_GRID_Y + SIZE_SPACE * (j + 1), SIZE_SQUARE, SIZE_SQUARE, hWnd, NULL, NULL, NULL);
                            controlGame.controlGrid[j][i].status = BOAT_HIT;
                            break;
                        case 'C':
                            if (grid[j][i] == '~' || grid[j][i] == 'X' || grid[j][i] == 0)
                                DestroyWindow(controlGame.controlGrid[j][i].hwnd);
                            controlGame.controlGrid[j][i].hwnd = CreateWindowA("static", "C", BS_CENTER | SS_CENTER | WS_VISIBLE | WS_CHILD, START_GRID_X + SIZE_SPACE * (i + 1), START_GRID_Y + SIZE_SPACE * (j + 1), SIZE_SQUARE, SIZE_SQUARE, hWnd, NULL, NULL, NULL);
                            controlGame.controlGrid[j][i].status = BOAT_SUNK;
                            break;
                        }

                        id++;
                    }
                }
            }
            for (int i = 0; i < SIZE_GRID; i++)
            {
                for (int j = 0; j < SIZE_GRID; j++)
                {
                    grid[j][i] = ((Packet*)lParam)->game.grid[j][i];
                }
            }
            break;
        case MESSAGE_TYPE_ITS_YOUR_TURN:
            EnableWindow(controlGame.shotButton, TRUE);
            break;
        case MESSAGE_TYPE_YOU_WIN:
            if (!strncmp(username, "admin", strlen("admin")))
            {
                MessageBox(hWnd, L"La partie est terminée", L"Partie terminée", MB_ICONINFORMATION);
                EnableWindow(GetWindow(GetWindow(GetWindow(hWnd, GW_OWNER), GW_OWNER), GW_OWNER), TRUE);
                SetActiveWindow(GetWindow(GetWindow(GetWindow(hWnd, GW_OWNER), GW_OWNER), GW_OWNER));
                DestroyWindow(GetWindow(GetWindow(hWnd, GW_OWNER), GW_OWNER));
                
                break;
            }

            MessageBox(hWnd, L"Bravo, tu as gagné", L"Bravo", MB_ICONINFORMATION);
            GetWindowRect(hWnd, &rect);
            user.hwnd = CreateWindowW(user.className, titleBar, WS_VISIBLE | WS_CAPTION | WS_SYSMENU, rect.left, rect.top, 600, 500, NULL, NULL, NULL, NULL);
            DestroyWindow(hWnd);
            break;
        case MESSAGE_TYPE_YOU_LOSE:
            MessageBox(hWnd, L"Dommage tu as perdu", L"Perdu", MB_ICONINFORMATION);
            GetWindowRect(hWnd, &rect);
            user.hwnd = CreateWindowW(user.className, titleBar, WS_VISIBLE | WS_CAPTION | WS_SYSMENU, rect.left, rect.top, 600, 500, NULL, NULL, NULL, NULL);
            DestroyWindow(hWnd);
            break;
        }
        break;
        

    default:
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }
}