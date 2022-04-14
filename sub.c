#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

#include "keyValStore.h"

#define MAX_INPUT_LENGTH 1000
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




struct input{
    char command_s[MAX_COMMAND_LENGTH];
    char key_s[MAX_KEY_LENGTH];
    char value_s[MAX_VALUE_LENGTH];
};


struct input * input_func(int *fileDescriptor) {

    char command[MAX_COMMAND_LENGTH] = {};
    char key[MAX_KEY_LENGTH] = {};
    char value[MAX_VALUE_LENGTH] = {};
    char buff[MAX_INPUT_LENGTH] = {};

    static struct input in;

    int seperator[] = {0, 0};
    int x = 0;
    int y = 0;
    int z = 0;


    read(*fileDescriptor, buff, sizeof(buff));
    buff[strlen(buff) - 2] = ' ';


    //Trennen des Strings
    for (int i = 0; i < strlen(buff); i++) {
        if (buff[i] == ' ') {
            seperator[x] = i;
            x++;
        }
    }


    for (int i = 0; i < seperator[0]; i++) {
        command[i] = buff[i];
    }


    for (int i = seperator[0] + 1; i < seperator[1]; i++) {
        key[y] = buff[i];
        y++;
    }


    if (seperator[1] != 0) {
        for (int i = seperator[1] + 1; i < strlen(buff) - 2; i++) {
            value[z] = buff[i];
            z++;
        }
    }


    strcpy(in.command_s, command);
    strcpy(in.key_s, key);
    strcpy(in.value_s, value);

    return &in;
}


int execCommand(struct input *in, int *fileDescriptor, struct key_value_store *key_val) {
    if (strcmp(in->command_s, "GET") == 0) {
        return get(in->key_s, *fileDescriptor, key_val);
    }

    else if (strcmp(in->command_s, "PUT") == 0) {
        return put(in->key_s, in->value_s, *fileDescriptor, key_val);
    }

    else if (strcmp(in->command_s, "DEL") == 0) {
        return del(in->key_s, *fileDescriptor, key_val);
    }

    else if (strcmp(in->command_s, "QUIT") == 0) {
        close_socket(fileDescriptor);
        printf("Verbindung zu Client getrennt\n");
        return 2;
    }

    else {
        printf("Falsche Eingabe\n");
        if (send(*fileDescriptor, "Falsche Eingabe\n", sizeof ("Falsche Eingabe\n"), 0) == -1) {
            printf("Fehler bei send()\n");
        }
        return 0;
    }
}