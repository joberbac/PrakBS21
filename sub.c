//
// Created by dustin on 05.04.2022.
//
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>


#include "keyValStore.h"

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


