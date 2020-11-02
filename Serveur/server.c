#include "includes.h"

/************************************************************************/
/**
 * @brief  Fonction permettant de signaler au client qu'il y a eu une erreur interne au sein du serveur
 * @param  sock : Socket ouverte entre le client et le serveur
 * @return void : Ne retourne rien
 */
void send_internal_error(int sock)
{
    Packet packet = {0};
    packet.messageType = MESSAGE_TYPE_INTERNAL_ERROR;
    send(sock, (char*)&packet, sizeof(Packet), 0);
}
/************************************************************************/
/**
 * @brief  Fonction permettant de lire les données envoyées par le client
 * @param  socket : Socket ouverte entre le client et le serveur
 * @return packet : Retourne un paquet avec les informations que le client a envoyé
 */
Packet read_client(int socket)
{
    struct timeval timeSelect = {0};
    timeSelect.tv_sec         = 10;
    fd_set fdsock             = {0};
    Packet recipient          = {0};

    FD_ZERO(&fdsock);
    FD_SET(socket, &fdsock);

    if(select(socket + 1, &fdsock, NULL, NULL, &timeSelect) > 0)
    {
        if(recv(socket, (char*)&recipient, sizeof(Packet), 0) == sizeof(Packet))
            return recipient;
    }

    recipient.messageType = MESSAGE_TYPE_CLOSE_CONNECTION;
    return recipient;
}
/************************************************************************/
/**
 * @brief  Fonction permettant d'envoyer des données au client
 * @param  socket : Socket ouverte entre le client et le serveur
 * @param  packet : Paquet contenant les informations à envoyer au client
 * @return void   : Ne retourne rien
 */
void write_client(int socket, Packet packet)
{
    send(socket, (char*)&packet, sizeof(Packet), 0);
}
/************************************************************************/
