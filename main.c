#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <printf.h>
#include <sys/mman.h>
#include "sub.h"


int main() {

    int sock, fileDescriptor;
    sock = create_socket(AF_INET, SOCK_STREAM, 0);
    bind_socket(&sock, INADDR_ANY, 5678);
    listen_socket(&sock);
    printf("Server running...\n");

    struct key_value_store *shared_memory = mmap(NULL, 1000, PROT_READ | PROT_WRITE,
                                                MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    do {

        accept_socket(&sock, &fileDescriptor);
        printf("New Client connected\n");
        send(fileDescriptor, "Hello Client\n", sizeof ("Hello Client\n"), 0);

        if (fork() == 0) {

            while (execCommand(input_func(&fileDescriptor), &fileDescriptor, shared_memory) != 2) {

            }
        }
        else
            close_socket(&fileDescriptor);
    } while (1);

}