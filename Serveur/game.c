#include "includes.h"

/************************************************************************/
/**
 * @brief  Fonction permettant de savoir si une partie existe
 * @param  packet : Paquet reçu par le serveur depuis le client contenant les données
 * @return 0      : Le nom de la game n'existe pas
 * @return 1      : Le nom de la game existe
 */
int game_exist(Packet* packet)
{
    Node* currentNode = NULL;
    currentNode = listGame.head;

    if(packet == NULL)
        return 0;

    while(currentNode != NULL)
    {
        printf("|%s|%s|\n", packet->game.gamename, ((Game*)(currentNode->data))->name);
        if(strncmp(packet->game.gamename, ((Game*)(currentNode->data))->name, SIZE_GAMENAME) == 0)
            return 1;
        currentNode = currentNode->next;
    }

    return 0;
}
/************************************************************************/
/**
 * @brief  Fonction permettant de créer une partie (par l'admin)
 * @param  recipient : Informations envoyées par le client (admin)
 * @return packet    : Le paquet à envoyer au client (admin) avec confirmation (ou non) de la création de la partie
 */
Packet game_create(Packet* recipient)
{
    Packet packet = {0};
    Game   data   = {0};

    if(game_exist(recipient))
        packet.messageType = MESSAGE_TYPE_GAME_CREATE_ALREADY_EXIST;
    else
    {
        add_node(&listGame);
        memcpy(data.name, recipient->game.gamename, SIZE_GAMENAME);
        data.nbPlayers = recipient->game.nbPlayers;
        data.nbBoats = recipient->game.nbBoats;
        init_data_game(listGame.tail, data);
        packet.messageType = MESSAGE_TYPE_GAME_CREATE_SUCCESS;
        random_grid(((Game*)(listGame.tail->data))->grid[0], &((Game*)(listGame.tail->data))->listBoats, ((Game*)(listGame.tail->data))->nbBoats);

    }

    return packet;
}
/************************************************************************/
/**
 * @brief  Fonction INUTILE car compile sans !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * @param  recipient : Informations envoyées par le client (admin ?)
 * @return packet    :
 */
Packet game_number(Packet* recipient)
{
    Packet packet = {0};
    Game   data   = {0};

    if(game_exist(recipient))
        packet.messageType = MESSAGE_TYPE_GAME_CREATE_ALREADY_EXIST;
    else
    {
        add_node(&listGame);
        memcpy(data.name, recipient->game.gamename, SIZE_GAMENAME);
        init_data_game(listGame.tail, data);
        memset(&data, 0, sizeof(Game));
        packet.messageType = MESSAGE_TYPE_GAME_CREATE_SUCCESS;
    }

    return packet;
}
/************************************************************************/
/**
 * @brief  Fonction permettant de rejoindre une partie
 * @param  recipient     : Informations envoyées par le client
 * @param  currentThread : Informations sur le thread du client
 * @return packet        : Le paquet à envoyer au client avec les informations de la partie
 */
