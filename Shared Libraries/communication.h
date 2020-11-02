#ifndef NETWORK_H
#define NETWORK_H
/************************************************************/
//DEFINE VERSION
/************************************************************/
#define VERSION L"2.8"
/************************************************************/
//DEFINE SIZES
/************************************************************/
#define SIZE_USERNAME 32
#define SIZE_PASSWORD 32
#define SIZE_GAMENAME 32
#define SIZE_GRID     15
/************************************************************/
//DEFINE MESSAGES TYPES
/************************************************************/
#define MESSAGE_TYPE_LOGIN_REQUEST                  1
#define MESSAGE_TYPE_LOGIN_REPLY_ADMIN_SUCCESS      2
#define MESSAGE_TYPE_LOGIN_REPLY_SUCCESS            3
#define MESSAGE_TYPE_LOGIN_REPLY_BAD_USERNAME       4
#define MESSAGE_TYPE_LOGIN_REPLY_BAD_PASSWORD       5
#define MESSAGE_TYPE_CLOSE_CONNECTION               6
#define MESSAGE_TYPE_ACCOUNT_CREATE_REQUEST         7
#define MESSAGE_TYPE_ACCOUNT_CREATE_ALREADY_EXIST   8
#define MESSAGE_TYPE_ACCOUNT_CREATE_OK              9
#define MESSAGE_TYPE_ACCOUNT_DELETE_REQUEST        10
#define MESSAGE_TYPE_INTERNAL_ERROR                11
#define MESSAGE_TYPE_ACCOUNT_DELETE_SUCCES         12
#define MESSAGE_TYPE_ACCOUNT_DELETE_BAD_USERNAME   13
#define MESSAGE_TYPE_GAME_ENUMERATE_REQUEST        14
#define MESSAGE_TYPE_GAME_CREATE_REQUEST           15
#define MESSAGE_TYPE_GAME_CREATE_ALREADY_EXIST     16
#define MESSAGE_TYPE_GAME_CREATE_SUCCESS           17
#define MESSAGE_TYPE_GAME_NUMBER_REPLY             18
#define MESSAGE_TYPE_GAME_JOIN_REQUEST             19
#define MESSAGE_TYPE_GAME_JOIN_OK                  20
#define MESSAGE_TYPE_GAME_JOIN_FULL                21
#define MESSAGE_TYPE_KEEP_ALIVE                    22
#define MESSAGE_TYPE_GAME_PLAYER_NUMBER_REQUEST    23
#define MESSAGE_TYPE_GAME_PLAYER_EJECT             24
#define MESSAGE_TYPE_GAME_PLAYER_NUMBER_REPLY      25
#define MESSAGE_TYPE_GAME_START_REQUEST            26
#define MESSAGE_TYPE_GAME_START_SUCCESS            27
#define MESSAGE_TYPE_GAME_START_NOT_FULL           28
#define MESSAGE_TYPE_GAME_JOIN_ERROR               29
//#define MESSAGE_TYPE_GAME_START                    30
#define MESSAGE_TYPE_USER_ENUMERATE_REQUEST        31
#define MESSAGE_TYPE_GAME_PLAYER_EJECT_SUCCESS     32
#define MESSAGE_TYPE_GRID_REQUEST                  33
#define MESSAGE_TYPE_SEND_SHOT                     35
//#define MESSAGE_TYPE_GENERATE_GRIDS                36
//#define MESSAGE_TYPE_GENERATE_GRIDS_SUCCESS        37
#define MESSAGE_TYPE_GRID_REPLY                    38
#define MESSAGE_TYPE_PLAYER_IN_GAME_RUNNING        39
#define MESSAGE_TYPE_PLAYER_IN_GAME                40
#define MESSAGE_TYPE_ITS_YOUR_TURN                 42
#define MESSAGE_TYPE_ITS_NOT_YOUR_TURN             43
#define MESSAGE_TYPE_TURN_REQUEST                  44
#define MESSAGE_TYPE_YOU_WIN                       45
#define MESSAGE_TYPE_YOU_LOSE                      46
#define MESSAGE_TYPE_USER_ENUMERATE_REPLY          47
//#define MESSAGE_TYPE_GAME_RUNNING_REQUEST          48
//#define MESSAGE_TYPE_GAME_RUNNING_REPLY            49
#define MESSAGE_TYPE_GAME_SHOW_ADMIN               50
#define MESSAGE_TYPE_SHOW_ADMIN_REPLY              51
#define MESSAGE_TYPE_GRID_ADMIN_REQUEST            52
#define MESSAGE_TYPE_GRID_ADMIN_REPLY              53
#define MESSAGE_TYPE_SCORE                         54
#define MESSAGE_TYPE_FIRST_SCORE                   55
#define MESSAGE_TYPE_GAME_STOP_SHOW_ADMIN          56
/************************************************************/
//DEFINE STRUCTURES
/************************************************************/
/**
 * @brief Structure pour les coordonnées d'une cellule de la grille
 */
typedef struct Point
{
    int x;
    int y;
} Point;
/*----------------------------------------------------------*/
/**
 * @brief Structure pour la partie du paquet liée aux informations du client
 */
typedef struct Packet_Account
{
    char username[SIZE_USERNAME + 1];
    char password[SIZE_PASSWORD + 1];
} Packet_Account;
/*----------------------------------------------------------*/
/**
 * @brief Structure pour la partie du paquet liée aux informations de la partie
 */
typedef struct Packet_Game
{
    char  gamename[SIZE_GAMENAME + 1];
    char  grid[SIZE_GRID][SIZE_GRID];
    int   nbBoats;
    int   nbPlayers;
    int   isRunnig;
    Point point;
    int   score;
} Packet_Game;
/*----------------------------------------------------------*/
/**
 * @brief Structure pour le paquet
 */
typedef struct Packet
{
    int             messageType;
    Packet_Account  account;
    Packet_Game     game;
} Packet;
/*----------------------------------------------------------*/
#endif // NETWORK_H
