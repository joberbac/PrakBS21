#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <printf.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "sub.h"

#define PORT 5678
#define BACKLOG 5       //Länge der Warteschlange für Clients
#define MAX_KEY_VALUE_STORE_SIZE 10     //Größe des Key Value Stores (Anzahl der Keys)
#define SEGSIZE sizeof(struct key_value_store[MAX_KEY_VALUE_STORE_SIZE])

int main() {

    struct input in;

    int exit;

    int sock_fd;            //Rendezvous-Descriptor, Rückgabewert des Sockets
    int connection_fd;      //Verbindungsdescriptor

    int id;                 //id für das Shared Memory
    struct key_value_store *shar_mem;          //mit *shar_mem können die im Shared Memory gespeicherten Werte verändert werden

    int sem_id;

    sock_fd = create_socket(AF_INET, SOCK_STREAM, 0);     //AF_INET = IPv4, SOCK_STREAM = TCP (verbindungsorientiert), 0 = Standartprotokoll
    bind_socket(&sock_fd, INADDR_ANY, PORT);            //INADDR_ANY = Adresse bestehend aus IP-Adresse und Port
    listen_socket(&sock_fd, BACKLOG);
    printf("Server running...\n");

    id = shmget(IPC_PRIVATE, SEGSIZE, IPC_CREAT|0777);      //neues Segment anlegen, IPC_PRIVATE = Key wird vom Betriebssystem selbst erzeugt, IPC_CREAT|0777 = Anlegen eines neuen Segments mit uneingeschränktem Zugriff
    if (id == -1)
        error_exit("shared memory");

    shar_mem = (struct key_value_store*)shmat(id, 0, 0);          //attach, Segment an Adressraum des aufrufenden Prozesses anhängen, erste 0 Pointer auf Adresse des Segments (0 bedeutet System soll selbst wählen), zweite 0 steht für flags

    sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT|0666);        //Anlegen einer neuen Semaphorengruppe, IPC_PRIVATE = Key wird vom Betriebssystem selbst erzeugt, 1 steht für die Anzahl der Semaphoren in der Gruppe, IPC_CREAT|0777 = Anlegen einer neuen Gruppe mit uneingeschränktem Zugriff
    if (sem_id == -1)
        error_exit("Error at semaphor");

    semctl(sem_id, 0, SETVAL, 1);      //Semaphor wird auf 1 gesetzt

    while(1) {

        accept_socket(&sock_fd, &connection_fd);
        printf("New Client connected\n");
        write(connection_fd, "Hello Client\n", sizeof ("Hello Client\n"));

        int pid = fork();
        if (pid < 0)            //Fehler beo fork()
            error_exit("fork");
        else if (pid == 0) {        //Kindprozess

            struct sembuf enter, leave;
            enter.sem_num = leave.sem_num = 0;          //Semaphor 0 in der Gruppe
            enter.sem_flg = leave.sem_flg = SEM_UNDO;
            enter.sem_op = -1;                          //Down-Operation, Semaphore wird um diesen Wert vermindert
            leave.sem_op = 1;                           //Up-Operation, Semaphore wird um diesen Wert erhöht

            while (exit != 2) {
                in = *input_func(&connection_fd);
                if (semop(sem_id, &enter, 1) < 0)       //Eintritt in kritischen Bereich
                    error_exit("Error at semop");
                exit = execCommand(&in, &connection_fd, shar_mem);
                if (semop(sem_id, &leave, 1))       //Verlassen des kritischen Bereichs
                    error_exit("Error at semop");
            }
        }
        else        //Vaterprozess
            close_socket(&connection_fd);
    }
    shmdt(shar_mem);            //detach, Segment wird aus Adressraum des aufrufenden Prozesses entfernt
    shmctl(id, IPC_RMID, 0);    //control, hier für das Löschen eines Shared Memory Segments
    semctl(sem_id, 1, IPC_RMID);    //Löschen des Semaphor

    close_socket(&sock_fd);
}