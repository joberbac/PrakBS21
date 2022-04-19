#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/mman.h>
#include "sub.h"
#include "keyValStore.h"

#define BUFSIZE 1024 // Größe des Buffers
#define PORT 5678


int main() {

    int socket_fd; // Rendezvous-Descriptor / Rückgabewert des Sockets → File descriptor
    int cfd; // Connection-Descriptor

    socket_fd = create_socket(AF_INET, SOCK_STREAM, 0);
    bind_socket(&socket_fd, INADDR_ANY, PORT);
    listen_socket(&socket_fd);
    printf("Socket is ready to accept connections\n");

    struct key_value_store *shared_memory = mmap(NULL, 1000, PROT_READ | PROT_WRITE,
                                                 MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    do {

        accept_socket(&socket_fd, &cfd); //TODO perhaps put accept somewhere else to be able to terminate
        printf("New Client connected\n");
        send(cfd, "Hello Client\n", sizeof("Hello Client\n"), 0);

        if (fork() == 0) { //neuer Kindprozess

            while (execCommand(input_func(&cfd), &cfd, shared_memory) != 2) {

            }
        } else
            close(cfd); //TODO break in Loop inorder to be able to close Rendezvous Descriptor and Terminate
    } while (1);

    //TODO close and empty shared memory
    // Rendezvous Descriptor schließen
    close(socket_fd);
    exit(0);

}
