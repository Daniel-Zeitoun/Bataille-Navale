#include "includes.h"

/************************************************************************/
/**
 * @brief  Fonction permettant de savoir si l'utilisateur qui souhaite se connecter est l'admin
 * @param  username : Le nom de l'utilisateur
 * @return 0        : Ce n'est pas l'identifiant de l'administrateur
 * @return 1        : C'est l'identifiant de l'administrateur
 */
 int is_admin(char* username)
{
    if(username == NULL)
        return 0;

    if(strncmp(username, ADMIN_USERNAME, SIZE_USERNAME) == 0)
        return 1;

    return 0;
}
/************************************************************************/
/**
 * @brief  Fonction permettant de savoir si l'utilisateur qui souhaite se connecter est l'admin
 * @param  password : Le mot de passe de l'utilisateur
 * @return 0        : Ce n'est pas le mot de passe de l'administrateur
 * @return 1        : C'est le mot de passe de l'administrateur
 */
int is_admin_good_password(char* password)
{
    if(password == NULL)
        return 0;

    if(strncmp(password, ADMIN_PASSWORD, SIZE_PASSWORD) == 0)
        return 1;

    return 0;
}
/************************************************************************/
/**
 * @brief  Fonction permettant de savoir si l'utilisateur qui souhaite se connecter utilise un couple
 *         identifiant/mot de passe valide
 * @param  credentials : La partie du paquet envoyé par le client contenant les informations d'identification
 * @return 0           : Ce n'est le bon couple identifiant/mot de passe
 * @return 1           : C'est le bon couple identifiant/mot de passe
 */
int is_good_password(Packet_Account* credentials)
{
    FILE* fileUser   = NULL;
    int bytes        = 0;
    Packet_Account currentUser = {0};

    if(credentials == NULL)
        return 0;
    if((fileUser = fopen(USERS_FILE, "rb")) == NULL)
        return 0;

    do
    {
        bytes = fread(&currentUser, sizeof(Packet_Account), 1, fileUser);
        if(strncmp(credentials->username, currentUser.username, SIZE_USERNAME) == 0)
        {
            if(strncmp(credentials->password, currentUser.password, SIZE_PASSWORD) == 0)
            {
                fclose(fileUser);
                return 1;
            }
        }
        memset(&currentUser, 0, sizeof(Packet_Account));

    } while(bytes == 1);

    return 0;
}
/************************************************************************/
/**
 * @brief  Fonction permettant de savoir si un utilisateur existe dans la "base de données" (fichier) des utilisateurs
 * @param  username : Le nom de l'utilisateur dont on souhaite connaitre l'existence
 * @return 0        : L'utilisateur n'existe pas
 * @return 1        : L'utilisateur exite
 */
int user_exist(char* username)
{
    FILE* fileUser   = NULL;
    int bytes        = 0;
    Packet_Account currentUser = {0};

    if(username == NULL)
        return 0;
    if((fileUser = fopen(USERS_FILE, "rb")) == NULL)
        return 0;

    do
    {
        bytes = fread(&currentUser, sizeof(Packet_Account), 1, fileUser);
        if(strncmp(username, currentUser.username, SIZE_USERNAME) == 0)
        {
            fclose(fileUser);
            return 1;
        }
        memset(&currentUser, 0, sizeof(Packet_Account));

    } while(bytes == 1);

    return 0;
}
/************************************************************************/
/**
 * @brief  Fonction permettant de se connecter à l'interface client
 * @param  recipient     : Les informations envoyées par le client dans un paquet
 * @param  currentThread : Le thread lié au client
 * @return packet        : Un paquet contenant un message validant ou non l'authentification de l'utlisateur
 */
Packet login(Packet* recipient, Thread* currentThread)
{
    Packet packet = {0};
    Player player = {0};

    if(is_admin(recipient->account.username))
    {
        if(is_admin_good_password(recipient->account.password))
            packet.messageType = MESSAGE_TYPE_LOGIN_REPLY_ADMIN_SUCCESS;
        else
            packet.messageType = MESSAGE_TYPE_LOGIN_REPLY_BAD_PASSWORD;
    }
    else if(user_exist(recipient->account.username))
    {
        if(is_good_password(&recipient->account))
        {
            packet.messageType = MESSAGE_TYPE_LOGIN_REPLY_SUCCESS;
            player = is_in_game(recipient->account.username);

            if(player.playerAddr != NULL)
            {
                ((Player*)(player.playerAddr->data))->sock = currentThread->sock;
                currentThread->game = player.game;
                currentThread->playerAddr = player.playerAddr;

                if(((Game*)player.game->data)->isRunning)
                {
                    //La game est deja lancée
                    packet.messageType = MESSAGE_TYPE_PLAYER_IN_GAME_RUNNING;
                }
                else
                {
                    packet.messageType = MESSAGE_TYPE_PLAYER_IN_GAME;
                    //Le joueur à rejoint mais la game n'est pas encore lancée
                }
            }
        }
        else
            packet.messageType = MESSAGE_TYPE_LOGIN_REPLY_BAD_PASSWORD;
    }
    else
        packet.messageType = MESSAGE_TYPE_LOGIN_REPLY_BAD_USERNAME;

    return packet;
}
/************************************************************************/
