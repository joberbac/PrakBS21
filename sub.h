#ifndef SUB_H_
#define SUB_H_

void error_exit(char *error_message);
int create_socket(int af, int type, int protocol);
void bind_socket(int *sock, unsigned long address, unsigned short port);
void listen_socket(int *sock);
void accept_socket(int *sock, int *fileDescriptor);
void close_socket(int *sock);

int input(int *sock, int fileDescriptor);

#endif