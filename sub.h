#ifndef SUB_H_
#define SUB_H_

#define MAX_COMMAND_LENGTH 10
#define MAX_KEY_LENGTH 10
#define MAX_VALUE_LENGTH 100

#define MAX_KEY_LENGTH 10
#define MAX_VALUE_LENGTH 100

struct key_value_store {
    char key_s[MAX_KEY_LENGTH];
    char value_s[MAX_VALUE_LENGTH];
};

struct input {
    char command_s[MAX_COMMAND_LENGTH];
    char key_s[MAX_KEY_LENGTH];
    char value_s[MAX_VALUE_LENGTH];
};

void error_exit(char *error_message);
int create_socket(int af, int type, int protocol);
void bind_socket(int *sock, unsigned long address, unsigned short port);
void listen_socket(int *sock);
void accept_socket(int *sock, int *fileDescriptor);
void close_socket(int *sock);

struct input * input_func(int *fileDescriptor);
int execCommand(struct input *in, int *fileDescriptor, struct key_value_store *key_val);


#endif