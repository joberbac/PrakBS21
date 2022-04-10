#include <sys/socket.h>
#include <netinet/in.h>
#include <printf.h>
#include <stdlib.h>
#include "sub.h"
#include "keyValStore.h"

#define BUF 1024

int main() {

    int sock, fileDescriptor;
    sock = create_socket(AF_INET, SOCK_STREAM, 0);
    bind_socket(&sock, INADDR_ANY, 5678);
    listen_socket(&sock);

    while (1) {

        accept_socket(&sock, &fileDescriptor);

        do {

        } while (input(&sock, fileDescriptor) != 2);

    }
}