Packet game_join(Packet* recipient, Player* currentThread)
{
    Packet packet        = {0};
    Node*  currentGame   = listGame.head;
    Node* currentPlayer  = NULL;
    Player dataPlayer    = {0};

    while(currentGame != NULL)
    {
        if(strncmp(recipient->game.gamename, ((Game*)(currentGame->data))->name, SIZE_GAMENAME) == 0)
        {
            if(((Game*)(currentGame->data))->listPlayer.size < ((Game*)(currentGame->data))->nbPlayers)
            {
                currentPlayer = ((Game*)(currentGame->data))->listPlayer.head;
                while(currentPlayer != NULL)
                {
                    if(strncmp(((Player*)(currentPlayer->data))->username, currentThread->username, SIZE_USERNAME) == 0)
                    {
                        packet.messageType = MESSAGE_TYPE_GAME_JOIN_ERROR;
                        return packet;
                    }
                    currentPlayer = currentPlayer->next;
                }
                add_node(&(((Game*)(currentGame->data))->listPlayer));
                currentThread->game = currentGame;
                currentThread->playerAddr = ((Game*)(currentGame->data))->listPlayer.tail;

                memcpy(dataPlayer.username, currentThread->username, SIZE_USERNAME);
                dataPlayer.game = currentGame;
                dataPlayer.playerAddr = ((Game*)(currentGame->data))->listPlayer.tail;
                dataPlayer.sock = currentThread->sock;
                dataPlayer.score = 0;

                init_data_game_player(((Game*)(currentGame->data))->listPlayer.tail, dataPlayer);

                packet.messageType = MESSAGE_TYPE_GAME_JOIN_OK;
                break;
            }
            else
                packet.messageType = MESSAGE_TYPE_GAME_JOIN_FULL;
            break;
        }
        currentGame = currentGame->next;
    }
    return packet;
}
/************************************************************************/
/**
 * @brief  Fonction permettant de savoir si un joueur est en train de jouer
 * @param  username      : Nom d'utilisateur
 * @return playerFound   : Les informations sur le joueur et la partie s'il a été trouvé dasn une partie, null sinon
 */
Player is_in_game(char* username)
{
    Node*  currentGame   = NULL;
    Node*  currentPlayer = NULL;
    Player playerFound   = {0};

    playerFound.game = NULL;
    playerFound.playerAddr = NULL;
    playerFound.sock = 0;

    currentGame = listGame.head;
    while(currentGame != NULL)
    {
        currentPlayer = ((Game*)(currentGame->data))->listPlayer.head;
        while(currentPlayer != NULL)
        {
            if(strncmp(username, ((Player*)(currentPlayer->data))->username, SIZE_USERNAME) == 0)
            {
                memcpy(playerFound.username, ((Player*)(currentPlayer->data))->username, SIZE_USERNAME);
                playerFound.game       = currentGame;
                playerFound.playerAddr = currentPlayer;
                playerFound.sock       = 0;
                return playerFound;
            }
            currentPlayer = currentPlayer->next;
        }
        currentGame = currentGame->next;
    }

    return playerFound;
}
/************************************************************************/
/**
 * @brief  Fonction permettant d'envoyer la liste des parties au client
 * @param  socket : Socket entre le serveur et le client
 * @return void   : Ne retourne rien, mais envoie les noms des parties au client
 */
void send_game(int socket)
{
    Packet packet = {0};
    Node* currentGame = NULL;

    packet.messageType = MESSAGE_TYPE_GAME_NUMBER_REPLY;

    currentGame = listGame.head;
    while(currentGame != NULL)
    {
        memcpy(packet.game.gamename, ((Game*)(currentGame->data))->name, SIZE_GAMENAME);
        currentGame = currentGame->next;
        write_client(socket, packet);
    }
}
/************************************************************************/
/**
 * @brief  Fonction permettant d'envoyer la liste des joueurs au client
 * @param  recipient : Informations envoyées par le client
 * @param  socket    : Socket entre le serveur et le client
 * @return void      : Ne retourne rien, mais envoie les noms des joueurs au client
 */
void send_game_player(Packet* recipient, int socket)
{
    Packet packet        = {0};
    Node*  currentGame   = listGame.head;
    Node*  currentPlayer = NULL;

    while(currentGame != NULL)
    {
        if(strncmp(((Game*)(currentGame->data))->name, recipient->game.gamename, SIZE_GAMENAME) == 0)
        {
            currentPlayer = ((Game*)(currentGame->data))->listPlayer.head;
            while(currentPlayer != NULL)
            {
                packet.messageType = MESSAGE_TYPE_GAME_PLAYER_NUMBER_REPLY;
                memcpy(packet.account.username, ((Player*)(currentPlayer->data))->username, SIZE_USERNAME);
                currentPlayer = currentPlayer->next;
                write_client(socket, packet);
                memset(&packet.account.username, 0, SIZE_USERNAME + 1);
            }

            break;
        }
        currentGame = currentGame->next;
    }
}
/************************************************************************/
/**
 * @brief  Fonction permettant d'éjecter un joueur d'une partie
 * @param  recipient : Informations envoyées par le client
 * @return packet    : Retourne un paquet qui confirme l'éjection du joueur
 */
