#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#include "keyValStore.h"

#define MAX_INPUT_LENGTH 100
#define MAX_COMMAND_LENGTH 10
#define MAX_KEY_LENGTH 10
#define MAX_VALUE_LENGTH 100

//Die Funktion gibt aufgetretene Fehler aus und beendet die Anwendung.
void error_exit(char *error_message) {
    printf("%s\n", error_message);
    exit(EXIT_FAILURE);
}

//Erzeugt das Socket.
int create_socket(int af, int type, int protocol) {
    int sock;

    if ((sock = socket(af, type, protocol)) < 0) {
        error_exit("Fehler beim Anlegen eines Sockets\n");
    }

    return sock;
}

//Weist dem Socket die IP-Adresse und die Portnummer zu, unter der das Socket auf Anfragen wartet.
void bind_socket(int *sock, unsigned long address, unsigned short port) {
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(address);        //Die Kommunikationsschnittstelle des Hosts. Normalerweise die IP-Adresse.
    server.sin_port = htons(port);                  //Transformiert die Portnummer in Network-Byte-Order.

    if (bind(*sock, (struct sockaddr*)&server, sizeof (server)) < 0) {
        error_exit("Kann das Socket nicht \"binden\"\n");
    }
}

//Warteschlange, die auf eingehende Verbindungswünsche eines Clients wartet.
void listen_socket(int *sock) {
    if (listen(*sock, 5) == -1) {
        error_exit("Fehler bei listen\n");
    }
}

//Bearbeitet die Verbindungswünsche von Clients.
//Der Aufruf von accept() blockiert so lange, bis ein Client Verbindung aufnimmt.
void accept_socket(int *sock, int *fileDescriptor) {
    struct sockaddr_in client;
    unsigned int client_size = sizeof (client);

    *fileDescriptor = accept(*sock, (struct sockaddr*)&client, &client_size);  //Rückgabewert von accept() ein neues Socket

    if (*fileDescriptor == -1) {
        error_exit("Fehler bei accept");
    }
}

//Socket schließen
void close_socket(int *sock) {
    close(*sock);
}


int input(int *sock, int fileDescriptor) {

    char command[MAX_COMMAND_LENGTH] = {};
    char key[MAX_KEY_LENGTH] = {};
    char value[MAX_VALUE_LENGTH] = {};
    char buff[MAX_INPUT_LENGTH] = {};

    int seperator[] = {};
    int j = 0;
    int a = 0;
    int b = 0;

    read (fileDescriptor, buff, sizeof (buff));
    buff[strlen (buff)-2] = ' ';


    //Trennen des Strings
    for (int i = 0; i < strlen(buff); i++) {
        if (buff[i] == ' ') {
            seperator[j] = i;
            j++;
        }
    }

    for (int i = 0; i < seperator[0]; i++) {
        command[i] = buff[i];
    }

    for (int i = seperator[0] + 1; i < seperator[1]; i++) {
        key[a] = buff[i];
        a++;
    }

    for (int i = seperator[1] + 1; i < strlen(buff) - 2; i++) {
        value[b] = buff[i];
        b++;
    }


    if (strcmp(command, "GET") == 0) {
        return get(key);
    }

    else if (strcmp(command, "PUT") == 0) {
        return put(key, value);
    }

    else if (strcmp(command, "DEL") == 0) {
        return del(key);
    }

    else if (strcmp(command, "QUIT") == 0) {
        close_socket(&fileDescriptor);
        printf("Verbindung zu Client %i getrennt\n", fileDescriptor);
        return 2;
    }

    else {
        printf("Falsche Eingabe\n");
        return 0;
    }
}
