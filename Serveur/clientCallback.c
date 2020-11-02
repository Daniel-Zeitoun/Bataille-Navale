#include "includes.h"

/**
 * @brief  Fonction appelée pour les thread des joueurs et active tant que le thread existe
 *         Réception des paquets et, selon le type du message, appelle les actions nécessaires
 * @param  arg  : Informations liées au thread de l'utilisateur
 * @return void : Cette fonction ne retourne rien, mais elle appelera les actions nécessaires qui retourneront (ou non) quelque chose
 */
void clientCallback(void* arg)
{
    Node*   currentNode   = (Node*)arg;
    Thread* currentThread = ((Thread*)((Node*)arg)->data);
    Packet  recipient     = {0};
    Packet  packet        = {0};

    while(1)
    {


        memset(&packet, 0, sizeof(Packet));

        recipient = read_client(currentThread->sock);


        #ifdef linux
        pthread_mutex_lock(&lockNetwork);
        #endif // linux

        #ifdef WIN32
        EnterCriticalSection(&lockNetwork);
        #endif // WIN32



        switch(recipient.messageType)
        {
            case MESSAGE_TYPE_CLOSE_CONNECTION:
                close(currentThread->sock);

                del_node(&listThread, index_of_node(listThread.head, currentNode));
                print_list_thread(listThread.head);

                #ifdef linux
                pthread_mutex_unlock(&lockNetwork);
                #endif // linux

                #ifdef WIN32
                LeaveCriticalSection(&lockNetwork);
                #endif // WIN32

                pthread_exit(0);
                break;

            case MESSAGE_TYPE_KEEP_ALIVE:
                //Le client est toujours connecté
                break;
            case MESSAGE_TYPE_LOGIN_REQUEST:
                strncpy(currentThread->username, recipient.account.username, SIZE_USERNAME);
                packet = login(&recipient, currentThread);
                write_client(currentThread->sock, packet);
                break;
            case MESSAGE_TYPE_ACCOUNT_CREATE_REQUEST:
                packet = account_create(&recipient);
                write_client(currentThread->sock, packet);
                break;
            case MESSAGE_TYPE_ACCOUNT_DELETE_REQUEST:
                packet = account_delete(&recipient);
                write_client(currentThread->sock, packet);
                break;
            case MESSAGE_TYPE_USER_ENUMERATE_REQUEST:
                send_user(currentThread->sock);
                break;
            case MESSAGE_TYPE_GAME_CREATE_REQUEST:
                packet = game_create(&recipient);
                write_client(currentThread->sock, packet);
                break;
            case MESSAGE_TYPE_GAME_ENUMERATE_REQUEST:
                send_game(currentThread->sock);
                break;
            case MESSAGE_TYPE_GAME_JOIN_REQUEST:
                packet = game_join(&recipient, currentThread);
                write_client(currentThread->sock, packet);
                break;
            case MESSAGE_TYPE_GAME_PLAYER_NUMBER_REQUEST:
                send_game_player(&recipient, currentThread->sock);
                break;
            case MESSAGE_TYPE_GAME_PLAYER_EJECT:
                packet = eject_player(&recipient);
                write_client(currentThread->sock, packet);
                break;
            case MESSAGE_TYPE_GAME_START_REQUEST:
                packet = init_game(&recipient);
                write_client(currentThread->sock, packet);
                break;
            case MESSAGE_TYPE_GRID_REQUEST:
                send_scores(currentThread, NULL);
                packet = send_grid(currentThread);
                write_client(currentThread->sock, packet);
                break;
            case MESSAGE_TYPE_GAME_SHOW_ADMIN:
                packet = send_game_show_admin(currentThread, recipient.game.gamename);
                write_client(currentThread->sock, packet);
                break;
            case MESSAGE_TYPE_GAME_STOP_SHOW_ADMIN:
                for(Node* currentGame = listGame.head ; currentGame != NULL ; currentGame = currentGame->next)
                {
                    if(!strncmp(((Game*)(currentGame->data))->name, recipient.game.gamename, SIZE_GAMENAME))
                    {
                        ((Game*)(currentGame->data))->threadAdmin = NULL;
                    }
                }
                break;
            case MESSAGE_TYPE_GRID_ADMIN_REQUEST:
                packet = send_grid_admin(currentThread, recipient.game.gamename);
                write_client(currentThread->sock, packet);
                send_scores(currentThread, recipient.game.gamename);
                break;
            case MESSAGE_TYPE_TURN_REQUEST:
                send_turn(currentThread);
                break;
            case MESSAGE_TYPE_SEND_SHOT:
                update_grid(&recipient, currentThread);
                send_grid_all(currentThread);
                send_scores(currentThread, NULL);
                if(!check_send_game_finish(currentThread))
                {
                    set_turn(currentThread->game);
                    send_turn(currentThread);
                }
                else
                {
                    del_node(&listGame, index_of_node(listGame.head, currentThread->game));
                }
                break;
        }

        #ifdef linux
        pthread_mutex_unlock(&lockNetwork);
        #endif // linux

        #ifdef WIN32
        LeaveCriticalSection(&lockNetwork);
        #endif // WIN32

    }

}
