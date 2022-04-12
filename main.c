#include <sys/socket.h>
#include <netinet/in.h>
#include <printf.h>
#include <stdlib.h>
#include "sub.h"
#include "keyValStore.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


#define PORT 5678
//Protos
char *connecter(char input[]);

void ExtractKey(char* from,char* into);
void ExtractValue(char* from,char* into);


void fixInput(char* buffer);
void *connect_sockets(void* socketNum);



static pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;		//Mutex to prevent race conditions
static pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex4 = PTHREAD_MUTEX_INITIALIZER;

int ReaderMode = 0;
int wait2Write = 0;
int soleSock = -1;
char latestAction[1024] = {0};



char* connecter(char input[]){

    return strcat(input,":");
}

void serverResponse(char* command, char* key, char* value, int curSocket)
{
    char output[1024]={'\0'};//is a global or static declaration, it will be stored in the binary's data segment with its initialized data, thus increasing your binary size by about 1024 bytes (in this case)
    //,n other words make array full of empty

    strcat(output,command);
    connecter(output);
    strcat(output,key);
    connecter(output);
    strcat(output,value);

    strcpy(latestAction, output);


    send(curSocket , output , strlen(output) , 0 );
}



void ExtractKey(char* from,char* into)
{
    char buffer[1024];
    strcpy(buffer, from);
    char * ptr;
    ptr = strchr( buffer, ' ' );	//String from first whitespace(nullchraacter)
    int wid =0;

    while(ptr[wid + 1] != ' ' && ptr[wid + 1] != '\n' && ptr[wid + 1] != '\0')
    {
        //printf( "%c\n",ptr[index + width + 1] );
        wid++;
    }
    memcpy( into, ptr + 1, wid);
    into[wid] = '\0';
    //printf( "KEY:%s\n",into);
}
void ExtractValue(char* from,char* into)
{
    char buffer[1024];
    char bS[1024];
    strcpy(buffer, from);
    strcpy(bS, buffer);
    char * ptr;
    ptr = strchr( buffer, ' ' );   //String from first whitespace(nullchraacter)
    ptr = strchr( ptr+1, ' ' );	//String from second whitespace(nullchraacter)
    //printf( "The first  %c in '%s' is '%s'\n",' ', buffer, ptr );
    int wid =0;
    while(ptr[wid + 1] != ' '&& ptr[wid + 1] != '\n' && ptr[wid + 1] != '\0')
    {
        //printf( "%c\n",ptr[index + width + 1] );
        wid++;
    }
    memcpy( into, ptr + 1, wid);
    into[wid] = '\0';

    //printf( "VALUE: %s\n\n",into);
    strcpy(from, bS);
}
void getFileName(char* key, char* into)
{
    strcpy(into, key);
    strcat(into,".txt");
}




void fixInput(char* buffer)
{
    int index = 0;
    while(buffer[index] != '\0')
    {
        if(buffer[index] == '\n')
        {
            buffer[index-1] = ' ';
            buffer[index] = '\0';
        }
        index++;
    }
}
void *connect_sockets(void* socketNum)
{

    int *num =(int *) socketNum;
    int valread, curSocket = *num;
    char buffer[1024] = {0};
    while(1)
    {
        valread = read( curSocket , buffer, 1024);
        printf( "%i. Socket send a request!\n",curSocket);
        fixInput(buffer);	//Input need a space at the end

        char command[1024];
        strcpy(command, buffer);
        char key[1024]={'\0'};
        char value[1024];
        pthread_mutex_lock(&mutex4);
        pthread_mutex_unlock(&mutex4);
        if(curSocket == soleSock || soleSock == -1)
        {
            if(strstr(command, "PUT") != NULL ||strstr(command, "DEL") != NULL)	//Write-mode
            {
                // 5x Mutex solve Reading
                // Writing Problem
                pthread_mutex_lock(&mutex2);
                wait2Write ++;
                pthread_mutex_unlock(&mutex2);

                while(ReaderMode > 0)
                {
                    sleep(1);	//waits 1 sec
                }
                pthread_mutex_lock(&mutex1);		//locking to prevent rase conditions.for Zombie Problem?
                if (strstr(command, "PUT") != NULL)
                {
                    ExtractKey(buffer,key);
                    ExtractValue(buffer, value);
                    put(key,value);

                }
                if (strstr(command, "DEL") != NULL) {
                    ExtractKey(buffer,key);
                    del(key);

                }

                pthread_mutex_unlock(&mutex1);		//unlock mutex
                pthread_mutex_lock(&mutex2);
                wait2Write--;
                pthread_mutex_unlock(&mutex2);
            }
            else				//Reader-mod
            {
                while(wait2Write > 0)
                {
                    sleep(1);
                }
                pthread_mutex_lock(&mutex3);
                ReaderMode ++;
                pthread_mutex_unlock(&mutex3);
                if (strstr(command, "GET") != NULL)
                {
                    ExtractKey(buffer,key);
                    get(key);
                }
                pthread_mutex_lock(&mutex3);
                ReaderMode --;
                pthread_mutex_unlock(&mutex3);
            }
        }else
        {
            send(curSocket , "A client has sole access, please try later.", strlen("A client has sole access, please try later.") , 0 );
        }

        if (strstr(command, "QUIT") != NULL) {
            printf("Socket %i disconnected\n", curSocket);
            send(curSocket , "CONNECTION LOST!", strlen("CONNECTION LOST!") , 0 );
            break;
        }
    }
    return 0;
}
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
