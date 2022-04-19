#ifndef SUB_H_
#define SUB_H_

#define MAX_COMMAND_LENGTH 10
#define MAX_KEY_LENGTH 10
#define MAX_VALUE_LENGTH 100

#include "keyValStore.h"


struct input {
    char command_s[MAX_COMMAND_LENGTH];
    char key_s[MAX_KEY_LENGTH];
    char value_s[MAX_VALUE_LENGTH];
};

void error_exit(char *error_message);
int create_socket(int af, int type, int protocol);
void bind_socket(int *socket_fd, unsigned long address, unsigned short port);
void listen_socket(int *socket_fd);
void accept_socket(int *socket_fd, int *cfd);
void close_process(int *port);

struct input * input_func(int *cfd);
int execCommand(struct input *in, int *cfd, struct key_value_store *key_val);


#endif
