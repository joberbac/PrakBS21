#include <sys/socket.h>
#include <netinet/in.h>
#include <printf.h>
#include <stdlib.h>
#include "sub.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>


#define PORT 5678
//Protos

int main()///int main(int argc, char const *argv[])
{
    int socketDescr, curSocket;
    ///   int socketConnections[5];
    int* socketConnections;
    socketConnections = (int*)malloc(sizeof(int)*4);
    if(socketConnections==NULL){
        printf("Allocation Error");
        return -1;
    }
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};///is a global or static declaration, it will be stored in the binary's data segment with its initialized data, thus increasing your binary size about 1024 bytes (in this case of course)




    // create server socket		SOCK_STREAM = enables tcp/ip Protocoll	0= InternetProtocol
    if ((socketDescr = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");//////Exatcly I don't know what perror does but Youtubers using it in their tutorials
        exit(EXIT_FAILURE);
    }

    // settings optional options for other connections
    if (setsockopt(socketDescr, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;		//Settings listening ip adress to pcs current ip adress
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );		//settings the port to 5678

    // bind the socket to our specified IP and port
    if (bind(socketDescr, (struct sockaddr *)&address,
             sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(socketDescr, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //storing socketconnection into new_socket
    if ((curSocket = accept(socketDescr, (struct sockaddr *)&address,
                            (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    printf( "Connected to socket:%i\n",curSocket);

    char greeting[1024]="Connected to server! ";
    char str[12];
    sprintf(str, "%d", curSocket); ///In Client's side

    strcat(greeting,str);
    strcat(greeting,"\n");
    send(curSocket , greeting, strlen(greeting) , 0 );

    pthread_t threads[20];
    pthread_create(&threads[0],NULL,connect_sockets,&curSocket);
    int threadIndex = 1;

    while(1)//infinite loop  to make it continuous
    {
        printf( "Waiting for client to connect!\n");
        //storing socketconnection into new_socket
        if ((curSocket = accept(socketDescr, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf( "Connected to socket:%i\n",curSocket);

        pthread_create(&threads[threadIndex++],NULL,connect_sockets,&curSocket);

        strcpy(greeting, "Connected to server! ");
        sprintf(str, "%d", curSocket);
        strcat(greeting,str);
        strcat(greeting,"\n");
        send(curSocket , greeting, strlen(greeting) , 0 );
    }

    return 0;
}
