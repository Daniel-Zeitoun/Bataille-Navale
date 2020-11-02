#include "../headers/client.h"



void request_reply_server(Packet* packet)
{
    int bytes = 0;
    bytes = send(sock, packet, sizeof(Packet), 0);
    memset(packet, 0, sizeof(Packet));
    bytes = recv(sock, packet, sizeof(Packet), 0);
}

Packet read_server(int socket)
{
    struct timeval timeSelect = { 0 };
    timeSelect.tv_usec = 1000;
    fd_set fdsock = { 0 };
    Packet recipient = { 0 };

    FD_ZERO(&fdsock);
    FD_SET(socket, &fdsock);

    if (select(socket + 1, &fdsock, NULL, NULL, &timeSelect) > 0)
    {
        if (recv(socket, (char*)&recipient, sizeof(Packet), 0) == sizeof(Packet))
            return recipient;
    }

    recipient.messageType = MESSAGE_TYPE_CLOSE_CONNECTION;
    return recipient;
}

void write_server(Packet* packet)
{
    send(sock, packet, sizeof(Packet), 0);
}

Packet connect_server(HWND hWnd)
{
    Packet packet = { 0 };
    char               bufferMessageBox[2048] = { 0 };
    struct sockaddr_in sin = { 0 };
    struct hostent* host = NULL;
    int                bytes = 0;

    GetWindowTextA(hwndUsername, packet.account.username, SIZE_USERNAME);
    GetWindowTextA(hwndPassword, packet.account.password, SIZE_PASSWORD);

    memset(username, 0, sizeof(username));
    GetWindowTextA(hwndUsername, username, SIZE_USERNAME);

    if (packet.account.username[0] == 0 || packet.account.password[0] == 0)
        return packet;

    packet.messageType = MESSAGE_TYPE_LOGIN_REQUEST;

    if (initNetwork() != 0)
    {
        MessageBoxW(hWnd, L"Linitialisation du réseau a echoué", L"initNetwork", MB_ICONERROR);
        return packet;
    }
    //Pour les noms de domaines
    if ((host = gethostbyname(server)) == NULL)
    {
        MessageBoxW(hWnd, L"Impossible de récupperer l'adresse IP du serveur", L"gethostbyname", MB_ICONERROR);
        endNetwork(sock);
        return packet;
    }
    //
    if ((sock = (int)socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        MessageBoxW(hWnd, L"La création du socket a échouée", L"Socket", MB_ICONERROR);
        endNetwork(sock);
        return packet;
    }

    //Déclaration des paramètres IP pour le serveur
    sin.sin_family = AF_INET;
    sin.sin_addr = *(struct in_addr*)host->h_addr;
    sin.sin_port = htons(port);
    memset((void*)sin.sin_zero, 0, 8);

    if ((connect(sock, (struct sockaddr*)&sin, sizeof(sin))) == -1)
    {
        sprintf(bufferMessageBox, "Aucune réponse du serveur : %s:%d", server, port);
        MessageBoxA(hWnd, bufferMessageBox, "Connect", MB_ICONERROR);
        endNetwork(sock);
        return packet;
    }

    request_reply_server(&packet);
    return packet;
}


void keep_alive_thread_proc(int socket)
{
    Packet packet = { 0 };
    packet.messageType = MESSAGE_TYPE_KEEP_ALIVE;

    while (1)
    {
        send(socket, &packet, sizeof(Packet), 0);
        Sleep(5000);
    }
}

int initNetwork(void)
{
    WSADATA wsa = { 0 };
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return -1;

    return 0;
}

void endNetwork(int socket)
{
    closesocket(socket);
    WSACleanup();
}

void close_connection(void)
{
    Packet packet = { 0 };
    packet.messageType = MESSAGE_TYPE_CLOSE_CONNECTION;
    send(sock, &packet, sizeof(Packet), 0);
}