Packet eject_player(Packet* recipient)
{
    Packet packet = {0};
    Node*  currentGame   = listGame.head;
    Node*  currentPlayer = NULL;

    while(currentGame != NULL)
    {
        if(!strncmp(recipient->game.gamename, ((Game*)(currentGame->data))->name, SIZE_GAMENAME))
        {
            if( ((Game*)(currentGame->data))->isRunning != 0)
                return packet;

            currentPlayer = ((Game*)(currentGame->data))->listPlayer.head;
            while(currentPlayer != NULL)
            {
                if(strncmp(((Player*)(currentPlayer->data))->username, recipient->account.username, SIZE_USERNAME) == 0)
                {
                    if(((Player*)(currentPlayer->data))->sock != 0)
                    {
                        packet.messageType = MESSAGE_TYPE_GAME_PLAYER_EJECT;
                        write_client(((Player*)(currentPlayer->data))->sock, packet);
                    }
                    del_node(&(((Game*)(currentGame->data))->listPlayer), index_of_node(((Game*)(currentGame->data))->listPlayer.head, currentPlayer));
                    memset(&packet, 0, sizeof(Packet));

                    packet.messageType = MESSAGE_TYPE_GAME_PLAYER_EJECT_SUCCESS;
                    return packet;
                }
                currentPlayer = currentPlayer->next;
            }
            break;
        }
        currentGame = currentGame->next;
    }
    packet.messageType = MESSAGE_TYPE_INTERNAL_ERROR;
    return packet;
}

/************************************************************************/
/**
 * @brief  Fonction permettant d'initialiser le début de la partie lorsqu'on la lance
 * @param  recipient : Informations envoyées par le client
 * @return packet    : Retourne un paquet qui confirme (ou non) le bon lancement de la partie
 */
Packet init_game(Packet* recipient)
{
    Node* currentGame   = listGame.head;
    Node* currentPlayer = NULL;
    Packet packet       = {0};

    if(currentGame == NULL)
    {
        packet.messageType = MESSAGE_TYPE_INTERNAL_ERROR;
        return packet;
    }

    while(currentGame != NULL)
    {
        if(!strncmp(recipient->game.gamename, ((Game*)(currentGame->data))->name, SIZE_GAMENAME))
        {
            if(((Game*)(currentGame->data))->isRunning == 1)
                return packet;

            if(((Game*)(currentGame->data))->listPlayer.size == ((Game*)(currentGame->data))->nbPlayers)
            {
                ((Game*)(currentGame->data))->isRunning = 1;
                ((Game*)(currentGame->data))->turn = 0;
                packet.messageType = MESSAGE_TYPE_GAME_START_SUCCESS;
                currentPlayer = ((Game*)(currentGame->data))->listPlayer.head;
                while(currentPlayer != NULL)
                {
                    write_client(((Player*)(currentPlayer->data))->sock, packet);
                    currentPlayer = currentPlayer->next;
                }
                break;
            }
            packet.messageType = MESSAGE_TYPE_GAME_START_NOT_FULL;
            break;
        }
        currentGame = currentGame->next;
    }
    return packet;
}
/************************************************************************/
/**
 * @brief  Fonction permettant d'envoyer la grille de jeu à un joueur
 * @param  currentThread : Thread actif du joueur avec les informations liées au joueur
 * @return packet        : Retourne un paquet qui contient la grille de jeu
 */
