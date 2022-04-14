#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <printf.h>
#include "sub.h"

int main() {

    int sock, fileDescriptor;
    sock = create_socket(AF_INET, SOCK_STREAM, 0);
    bind_socket(&sock, INADDR_ANY, 5678);
    listen_socket(&sock);
    printf("Server running...\n");


    do {

        accept_socket(&sock, &fileDescriptor);
        printf("New Client connected\n");
        send(fileDescriptor, "Hello Client\n", sizeof ("Hello Client\n"), 0);

        if (fork() == 0) {
            while (execCommand(input_func(&fileDescriptor), &fileDescriptor) != 2) {

            }
        }
        else
            close_socket(&fileDescriptor);
    } while (1);

}