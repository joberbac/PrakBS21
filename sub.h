#ifndef SUB_H_
#define SUB_H_

#define MAX_COMMAND_LENGTH 10
#define MAX_KEY_LENGTH 10
#define MAX_VALUE_LENGTH 100


struct key_value_store {
    char key_s[MAX_KEY_LENGTH];
    char value_s[MAX_VALUE_LENGTH];
};

struct input{
    char command_s[MAX_COMMAND_LENGTH];
    char key_s[MAX_KEY_LENGTH];
    char value_s[MAX_VALUE_LENGTH];
};

struct subscribe{
    int pid;
    char key_s[MAX_KEY_LENGTH];
};


void error_exit(char *error_message);
int create_socket(int af, int type, int protocol);
void bind_socket(int *sock_fd, unsigned long address, unsigned short port);
void listen_socket(int *sock_fd, int backlog);
void accept_socket(int *sock_fd, int *connection_fd);
void close_socket(int *sock_fd);

void output(int *connection_fd, char *string);
struct input * input_func(int *connection_fd);
int execCommand(struct input *in, int *connection_fd, struct key_value_store *shar_mem, struct subscribe *sub, int *msg_id);

int subscribe(struct subscribe *sub, char *key, int *connection_fd);
int unsub(struct subscribe *sub, char *key, int *connection_fd);
void notify(struct subscribe *sub, struct key_value_store *shar_mem, char *key, int *msg_id, char *string);

void receiveMessage(int *msg_id, int *connection_fd);

#endif