Packet send_grid(Player* currentThread)
{
    Packet  packet = {0};

    if(currentThread->game == NULL)
        return packet;

    for(int i = 0 ; i < SIZE_GRID ; i++)
    {
        for(int j = 0 ; j < SIZE_GRID ; j++)
        {
            packet.game.grid[i][j] = ((Game*)(currentThread->game->data))->gridShots[i][j];
        }
    }

    packet.messageType = MESSAGE_TYPE_GRID_REPLY;

    return packet;
}
/************************************************************************/
/**
 * @brief  Fonction permettant d'envoyer la grille de jeu à l'admin
 * @param  currentThread : Thread actif de l'admin avec les informations liées à l'admin
 * @param  gamename      : Nom de la partie que l'admin veut (ou est en train) de regarder
 * @return packet        : Retourne un paquet qui contient la grille de jeu
 */
Packet send_grid_admin(Thread* currentThread, char* gamename)
{
    Packet  packet = {0};

    for(Node* currentGame = listGame.head ; currentGame != NULL ; currentGame = currentGame->next)
    {
        if(!strncmp(((Game*)(currentGame->data))->name, gamename, SIZE_GAMENAME))
        {
            if(((Game*)(currentGame->data))->isRunning != 1)
                return packet;


            for(int i = 0 ; i < SIZE_GRID ; i++)
            {
                for(int j = 0 ; j < SIZE_GRID ; j++)
                {
                    packet.game.grid[i][j] = ((Game*)(currentGame->data))->grid[i][j];
                }
            }
        }
    }

    packet.messageType = MESSAGE_TYPE_GRID_ADMIN_REPLY;

    return packet;
}
/************************************************************************/
/**
 * @brief  Fonction permettant de faire en sorte que l'admin puisse être spectateur d'une partie
 * @param  currentThread : Thread actif de l'admin avec les informations liées à l'admin
 * @param  currentThread : Nom de la partie que l'admin veut de regarder
 * @return packet        : Retourne un paquet qui confirme que l'admin peut visualiser la partie
 */
Packet send_game_show_admin(Thread* currentThread, char* gamename)
{
    Packet  packet = {0};

    for(Node* currentGame = listGame.head ; currentGame != NULL ; currentGame = currentGame->next)
    {
        if(!strncmp(((Game*)(currentGame->data))->name, gamename, SIZE_GAMENAME))
        {
            if(((Game*)(currentGame->data))->isRunning != 1)
                return packet;

            packet.messageType = MESSAGE_TYPE_SHOW_ADMIN_REPLY;

            ((Game*)(currentGame->data))->threadAdmin = currentThread;
            break;
        }
    }

    return packet;
}
/************************************************************************/
/**
 * @brief  Fonction permettant d'envoyer la grille de jeu à tous les clients
 * @param  currentThread : Thread actif du client avec les informations liées au client
 * @return void          : Ne retourne rien, mais envoie la grille de jeu au client
 */
void send_grid_all(Player* currentThread)
{
    Packet  packet = {0};

    for(int i = 0 ; i < SIZE_GRID ; i++)
    {
        for(int j = 0 ; j < SIZE_GRID ; j++)
        {
            packet.game.grid[i][j] = ((Game*)(currentThread->game->data))->gridShots[i][j];
        }
    }

    packet.messageType = MESSAGE_TYPE_GRID_REPLY;

    for(Node* player = listThread.head ; player != NULL ; player = player->next)
        write_client(((Player*)player->data)->sock, packet);

    if(((Game*)(((Node*)(currentThread->game))->data))->threadAdmin != NULL)
        send_grid_admin(((Game*)(((Node*)(currentThread->game))->data))->threadAdmin, ((Game*)(currentThread->game->data))->name);
}
/************************************************************************/
/**
 * @brief  Fonction permettant de dire au joueur si c'est à son tour de jouer ou non
 * @param  currentThread : Thread actif du joueur avec les informations liées au joueur
 * @return void          : Ne retourne rien, mais envoie au joueur si c'est son tour ou non
 */
