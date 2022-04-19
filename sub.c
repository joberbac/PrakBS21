#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#include "sub.h"
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

/*****************************************************************************************************************/
/*************************************** Socket Systemaufrufe ****************************************************/
/*****************************************************************************************************************/

int create_socket(int af, int type, int protocol) {
    int sock_fd;

    if ((sock_fd = socket(af, type, protocol)) < 0) {
        error_exit("Error occurred at create_socket\n");
    }

    return sock_fd;
}


//Weist dem Socket die IP-Adresse und die Portnummer zu, unter der das Socket auf Anfragen wartet.
void bind_socket(int *socket_fd, unsigned long address, unsigned short port) {

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY; //alle IP-Adressen des Rechners
    server.sin_port = htons(port); //Transformiert die Portnummer in Network-Byte-Order.

    if (bind(*socket_fd, (struct sockaddr*)&server, sizeof (server)) < 0) { //hier das eigentliche Binden
        error_exit("Error occurred at bind_socket\n");
    }
}


//Warteschlange, die auf eingehende Verbindungswünsche eines Clients wartet.
void listen_socket(int *socket_fd) {
    if (listen(*socket_fd, 5) == -1) { // 2. Parameter Warteschlangenlänge
        error_exit("Error occurred at listen_socket\n");
    }
}


//Verbindung eines Clients wird entgegengenommen
//Der Aufruf von accept() blockiert so lange, bis ein Client Verbindung aufnimmt.
void accept_socket(int *socket_fd, int *cfd) {
    struct sockaddr_in client;
    unsigned int client_size = sizeof (client);

    *cfd = accept(*socket_fd, (struct sockaddr*)&client, &client_size);  //Rückgabewert von accept() ein neues Socket

    if (*cfd == -1) {
        error_exit("Error occurred at accept_socket\n");
    }

}


//Socket schließen
void close_process(int *port) {
    close(*port);
}

/*****************************************************************************************************************/
/*************************************** Input Verarbeitung ******************************************************/
/*****************************************************************************************************************/

struct input * input_func(int *cfd) {

    char command[MAX_COMMAND_LENGTH] = {};
    char key[MAX_KEY_LENGTH] = {};
    char value[MAX_VALUE_LENGTH] = {};
    char buff[MAX_INPUT_LENGTH] = {};

    static struct input in;

    int separator[] = {0, 0};
    int x = 0;
    int y = 0;
    int z = 0;


    read(*cfd, buff, sizeof(buff));
    buff[strlen(buff) - 2] = ' ';


    //Trennen des Strings
    for (int i = 0; i < strlen(buff); i++) {
        if (buff[i] == ' ') {
            separator[x] = i;
            x++;
        }
    }


    for (int i = 0; i < separator[0]; i++) {
        command[i] = buff[i];
    }


    for (int i = separator[0] + 1; i < separator[1]; i++) {
        key[y] = buff[i];
        y++;
    }


    if (separator[1] != 0) {
        for (int i = separator[1] + 1; i < strlen(buff) - 2; i++) {
            value[z] = buff[i];
            z++;
        }
    }


    strcpy(in.command_s, command);
    strcpy(in.key_s, key);
    strcpy(in.value_s, value);

    return &in;
}


int execCommand(struct input *in, int *cfd, struct key_value_store *key_val) {
    if (strcmp(in->command_s, "GET") == 0) {
        return get(in->key_s, *cfd, key_val);
    }

    else if (strcmp(in->command_s, "PUT") == 0) {
        return put(in->key_s, in->value_s, *cfd, key_val);
    }

    else if (strcmp(in->command_s, "DEL") == 0) {
        return del(in->key_s, *cfd, key_val);
    }

    else if (strcmp(in->command_s, "QUIT") == 0) {
        close_process(cfd);
        printf("Disconnected Client\n");
        return 2;
    }

    else {
        printf("Invalid Input\n");
        if (send(*cfd, "Invalid Input\n", sizeof ("Invalid Input\n"), 0) == -1) {
            printf("Error occurred at send\n");
        }
        return 0;
    }
}
