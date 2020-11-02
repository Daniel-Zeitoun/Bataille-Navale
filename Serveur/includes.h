#ifndef INCLUDES_H
#define INCLUDES_H
/*----------------------------------------------------------*/
#ifdef WIN32
#include <windows.h>
#include <io.h>
#endif // WIN32
/*----------------------------------------------------------*/
#ifdef linux
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#endif // linux
/**********************************************************/
//INCLUDES
/**********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <ctype.h>
#include <pthread.h>
#include "../Shared Libraries/communication.h"
/**********************************************************/
//DEFINES
/********************************************************/
#define NBELEM(val) (sizeof(val)/sizeof*(val))
#define randomize() srand((unsigned)time(NULL))
#define random(val) (int)(rand()/(double)RAND_MAX * (val))

#define PORT 7001
#define SIZE_ID     32
#define SIZE_PASS   32
#define SEPARATOR   1
#define NULL_BYTE   0
#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "ESGI"
#define USERS_FILE     "users.data"

/**********************************************************/
//STRUCTURES
/**********************************************************/
/**
 * @brief Structure pour les noeuds d'une liste
 */
typedef struct Node
{
    struct Node* prev;
    struct Node* next;
    void*        data;
} Node;
/*----------------------------------------------------------*/
/**
 * @brief Structure pour les listes
 */
typedef struct List
{
    struct Node* head;
    struct Node* tail;
    int          size;
} List;
/*----------------------------------------------------------*/
/**
 * @brief Structure pour les informations qu'un joueur possèdent
 */
typedef struct Player
{
    int   sock;
    char  username[SIZE_USERNAME + 1];
    Node* game;
    Node* playerAddr;
    int   score;
} Thread, Player;
/*----------------------------------------------------------*/
/**
 * @brief Structure pour les informations qu'une partie possèdent
 */
typedef struct Game
{
    char name[SIZE_GAMENAME + 1];
    int  nbPlayers;
    int  nbBoats;
    int  isRunning;
    int  turn;
    List listPlayer;
    List listBoats;
    char  grid[SIZE_GRID][SIZE_GRID];
    char  gridShots[SIZE_GRID][SIZE_GRID];
    Thread* threadAdmin;
} Game;
/*----------------------------------------------------------*/
/**
 * @brief Direction que peut prendre un bateau
 */
typedef enum
{
    NORTH,
    SOUTH,
    EAST,
    WEST,
    DIRECTION_NB,
    UNDEFINED
} Direction;
/*----------------------------------------------------------*/
/**
 * @brief Etat que peuvent prendre les cellules de la grille de jeu lors de la génération aléatoire uniquement
 */
typedef enum
{
    WATER,
    BOAT,
    SEEN,
    STATUS_NB
} Status;
/*----------------------------------------------------------*/
/**
 * @brief Structure pour les cellules de la grille de jeu lors de la génération aléatoire uniquement
 */
typedef struct Element
{
    Status status;
    char id;
} Element;
/*----------------------------------------------------------*/
/**
 * @brief Structure pour la grille de jeu lors de la génération aléatoire uniquement
 */
typedef struct Play
{
    struct Element grid[SIZE_GRID][SIZE_GRID];
} Play;
/*----------------------------------------------------------*/
/**
 * @brief Structure pour les informations que possèdent un bateau
 */
typedef struct Boat
{
    int x;
    int y;
    Direction direction;
    int length;
    int id;
} Boat;
/*----------------------------------------------------------*/
// Variables globales utiles à l'ensemble du programme
List listThread;
List listGame;
Thread threadAdmin;

// Pour gérer les sections critiques
#ifdef WIN32
CRITICAL_SECTION lockNetwork;
#endif // WIN32

#ifdef linux
pthread_mutex_t lockNetwork;
#endif // Linux

/**********************************************************/
//FONCTIONS
/**********************************************************/
/*----------------------------------------------------------*/
/**
 * account.c
 */
int create_user(Packet* packet);
int delete_user(char* username);
Packet account_create(Packet* recipient);
Packet account_delete(Packet* recipient);
void send_user(int socket);
/*----------------------------------------------------------*/
/**
 * clientCallback.c
 */
void clientCallback(void* arg);
/*----------------------------------------------------------*/
/**
 * game.c
 */
int game_exist(Packet* packet);
Packet game_create(Packet* recipient);
Packet game_join(Packet* recipient, Player* currentThread);
Player is_in_game(char* username);
void send_game(int socket);
void send_game_player(Packet* recipient, int socket);
Packet eject_player(Packet* recipient);
Packet init_game(Packet* recipient);
Packet send_grid(Player* currentThread);
Packet send_grid_admin(Thread* currentThread, char* gamename);
Packet send_game_show_admin(Thread* currentThread, char* gamename);
void send_grid_all(Player* currentThread);
void send_turn(Player* currentThread);
void set_turn(Node* game);
void update_grid(Packet* recipient, Player* currentThread);
int check_send_game_finish(Player* currentThread);
void send_scores(Thread* currentThread, char* gamename);
/*----------------------------------------------------------*/
/**
 * list.c
 */
Node* add_node(List* list);
void init_data_thread(Node* node, Thread data);
void init_data_game(Node* node, Game data);
void init_data_game_player(Node* node, Thread data);
void init_data_boat(Node* node, Boat data);
void print_list_thread(Node* node);
void del_node(List* list, int index);
int index_of_node(Node* head, Node* node);
Node* node_of_index(Node* head, int index);
/*----------------------------------------------------------*/
/**
 * login.c
 */
int is_admin(char* username);
int is_admin_good_password(char* password);
int is_good_password(Packet_Account* credentials);
int user_exist(char* username);
Packet login(Packet* recipient, Thread* currentThread);
/*----------------------------------------------------------*/
/**
 * randomGrid.c
 */
void grid_init(Play* play);
void print_play(Play* play);
void copy_play_grid(Play* play, char* grid);
int check_grid(Play* play, Boat* boat, Boat pos);
void put_boat(Play* play, Boat* boat);
void mark_ns(Play* play, Direction direction, int x0, int xlen, int y0, int ylen);
void mark_ew(Play* play, Direction direction, int x0, int xlen, int y0, int ylen);
void mark_boat(Play* play, Boat* boat);
void placement(Play* play, Boat* boat);
int random_grid(char* grid, List* listBoat, int nbBoats);
/*----------------------------------------------------------*/
/** trop de temps. Je voulais rajouter des trucs pour obfusqué.
 * server.c
 */
void send_internal_error(int sock);
Packet read_client(int socket);
void write_client(int socket, Packet packet);


#endif // INCLUDES_H