void send_turn(Player* currentThread)
{
    Packet packet1 = {0};
    Packet packet2 = {0};

    //if(currentThread->playerAddr->next != NULL)

        //Tour au next
    //else
        //Tour au premier currentThread->game)
    packet1.messageType = MESSAGE_TYPE_ITS_YOUR_TURN;
    packet2.messageType = MESSAGE_TYPE_ITS_NOT_YOUR_TURN;

    Node* playerTurn = (Node*) node_of_index(((Game*)(currentThread->game->data))->listPlayer.head, ((Game*)(currentThread->game->data))->turn);

    printf("send_turn()\n");
    printf("Adresse du joueur dont c'est le tour : %p\n", playerTurn);
    printf("Adresse de %s : %p \n", currentThread->username, currentThread->playerAddr);



    if(playerTurn == currentThread->playerAddr)
    {
        printf("Envoi du paquet 1 ! \n");
        send(currentThread->sock, (char*)&packet1, sizeof(Packet), 0);
    }
    else
    {
        send(((Player*)(node_of_index(((Game*)(currentThread->game->data))->listPlayer.head, ((Game*)(currentThread->game->data))->turn)->data))->sock, (char*)&packet1, sizeof(Packet), 0);
        send(currentThread->sock, (char*)&packet2, sizeof(Packet), 0);
    }
}
/************************************************************************/
/**
 * @brief  Fonction permettant de noter qui va être le prochain joueur à jouer
 * @param  game : Partie en cours avec toutes les informations qu'elle contient
 * @return void : Ne retourne rien
 */
void set_turn(Node* game)
{
    printf("%d = %d\n", ((Game*)(game->data))->turn, index_of_node(((Game*)(game->data))->listPlayer.head, ((Game*)(game->data))->listPlayer.tail));
    if (((Game*)(game->data))->turn == index_of_node(((Game*)(game->data))->listPlayer.head, ((Game*)(game->data))->listPlayer.tail))
    {
        ((Game*)(game->data))->turn = 0;
    }
    else
    {
        ((Game*)(game->data))->turn++;
    }
}
/************************************************************************/
/**
 * @brief  Fonction permettant de mettre à jour la grille de jeu (bateau touché ou coulé et coups dans l'eau)
 * @param  recipient     : Informations envoyées par le client (dans un paquet)
 * @param  currentThread : Thread actif du joueur avec les informations liées au joueur
 * @return void          : Ne retourne rien
 */
