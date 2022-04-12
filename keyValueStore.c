//
// Created by dustin on 05.04.2022.
//
#include <stdio.h>
#include <string.h>

#include "sub.h"

#define MAX_KEY_LENGTH 10
#define MAX_VALUE_LENGTH 100
#define MAX_KEY_VALUE_STORE_SIZE 10



struct key_value_store {
    char key_s[MAX_KEY_LENGTH];
    char value_s[MAX_VALUE_LENGTH];
};

struct key_value_store key_val[MAX_KEY_VALUE_STORE_SIZE];

void serverResponse(char* command, char* key, char* value, int curSocket);
void getFileName(char* key, char* into);

int put(char* key, char* value)
{
    int curSocket = 4;

    char fileName[1024]={'\0'};	//key + ".txt" ending
    getFileName(key,fileName);

    //printf("adding to filename => %s\n",fileName);
    FILE * file;

    /* open the file for writing*/
    file = fopen (fileName,"w+");
    //can be writen w instead of w+
    //Write
    fprintf (file, "%s\n",value);

    /* close the file*/
    fclose (file);
    strcat(value,"\n");

    //printf("Key added\n");
    serverResponse("PUT", key, value, curSocket);
    return 1;
}
int get(char* key){

    int curSocket = 4;

    char fileName[1024]={'\0'};	//key + .txt ending
    getFileName(key,fileName);


    FILE *file;
    char readed[1024];

    file = fopen (fileName, "r+");
    //can be writen r instead of r+
    if (file != NULL)
    {
        while(fscanf(file, "%s", readed)!=EOF) ///EOF=End Of File
        {
            printf("%s (From GET)\n", readed );
        }
        fclose (file);
        strcat(readed,"\n");
        serverResponse("GET", key, readed, curSocket);
    }
    else
    {

        serverResponse("GET", key, "key_nonexistent\n", curSocket);
        return 1;
    }
    return -1;
}
int del(char* key)
{
    int curSocket = 4;

    char fileName[1024]={'\0'};	//key + .txt
    getFileName(key,fileName);

    if (0==remove(fileName))
        serverResponse("DEL", key, "key_deleted\n", curSocket);
        ///printf("The file is Deleted \n");

    else
        serverResponse("DEL", key, "key_nonexistent\n", curSocket);
    ///printf("the file is NOT Deleted\n");
}


