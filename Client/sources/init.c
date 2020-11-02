#include "../headers/client.h"

void init(void)
{
    wcscat(titleBar, L"Bataille Navale ");
    wcscat(titleBar, VERSION);
    port = 7001;
    strcat(server, "localhost");
    memset(username, 0, SIZE_USERNAME);

    login.className = L"loginClass";
    login.wndClass.lpfnWndProc = loginWindowProc;
    login.wndClass.hInstance = hInst;
    login.wndClass.lpszClassName = login.className;
    login.wndClass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
    RegisterClass(&login.wndClass);

    tools.className = L"toolsClass";
    tools.wndClass.lpfnWndProc = toolsWindowProc;
    tools.wndClass.hInstance = hInst;
    tools.wndClass.lpszClassName = tools.className;
    tools.wndClass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
    RegisterClass(&tools.wndClass);

    admin.className = L"adminClass";
    admin.wndClass.lpfnWndProc = adminWindowProc;
    admin.wndClass.hInstance = hInst;
    admin.wndClass.lpszClassName = admin.className;
    admin.wndClass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
    RegisterClass(&admin.wndClass);

    createAccount.className = L"createAccountClass";
    createAccount.wndClass.lpfnWndProc = createAccountWindowProc;
    createAccount.wndClass.hInstance = hInst;
    createAccount.wndClass.lpszClassName = createAccount.className;
    createAccount.wndClass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
    RegisterClass(&createAccount.wndClass);

    deleteAccount.className = L"deleteAccountClass";
    deleteAccount.wndClass.lpfnWndProc = deleteAccountWindowProc;
    deleteAccount.wndClass.hInstance = hInst;
    deleteAccount.wndClass.lpszClassName = deleteAccount.className;
    deleteAccount.wndClass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
    RegisterClass(&deleteAccount.wndClass);

    createGame.className = L"createGameClass";
    createGame.wndClass.lpfnWndProc = createGameWindowProc;
    createGame.wndClass.hInstance = hInst;
    createGame.wndClass.lpszClassName = createGame.className;
    createGame.wndClass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
    RegisterClass(&createGame.wndClass);

    listGame.className = L"listGameClass";
    listGame.wndClass.lpfnWndProc = listGameWindowProc;
    listGame.wndClass.hInstance = hInst;
    listGame.wndClass.lpszClassName = listGame.className;
    listGame.wndClass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
    RegisterClass(&listGame.wndClass);

    manageGame.className = L"manageGameClass";
    manageGame.wndClass.lpfnWndProc = manageGameWindowProc;
    manageGame.wndClass.hInstance = hInst;
    manageGame.wndClass.lpszClassName = manageGame.className;
    manageGame.wndClass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
    RegisterClass(&manageGame.wndClass);

    user.className = L"userClass";
    user.wndClass.lpfnWndProc = userWindowProc;
    user.wndClass.hInstance = hInst;
    user.wndClass.lpszClassName = user.className;
    user.wndClass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
    RegisterClass(&user.wndClass);

    joinGame.className = L"joinGameClass";
    joinGame.wndClass.lpfnWndProc = joinGameWindowProc;
    joinGame.wndClass.hInstance = hInst;
    joinGame.wndClass.lpszClassName = joinGame.className;
    joinGame.wndClass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
    RegisterClass(&joinGame.wndClass);

    waiting.className = L"waitingGameClass";
    waiting.wndClass.lpfnWndProc = waitingWindowProc;
    waiting.wndClass.hInstance = hInst;
    waiting.wndClass.lpszClassName = waiting.className;
    waiting.wndClass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
    RegisterClass(&waiting.wndClass);

    game.className = L"gameClass";
    game.wndClass.lpfnWndProc = gameWindowProc;
    game.wndClass.hInstance = hInst;
    game.wndClass.lpszClassName = game.className;
    game.wndClass.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
    RegisterClass(&game.wndClass);
    //game.hwnd = CreateWindowW(game.className, titleBar, WS_VISIBLE | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, NULL, NULL);

    //char buffer[2048] = { 0 };

    //sprintf(buffer, "%s : %d\r\n", "daniel", 10);
    //SetWindowTextA(scores, buffer);

    //int ndx;
    //ndx = GetWindowTextLengthA(scores);
    //SendMessageA(scores, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
    //SendMessageA(scores, EM_REPLACESEL, 0, (LPARAM)(LPCTSTR)"azeazeaze");

    //SendMessageA(scores, WM_SETTEXT, 0, (LPARAM)"azeazeaz");

    //system("pause");
    login.hwnd = CreateWindowW(login.className, titleBar, WS_VISIBLE | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, NULL, NULL);
}