void update_grid(Packet* recipient, Player* currentThread)
{
    Game* game = (Game*)(currentThread->game->data);

    //ATTENTION ! La grille est inversée, c'est pour ça qu'on stocke point.x dans y et point.y dans x
    int x       = recipient->game.point.x;
    int y       = recipient->game.point.y;

    printf("Joueur %s tire sur : x = %d ; y = %d !\n", currentThread->username, x, y);
    //Si la case visée est un bateau
    if (isdigit(game->grid[y][x]))
    {
        int numBoat = game->grid[y][x] - '0';
        int nbHit = 0; // Compteur pour le nombre de fois qu'un bateau a été touché
        Boat* targetedBoat = (Boat*) (node_of_index(game->listBoats.head, numBoat - 1)->data);
        int xBoat = targetedBoat->x;
        int yBoat = targetedBoat->y;

        //On met T (pour touché) dans la case visée
        game->grid[y][x] = 'T';
        game->gridShots[y][x] = 'T';
        printf("size = %d | x = %d | y = %d\n", targetedBoat->length, targetedBoat->x, targetedBoat->y);

        //On augmente le score du joueur quand il touche un bateau
        ((Player*)(currentThread->playerAddr->data))->score++;
        printf("Le score du joueur  %s a augmenté à %d points \n", currentThread->username, ((Player*)(currentThread->playerAddr->data))->score);

        //On check le bateau pour savoir si toutes ses cases sont T ou non
        for (int i = 0 ; i < targetedBoat->length ; i++)
        {
            switch (targetedBoat->direction)
            {
            case NORTH:
                yBoat = targetedBoat->y - i; // Le bateau pointe vers le haut de la grille
                break;
            case SOUTH:
                yBoat = targetedBoat->y + i; // Le bateau pointe vers le bas de la grille
                break;
            case EAST:
                xBoat = targetedBoat->x + i; // Le bateau pointe vers la droite de la grille
                break;
            case WEST:
                xBoat = targetedBoat->x - i; // Le bateau pointe vers la gauche de la grille
                break;
            default:
                printf("Erreur lors du ciblage du bateau : celui-ci n'a pas de direction !");
                break;
            }
            if (game->grid[yBoat][xBoat] == 'T')
                nbHit++;
       }

        xBoat = targetedBoat->x;
        yBoat = targetedBoat->y;

        //Si toutes les cases sont T, on les passe à C (coulé)
        if (nbHit == targetedBoat->length)
        {
            //enemy->nbBoatsKilled++;
            for (int i = 0; i < targetedBoat->length; i++)
            {
                switch (targetedBoat->direction)
                {
                case NORTH:
                    yBoat = targetedBoat->y - i; // Le bateau pointe vers le haut de la grille
                        break;
                case SOUTH:
                    yBoat = targetedBoat->y + i; // Le bateau pointe vers le bas de la grille
                    break;
                case EAST:
                    xBoat = targetedBoat->x + i; // Le bateau pointe vers la droite de la grille
                    break;
                case WEST:
                    xBoat = targetedBoat->x - i; // Le bateau pointe vers la gauche de la grille
                    break;
                default:
                    printf("Erreur lors du ciblage du bateau : celui-ci n'a pas de direction !");
                    break;
               }
                game->grid[yBoat][xBoat] = 'C';
                game->gridShots[yBoat][xBoat] = 'C';
            }
            //Si tous les bateaux sont coulés
        }
    }
    //Sinon, si la case visée est de l'eau on met un X dans les grilles
    else if(game->grid[y][x] == '~' || game->grid[y][x] == 'X')
    {
        game->grid[y][x] = 'X';
        game->gridShots[y][x] = 'X';
    }
}
/************************************************************************/
/**
 * @brief  Fonction permettant de savoir si la partie est terminée ou non et, si elle est finie,
 *         de calculer les scores et envoyer aux joueurs s'ils ont gagné ou perdu
 * @param  currentThread : Thread actif du joueur avec les informations liées au joueur
 * @return 0             : La partie n'est pas encore terminée
 * @return 1             : La partie est finie
 */
int check_send_game_finish(Player* currentThread)
{
    printf("check_send_game_finish() \n");

    Packet packetWin    = {0};
    Packet packetLose   = {0};

    packetWin.messageType  = MESSAGE_TYPE_YOU_WIN;
    packetLose.messageType = MESSAGE_TYPE_YOU_LOSE;

    Game* game = (Game*)(currentThread->game->data);

    for (int i = 0; i < SIZE_GRID; i++)
        for (int j = 0; j < SIZE_GRID; j++)
            // Lors du parcours du tableau, on retourne faux si on croise un caractère qui n'est pas ~ (eau), X (tir dans l'eau), ou C (bateau coulé)
            if (!(game->grid[i][j] == '~' || game->grid[i][j] == 'X' || game->grid[i][j] == 'C'))
            {
                printf("Partie pas encore terminée ... \n");
                return 0;
            }

    // Si tout le tableau a été parcouru sans encombre (donc que la partie est finie)
    printf("Partie terminée ! \nCalcul des scores et du/des vainqueur(s) en cours ... \n");

    Node* currentPlayer = game->listPlayer.head;
    int best_score = 0;

    // Récupération du score meilleur score pour détermination du/des vainqueur(s) (possibilité de plusieurs si ex-aequo)
    while(currentPlayer != NULL)
    {
        if(((Player*)(currentPlayer->data))->score > best_score)
        {
            best_score = ((Player*)(currentPlayer->data))->score;
            printf("Meilleur score : %d points \n", best_score);
        }
        currentPlayer = currentPlayer->next;
    }



    if(game->threadAdmin != NULL)
        write_client(game->threadAdmin->sock, packetWin);

    currentPlayer = game->listPlayer.head;
    // Envoi aux joueurs du message gagné ou perdu

    while(currentPlayer != NULL)
    {
        if(((Player*)(currentPlayer->data))->score == best_score)
        {
            write_client(((Player*)(currentPlayer->data))->sock, packetWin);
            printf("Victoire de %s avec %d points \n", ((Player*)(currentPlayer->data))->username, ((Player*)(currentPlayer->data))->score);
        }
        else
        {
            write_client(((Player*)(currentPlayer->data))->sock, packetLose);
            printf("Défaite de %s avec %d points \n", ((Player*)(currentPlayer->data))->username, ((Player*)(currentPlayer->data))->score);
        }
        currentPlayer = currentPlayer->next;
    }

    // On retourne vrai (la partie est finie)
    return 1;
}
/************************************************************************/
/**
 * @brief  Fonction permettant de mettre à jour les scores des joueurs sur les terminaux des clients
 * @param  currentThread : Thread actif du joueur avec les informations liées au joueur
 * @param  gamename      : Utilisé dans le cas où l'admin regarde la partie, permet de savoir quelle partie il regarde
 * @return void          : Ne retourne rien, mais envoie aux clients liés à la partie (joueurs + admin si spectateur) les scores à jour
 */
