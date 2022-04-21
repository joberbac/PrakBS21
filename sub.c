#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/sem.h>

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
        error_exit("Error at creating stream socket");
    }

    return sock;
}

//Weist dem Socket die IP-Adresse und die Portnummer zu, unter der das Socket auf Anfragen wartet.
void bind_socket(int *sock_fd, unsigned long address, unsigned short port) {
    struct sockaddr_in server;
    server.sin_family = AF_INET;                    //AF_INET = IPv4
    server.sin_addr.s_addr = htonl(address);        //Die Kommunikationsschnittstelle des Hosts. Normalerweise die IP-Adresse.
    server.sin_port = htons(port);                  //Transformiert die Portnummer in Network-Byte-Order.

    if (bind(*sock_fd, (struct sockaddr*)&server, sizeof (server)) < 0) {
        error_exit("Error at binding socket");
    }
}

//Warteschlange, die auf eingehende Verbindungswünsche eines Clients wartet.
void listen_socket(int *sock_fd, int backlog) {
    if (listen(*sock_fd, backlog) == -1) {
        error_exit("Error at listen");
    }
}

//Bearbeitet die Verbindungswünsche von Clients.
//Der Aufruf von accept() blockiert so lange, bis ein Client Verbindung aufnimmt.
void accept_socket(int *sock_fd, int *connection_fd) {
    struct sockaddr_in client;
    unsigned int client_size = sizeof (client);

    *connection_fd = accept(*sock_fd, (struct sockaddr*)&client, &client_size);

    if (*connection_fd == -1) {
        error_exit("Error at accept");
    }
}

//Socket schließen
void close_socket(int *sock_fd) {
    close(*sock_fd);
}



struct input{
    char command_s[MAX_COMMAND_LENGTH];
    char key_s[MAX_KEY_LENGTH];
    char value_s[MAX_VALUE_LENGTH];
};


void output(int *connection_fd, char *message) {
    printf("%s", message);
    if (write(*connection_fd, message, strlen (message)) == -1) {
        error_exit("Error at output");
    }
}


struct input * input_func(int *connection_fd) {

    char command[MAX_COMMAND_LENGTH] = {};
    char key[MAX_KEY_LENGTH] = {};
    char value[MAX_VALUE_LENGTH] = {};
    char buff[MAX_INPUT_LENGTH] = {};

    static struct input in;

    int seperator[] = {0, 0};       //Seperator = Trenner zwischen den Command, Key und Value
    int x = 0;
    int y = 0;
    int z = 0;

    read(*connection_fd, buff, sizeof(buff));
    buff[strlen(buff) - 2] = ' ';

    //Trennen des Strings
    for (int i = 0; i < strlen(buff); i++) {        //Ermittelt die Stellen der " " Leerzeichen
        if (buff[i] == ' ') {
            if (x == 2)
                break;
            seperator[x] = i;
            x++;
        }
    }

    for (int i = 0; i < seperator[0]; i++) {        //Separiert den Command
        command[i] = buff[i];
    }

    for (int i = seperator[0] + 1; i < seperator[1]; i++) {     //Separiert den Key
        key[y] = buff[i];
        y++;
    }

    if (seperator[1] != 0) {        //Separiert den Value
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


int execCommand(struct input *in, int *connection_fd, struct key_value_store *shar_mem) {

    if (strcmp(in->command_s, "GET") == 0) {
        return get(in->key_s, connection_fd, shar_mem);
    }

    else if (strcmp(in->command_s, "PUT") == 0) {
        return put(in->key_s, in->value_s, connection_fd, shar_mem);
    }

    else if (strcmp(in->command_s, "DEL") == 0) {
        return del(in->key_s, connection_fd, shar_mem);
    }

    else if (strcmp(in->command_s, "QUIT") == 0) {
        close_socket(connection_fd);
        printf("Verbindung zu Client getrennt\n");
        return 2;
    }

    else if (strcmp(in->command_s, "BEG") == 0) {
        return 5;
    }

    else if (strcmp(in->command_s, "END") == 0) {
        return 6;
    }

    else {
        output(connection_fd, "Falsche Eingabe\n");
        return 0;
    }
}



