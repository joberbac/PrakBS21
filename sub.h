//
// Created by dustin on 05.04.2022.
//
#ifndef SUB_H_
#define SUB_H_

char *connecter(char input[]);
void ExtractKey(char* from,char* into);
void ExtractValue(char* from,char* into);
void fixInput(char* buffer);
void *connect_sockets(void* socketNum);
void serverResponse(char* command, char* key, char* value, int curSocket);
void getFileName(char* key, char* into);



#endif
