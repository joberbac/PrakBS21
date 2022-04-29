#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include "sub.h"

#define PORT 5678
#define BACKLOG 5       //Länge der Warteschlange für Clients
#define MAX_KEY_VALUE_STORE_SIZE 10     //Größe des Key Value Stores (Anzahl der Keys)
#define SEGSIZE_KEY_VALUE_STORE sizeof(struct key_value_store[MAX_KEY_VALUE_STORE_SIZE])
#define MAX_SUB_SIZE 10
#define SEGSIZE_PUB_SUB sizeof(struct subscribe[MAX_SUB_SIZE])

int main() {

    struct input in;
    int returnExecCommand;


    /*SERVER*/
    int sock_fd;            //Rendezvous-Descriptor, Rückgabewert des Sockets
    int connection_fd;      //Verbindungsdescriptor
    sock_fd = create_socket(AF_INET, SOCK_STREAM, 0);     //AF_INET = IPv4, SOCK_STREAM = TCP (verbindungsorientiert), 0 = Standartprotokoll
    bind_socket(&sock_fd, INADDR_ANY, PORT);            //INADDR_ANY = Adresse bestehend aus IP-Adresse und Port
    listen_socket(&sock_fd, BACKLOG);
    printf("Server running...\n");
    /*SERVER ENDE*/


    /*SHARED MEMORY FÜR KEY VALUE STORE*/
    int shm_id;                                                 //id für das Shared Memory
    struct key_value_store *shar_mem;                           //mit *shar_mem können die im Shared Memory gespeicherten Werte verändert werden
    shm_id = shmget(IPC_PRIVATE, SEGSIZE_KEY_VALUE_STORE, IPC_CREAT | 0777);    //neues Segment anlegen, IPC_PRIVATE = Key wird vom Betriebssystem selbst erzeugt,
                                                                // IPC_CREAT|0777 = Anlegen eines neuen Segments mit uneingeschränktem Zugriff
    if (shm_id == -1)
        error_exit("Error at shared memory");
    shar_mem = (struct key_value_store*)shmat(shm_id, 0, 0);    //attach, Segment an Adressraum des aufrufenden Prozesses anhängen, erste 0 Pointer auf Adresse des Segments (0 bedeutet System soll selbst wählen), zweite 0 steht für flags
    /*SHARED MEMORY FÜR KEY VALUE STORE ENDE*/


    /*SHARED MEMORY FÜR PUB/SUB*/
    int sub_id;
    struct subscribe *sub;
    sub_id = shmget(IPC_PRIVATE, SEGSIZE_PUB_SUB, IPC_CREAT | 0777);
    if (shm_id == -1)
        error_exit("Error at shared memory");
    sub = (struct subscribe*)shmat(sub_id, 0, 0);
    /*SHARED MEMORY FÜR PUB/SUB ENDE*/


    /*SEMAPHORE*/
    int sem_id;                                             //id für das Semaphor
    sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0777);        //Anlegen einer neuen Semaphorengruppe, IPC_PRIVATE = Key wird vom Betriebssystem selbst erzeugt,
                                                            //1 steht für die Anzahl der Semaphoren in der Gruppe,
                                                            //IPC_CREAT|0777 = Anlegen einer neuen Gruppe mit uneingeschränktem Zugriff
    if (sem_id == -1) {
        error_exit("Error at semaphor");
    }
    semctl(sem_id, 0, SETVAL, 1);                           //Semaphor wird auf 1 gesetzt
    /*SEMAPHORE ENDE*/


    /*NACHRICHTENWARTESCHLANGE*/
    int msg_id = msgget(IPC_PRIVATE, IPC_CREAT | 0777);
    if (shm_id == -1)
        error_exit("Error at message queue");
    /*NACHRICHTENWARTESCHLANGE ENDE*/


    while(1) {

        accept_socket(&sock_fd, &connection_fd);
        printf("New Client connected\n");
        write(connection_fd, "Hello Client\n", sizeof ("Hello Client\n"));

        int pid = fork();
        if (pid < 0) {            //Fehler bei fork()
            error_exit("Error at fork");
        }
        else if (pid > 0) {        //Vaterprozess

        }
        else if (pid == 0) {        //Kindprozess

            int pid2 = fork();
            if (pid2 < 0) {
                error_exit("Error at fork");
            }

            if (pid2 == 0) {
                while (1) {
                    receiveMessage(&msg_id, &connection_fd);
                }
            }

            if (pid2 > 0) {
                struct sembuf enter, leave;
                enter.sem_num = leave.sem_num = 0;          //Semaphor 0 in der Gruppe
                enter.sem_flg = leave.sem_flg = SEM_UNDO;
                enter.sem_op = -1;                          //Down-Operation, Semaphore wird um diesen Wert vermindert
                leave.sem_op = 1;                           //Up-Operation, Semaphore wird um diesen Wert erhöht

                int blocked = 0;

                do {
                    in = *input_func(&connection_fd);

                    if (blocked == 0) {
                        if (semop(sem_id, &enter, 1) < 0)       //Eintritt in kritischen Bereich
                            error_exit("Error at semop");
                    }
                    if (blocked == 0) {
                        if (semop(sem_id, &leave, 1))       //Verlassen des kritischen Bereichs
                            error_exit("Error at semop");
                    }

                    returnExecCommand = execCommand(&in, &connection_fd, shar_mem, sub, &msg_id);

                    if (returnExecCommand == 7) {
                        if (semop(sem_id, &enter, 1) < 0)       //Eintritt in kritischen Bereich
                            error_exit("Error at semop");
                        blocked = 1;
                    }

                    if (returnExecCommand == 8) {
                        if (semop(sem_id, &leave, 1))       //Verlassen des kritischen Bereichs
                            error_exit("Error at semop");
                        blocked = 0;
                    }

                } while (returnExecCommand != 2);
            }
        }

    }
    shmdt(shar_mem);            //detach, Segment wird aus Adressraum des aufrufenden Prozesses entfernt
    shmctl(shm_id, IPC_RMID, 0);    //control, hier für das Löschen eines Shared Memory Segments
    semctl(sem_id, 1, IPC_RMID);    //Löschen des Semaphor

    shmdt(sub);
    shmctl(sub_id, IPC_RMID, 0);

    msgctl(msg_id, IPC_RMID, 0);    //Löschen der Nachrichtenwarteschlange

    return 0;
}