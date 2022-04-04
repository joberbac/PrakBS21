#include <stdio.h>

int main() {
    int programRunning = 1;
    while (programRunning != 0) {
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

        switch (command) {
            case "GET":  //TODO GET akzeptiert einen key und liefert den zugehörigen value in dem Format 'GET:key:value'. Ist der key unbekannt wird statt dem value der Fehler "key_nonexistent" ausgegeben.
            case "PUT": // TODO akzeptiert einen key und einen value. Der Befehl liefert das gespeicherte Paar im Folgenden Format zurück: 'PUT:key:value'
            case "DEL": // TODO akzeptiert einen key und löscht das Paar aus der Datenhaltung. War der key vorhanden, wird 'DEL:key:key_deleted' ausgegeben. War der key nicht vorhanden, wird 'DEL:key:key_nonexistent' ausgegeben.
            case "QUIT":
                return 0; //TODO beendet die Socket Verbindung und terminiert.
                */
        return 0;
    }
}



/*
 *
 */
