#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>

#define SERVER_PORT 4711

int sock;
int fileDescriptor;

//Die Funktion gibt aufgetretene Fehler aus und beendet die Anwendung.
void error_exit(char *error_message) {
    printf("%s\n", error_message);
    exit(EXIT_FAILURE);
}

//Erzeugt das Socket.
int create_socket() {
    sock = socket(AF_INET, SOCK_STREAM, 0);     //Verfügbar durch #include <sys/socket.h>,
                                                //AF_INET steht für Adress Family Internet (IPv4),
                                                //SOCK_STREAM steht für zuverlässigen verbindungsorientierten Bytestrom (TCP).
                                                //Dritter Parameter gibt das Protokoll an. 0 verwendet das Standartprotokoll
                                                //für den gewählten Socket-Typ.

    if (sock < 0) {                             //Bei einem Fehler würde sock() -1 zurückgeben
        error_exit("Fehler beim Anlegen eines Sockets");
    }

    return sock;
}

//Weist dem Socket die IP-Adresse und die Portnummer zu, unter der das Socket auf Anfragen wartet.
void bind_socket() {
    struct sockaddr_in server;
    server.sin_family = AF_INET;                //AF_INET steht für Adress Family Internet (IPv4),
    server.sin_addr.s_addr = INADDR_ANY;        //Die Kommunikationsschnittstelle des Hosts. Normalerweise die IP-Adresse.
    server.sin_port = htons(SERVER_PORT);       //Transformiert die Portnummer in Network-Byte-Order.

    if (bind(sock, (struct sockaddr*)&server, sizeof (server)) < 0) {       //Bei einem Fehler würde bind() -1 zurückgeben
        error_exit("Kann das Socket nicht \"binden\"");
    }
}

//Warteschlange, die auf eingehende Verbindungswünsche eines Clients wartet.
void listen_socket() {
    if (listen(sock, 5) == -1) {
        error_exit("Fehler bei listen");
    }
}

//Bearbeitet die Verbindungswünsche von Clients.
//Der Aufruf von accept() blockiert so lange, bis ein Client Verbindung aufnimmt.
void accept_socket() {
    struct sockaddr_in client;
    client.sin_family = AF_INET;                //AF_INET steht für Adress Family Internet (IPv4),
    client.sin_addr.s_addr = INADDR_ANY;        //Die Kommunikationsschnittstelle des Hosts. Normalerweise die IP-Adresse.
    client.sin_port = htons(SERVER_PORT);       //Transformiert die Portnummer in Network-Byte-Order.

    socklen_t client_length = sizeof (client);

    fileDescriptor = accept(sock, (struct sockaddr*)&client, &client_length);  //Rückgabewert von accept() ein neues Socket
    if (fileDescriptor == -1) {             //Bei einem Fehler würde accept() -1 zurückgeben
        error_exit("Fehler bei accept");
    }
}

//Socket schließen
void close_socket(int socket) {
    close(socket);
}


int main() {

    create_socket();
    bind_socket();
    listen_socket();

    while (1) {
        accept_socket();
        send(fileDescriptor, "Hallo", sizeof("Hallo"), 0);
        close_socket(fileDescriptor);
    }
    return EXIT_SUCCESS;
}
