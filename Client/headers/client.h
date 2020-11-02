#ifndef CLIENT_H
#define CLIENT_H

#pragma warning(disable : 4996)
#pragma warning(disable : 4133)
#pragma warning(disable : 6276)
#pragma warning(disable : 4715)
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib, "comctl32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <CommCtrl.h>
#include <io.h>
#include <wchar.h>
#include <string.h>
#include "../../Shared Libraries/communication.h"
#include "../resources/resource.h"


#define ID_BUTTON_OK			 1
#define ID_MENU_TOOLS			 1
#define ID_BUTTON_CONNECT        2
#define ID_BUTTON_DISCONNECT     3
#define ID_BUTTON_ACCOUNT_CREATE 4
#define ID_BUTTON_ACCOUNT_DELETE 5
#define ID_BUTTON_GAME_CREATE	 6
#define ID_BUTTON_GAME_LIST		 7
#define ID_BUTTON_GAME_MANAGE	 8
#define ID_BUTTON_GAME_JOIN		 9
#define ID_BUTTON_EJECT_PLAYER  10
#define ID_BUTTON_START_GAME    11
#define ID_WAITING_FROM_SERVER  12
#define ID_BUTTON_GAME_SHOW     13
#define ID_BUTTON_SHOT        1000


#define SIZE_SQUARE             20
#define SIZE_SPACE              25 //SIZE_SQUARE + 5
#define START_GRID_X           190
#define START_GRID_Y            75

LRESULT CALLBACK loginWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK toolsWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK adminWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK userWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK createAccountWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK deleteAccountWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK createGameWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK listGameWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK joinGameWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK manageGameWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK waitingWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK gameWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void init(void);
int initNetwork(void);
void endNetwork(int socket);
void close_connection(void);
void request_reply_server(Packet* packet);
Packet read_server(int socket);
void write_server(Packet* packet);
void recv_from_server_thread_proc(int socket);
void keep_alive_thread_proc(int socket);
Packet connect_server(HWND hWnd);

HINSTANCE hInst;


typedef struct Window
{
	WNDCLASSW wndClass;
	LPWSTR    className;
	HWND      hwnd;
} Window;

typedef enum Type
{
	STATIC,
	BUTTON
} Type;
typedef enum Status
{
	WATER,
	BOAT,
	BOAT_HIT,
	BOAT_SUNK
} Status;

typedef struct Control
{
	HWND   hwnd;
	Type   type;
	Point  point;
	Status status;
	HMENU  id;
	char c;
} Control;

typedef struct ControlGame
{
	HWND    gameWindow;
	HWND    shotButton;
	HWND    staticLetter;
	HWND    staticNumber;
	Control controlGrid[SIZE_GRID][SIZE_GRID];
} ControlGame;


Window login;
Window tools;
Window admin;
Window createAccount;
Window deleteAccount;
Window createGame;
Window listGame;
Window manageGame;
Window user;
Window joinGame;
Window waiting;
Window game;

HWND listboxDeleteAccount;
HWND listboxGame;
HWND listboxPlayers;
HWND scores;
HWND hwndServer;
HWND hwndPort;
HMENU hmenuTools;




HWND hwndUsername;
HWND hwndPassword;



char server[2048];
char username[SIZE_USERNAME + 1];
char gamename[SIZE_GAMENAME + 1];
int port;
int sock;
HANDLE threadKeepAlive;
HANDLE threadRecvFromServer;
WCHAR titleBar[1024];

char grid[SIZE_GRID][SIZE_GRID];
Point prevShot;
#endif //CLIENT_H