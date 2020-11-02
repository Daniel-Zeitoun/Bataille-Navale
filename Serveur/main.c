#include "includes.h"


int main(int argc, char* argv[])
{
    printf("######################################\n");
    printf("########## Bataille  navale ##########\n");
    printf("######################################\n");
    printf("# Daniel ZEITOUN -- Damien MICHAUDEL #\n");
    printf("######################################\n");
    wprintf(L"VERSION : %ls\n\n", VERSION);

    #ifdef WIN32
    printf("SERVEUR - WINDOWS\n\n");
    WSADATA wsa = {0};
    WSAStartup(MAKEWORD(2, 2), &wsa);
    #endif // WIN32

    #ifdef linux
    printf("SERVEUR - LINUX\n\n");
    #endif // linux

    struct sockaddr_in sin     = {0};
    unsigned int       sock    = 0;
    int                sinSize = 0;


    #ifdef linux
    pthread_mutex_init(&lockNetwork, NULL);
    #endif // linux

    #ifdef WIN32
    InitializeCriticalSection(&lockNetwork);
    #endif // WIN32


    FILE* fileUsers = NULL;
    if((fileUsers = fopen(USERS_FILE, "a+")) == NULL)
    {
        printf("Echec de l'initialisation du fichier users.bn\n");
        return 0;
    }
    fclose(fileUsers);


    if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(-1);
    }

    //Déclaration des paramètres IP pour le serveur
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORT);
    memset((void*)sin.sin_zero, 0, 8);
    sinSize = sizeof(sin);

    //Met en lien les paramètres IP avec le socket
    if(bind(sock, (const struct sockaddr*)&sin, sinSize) == -1)
    {
        perror("bind");
        exit(-1);
    }

    if(listen(sock, 0) == -1)
    {
        perror("accept");
        exit(-1);
    }

    while(1)
    {
        unsigned int csock = 0;
        pthread_t thread   = {0};
        Thread data        = {0};

        printf("Attente d'un client ...\n");
        if((csock = accept(sock, (struct sockaddr*)&sin, &sinSize)) == -1)
        {
            perror("accept");
            exit(-1);
        }
        printf("socket = %d : connection -> OK\n", csock);

        #ifdef linux
        pthread_mutex_lock(&lockNetwork);
        #endif // linux

        #ifdef WIN32
        EnterCriticalSection(&lockNetwork);
        #endif // WIN32

        add_node(&listThread);
        data.sock = csock;
        init_data_thread(listThread.tail, data);
        pthread_create(&thread, NULL, (void*)clientCallback, (void*)listThread.tail);

        #ifdef linux
        pthread_mutex_unlock(&lockNetwork);
        #endif // linux

        #ifdef WIN32
        LeaveCriticalSection(&lockNetwork);
        #endif // WIN32

    }

    #ifdef linux
    pthread_mutex_destroy(&lockNetwork);
    #endif // linux

    #ifdef WIN32
    DeleteCriticalSection(&lockNetwork);
    #endif // WIN32


    close(sock);
    #ifdef WIN32
    WSACleanup();
    #endif // WIN32

    return 0;
}

