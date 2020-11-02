#include "includes.h"
/************************************************************************/
/**
 * @brief  Fonction permettant la création d'un user (fait par l'admin)
 * @param  packet : Paquet reçu par le serveur depuis le client contenant les données
 * @return -1     : Impossible d'ouvrir le fichier des comptes
 * @return 1      : La compte a été créé
 */
int create_user(Packet* packet)
{
    FILE* file  = NULL;
    Packet_Account credentials   = {0};

    file = fopen(USERS_FILE, "ab");
    if(file == NULL)
        return -1;

    strncat(credentials.username, packet->account.username, SIZE_USERNAME);
    strncat(credentials.password, packet->account.password, SIZE_PASSWORD);
    fwrite(&credentials, sizeof(Packet_Account), 1, file);

    fclose(file);
    return 1;
}
/************************************************************************/
/**
 * @brief  Fonction permettant la suppresion d'un utilisateur (fait par l'admin)
 * @param  username : Nom de l'utilisateur dont on doit supprimer le compte
 * @return 0        : Impossible d'ouvrir le fichier des comptes
 * @return 1        : La compte a été supprimé
 */
int delete_user(char* username)
{
    int   bytes = 0;
    FILE* file  = NULL;
    FILE* file2 = NULL;
    Packet_Account  currentUser   = {0};

    file = fopen(USERS_FILE, "rb");
    if(file == NULL)
        return 0;

    file2 = fopen("users.tmp", "wb+");
    if(file2 == NULL)
    {
        fclose(file);
        return 0;
    }
    do
    {
        memset(&currentUser, 0, sizeof(Packet_Account));
        bytes = fread(&currentUser, sizeof(Packet_Account), 1, file);
        if(strncmp(username, currentUser.username, SIZE_USERNAME) != 0)
        {
            fwrite(&currentUser, sizeof(Packet_Account), bytes, file2);
        }


    }while(bytes == 1);


    fclose(file);
    rewind(file2);
    file = fopen(USERS_FILE, "wb");
    if(file == NULL)
    {
        fclose(file2);
        return 0;
    }

    do
    {
        bytes = fread(&currentUser, sizeof(Packet_Account), 1, file2);
        fwrite(&currentUser, sizeof(Packet_Account), bytes, file);
        memset(&currentUser, 0, sizeof(Packet_Account));

    }while(bytes == 1);

    fclose(file);
    fclose(file2);
    remove("users.tmp");
    return 1;
}
/************************************************************************/
/**
 * @brief  Fonction permettant de confirmer au terminal client (admin) que le compte a bien été crée
 * @param  recipient : Paquet contenant les informations du client devant recevoir le paquet
 * @return packet    : Paquet à envoyer au client contenant le message de confirmation (ou non) de création du compte
 */
Packet account_create(Packet* recipient)
{
    Packet packet = {0};

    if(user_exist(recipient->account.username))
        packet.messageType = MESSAGE_TYPE_ACCOUNT_CREATE_ALREADY_EXIST;
    else
    {
        if(create_user(recipient))
            packet.messageType = MESSAGE_TYPE_ACCOUNT_CREATE_OK;
        else
            packet.messageType = MESSAGE_TYPE_INTERNAL_ERROR;
    }

    return packet;
}
/************************************************************************/
/**
 * @brief  Fonction permettant de confirmer au terminal client (admin) que le compte a bien été supprimé
 * @param  recipient : Paquet contenant les informations du client devant recevoir le paquet
 * @return packet    : Paquet à envoyer au client contenant le message de confirmation (ou non) de suppression du compte
 */
Packet account_delete(Packet* recipient)
{
    Packet packet = {0};

    if(user_exist(recipient->account.username))
    {
        if(delete_user(recipient->account.username))
            packet.messageType = MESSAGE_TYPE_ACCOUNT_DELETE_SUCCES;
    }
    else
        packet.messageType = MESSAGE_TYPE_ACCOUNT_DELETE_BAD_USERNAME;

    return packet;
}
/************************************************************************/
/**
 * @brief  Fonction permettant de récupérer la liste des utilisateurs pour l'admin (utilisé dans le cadre de la suppression des comptes)
 * @param  socket : Socket entre le serveur et le client (admin) pour envoyer les informations par paquets
 * @return void   : Cette fonction ne retourne rien, mais elle envoie les informations des utilisateurs dans des paquets à travers la socket
 */
void send_user(int socket)
{
    Packet packet = {0};
    FILE* userFile = NULL;
    int bytes = 0;
    Packet_Account currentUser = {0};

    if((userFile = fopen(USERS_FILE, "rb")) == NULL)
        return;

    packet.messageType = MESSAGE_TYPE_USER_ENUMERATE_REPLY;
    do
    {
        memset(&currentUser, 0, sizeof(Packet_Account));
        bytes = fread(&currentUser, sizeof(Packet_Account), 1, userFile);
        memcpy(&packet.account.username, &currentUser.username, SIZE_USERNAME);
        if(bytes == 1)
        {
            write_client(socket, packet);
        }

    } while(bytes == 1);

    fclose(userFile);
}
/************************************************************************/