void send_scores(Thread* currentThread, char* gamename)
{
    // Fonction permettant la mise à jour des scores des joueurs
    // Il y aura autant de paquets que de joueur envoyé à UN joueur (pour mettre à jour les scores de tous les joueurs)
    Packet packetScore = {0};

    Game* game;

    // Si c'est l'admin qui fait la requête, on doit d'abord chercher la partie dans la liste
    if(gamename != NULL)
    {
        for(Node* currentGame = listGame.head ; currentGame != NULL ; currentGame = currentGame->next)
        {
            if(!strncmp(((Game*)currentGame->data)->name, gamename, SIZE_GAMENAME))
            {
                if(((Game*)(currentGame->data))->isRunning != 1)
                    return;

                game = (Game*)currentGame->data;
            }
        }
    }
    // Sinon, c'est que c'est le joueur qui fait la requête et la partie est stockée dans son thread
    else
    {
        game = (Game*)currentThread->game->data;
    }


    if(game->threadAdmin != NULL)
    {
        packetScore.messageType = MESSAGE_TYPE_FIRST_SCORE;

        for(Node* currentPlayer = game->listPlayer.head ; currentPlayer != NULL ; currentPlayer = currentPlayer->next)
        {
            // On ajoute le username du joueur dans la partie account pour savoir à qui est associé le score
            strncpy(packetScore.account.username, ((Player*)(currentPlayer->data))->username, SIZE_USERNAME);

            // On ajoute le score du joueur dans la partie game
            packetScore.game.score = ((Player*)(currentPlayer->data))->score;


            write_client(game->threadAdmin->sock, packetScore);
            packetScore.messageType = MESSAGE_TYPE_SCORE;
        }
    }

    // On parcours la liste des joueurs de la partie et pour chaque joueur :
    for(Node* currentPlayer = game->listPlayer.head ; currentPlayer != NULL ; currentPlayer = currentPlayer->next)
    {
        packetScore.messageType = MESSAGE_TYPE_FIRST_SCORE;

        for(Node* currentPlayerScore = game->listPlayer.head ; currentPlayerScore != NULL ; currentPlayerScore = currentPlayerScore->next)
        {
            // On ajoute le username du joueur dans la partie account pour savoir à qui est associé le score
            strncpy(packetScore.account.username, ((Player*)(currentPlayerScore->data))->username, SIZE_USERNAME);

            // On ajoute le score du joueur dans la partie game
            packetScore.game.score = ((Player*)(currentPlayerScore->data))->score;


            write_client(((Player*)(currentPlayer->data))->sock, packetScore);
            packetScore.messageType = MESSAGE_TYPE_SCORE;
        }
    }
}
/************************************************************************/
