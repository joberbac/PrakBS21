#ifndef KEYVALSTORE_H_
#define KEYVALSTORE_H_

#define MAX_KEY_LENGTH 10
#define MAX_VALUE_LENGTH 100

struct key_value_store {
    char key_s[MAX_KEY_LENGTH];
    char value_s[MAX_VALUE_LENGTH];
};

struct subscribe{
    int pid;
    char key_s[MAX_KEY_LENGTH];
};

int put(char *key, char *value, int *connection_fd, struct key_value_store *shar_mem, struct subscribe *sub);
int get(char *key, int *connection_fd, struct key_value_store *shar_mem);
int del(char *key, int *connection_fd, struct key_value_store *shar_mem, struct subscribe *sub);

#endif