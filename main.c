#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFSIZE 1024 // Größe des Buffers
#define TRUE 1
#define ENDLOSSCHLEIFE 1
#define PORT 5678


int main() {

    int rfd; // Rendevouz-Descriptor
    int cfd; // Verbindungs-Descriptor

    struct sockaddr_in client; // Socketadresse eines Clients
    socklen_t client_len; // Länge der Client-Daten
    char in[BUFSIZE]; // Daten vom Client an den Server
    int bytes_read; // Anzahl der Bytes, die der Client geschickt hat


    // Socket erstellen
    rfd = socket(AF_INET, SOCK_STREAM, 0);
    if (rfd < 0) {
        fprintf(stderr, "socket konnte nicht erstellt werden\n");
        exit(-1);
    }


    // Socket Optionen setzen für schnelles wiederholtes Binden der Adresse
    int option = 1;
    setsockopt(rfd, SOL_SOCKET, SO_REUSEADDR, (const void *) &option, sizeof(int));


    // Socket binden
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    int brt = bind(rfd, (struct sockaddr *) &server, sizeof(server));
    if (brt < 0) {
        fprintf(stderr, "socket konnte nicht gebunden werden\n");
        exit(-1);
    }


    // Socket lauschen lassen
    int lrt = listen(rfd, 5);
    if (lrt < 0) {
        fprintf(stderr, "socket konnte nicht listen gesetzt werden\n");
        exit(-1);
    }

    while (ENDLOSSCHLEIFE) {

        // Verbindung eines Clients wird entgegengenommen
        cfd = accept(rfd, (struct sockaddr *) &client, &client_len);

        // Lesen von Daten, die der Client schickt
        bytes_read = read(cfd, in, BUFSIZE);
        switch (in) {
            case "GET"://TODO GET akzeptiert einen key und liefert den zugehörigen value in dem Format 'GET:key:value'. Ist der key unbekannt wird statt dem value der Fehler "key_nonexistent" ausgegeben.
            break;
            case "PUT": // TODO akzeptiert einen key und einen value. Der Befehl liefert das gespeicherte Paar im Folgenden Format zurück: 'PUT:key:value'
            break;
            case "DEL": // TODO akzeptiert einen key und löscht das Paar aus der Datenhaltung. War der key vorhanden, wird 'DEL:key:key_deleted' ausgegeben. War der key nicht vorhanden, wird 'DEL:key:key_nonexistent' ausgegeben.
            break;
            case "QUIT": //TODO beendet die Socket Verbindung und terminiert.
            break;

                // Zurückschicken der Daten, solange der Client welche schickt (und kein Fehler passiert)
        while (bytes_read > 0) {
            printf("sending back the %d bytes I received...\n", bytes_read);

            write(cfd, in, bytes_read);
            bytes_read = read(cfd, in, BUFSIZE);

        }
        close(cfd);
    }

    // Rendevouz Descriptor schließen
    close(rfd);

}

/*
* key und value akzeptieren alphanumerische Zeichen, keine Sonderzeichen.
* Der key darf keine Leerzeichen enthalten.
* INPUT-Format: [command:key:value]
 *
 * BSP:
    GET key1
    > GET:key1:key_nonexistent
    PUT key1 value1
    > PUT:key1:value1
    PUT key2 value2
    > PUT:key2:value2
    PUT key1 value3
    > PUT:key1:value3
    DEL key2
    > DEL:key2:key_deleted
    DEL key2
    > DEL:key2:key_nonexistent
    GET key1
    > GET:key1:value3
    QUIT
*/
/*char[] command;
printf("Enter Command (GET, PUT, DEL, QUIT):");
scanf("%c", &command);

        */
