#ifndef KEYVALSTORE_H_
#define KEYVALSTORE_H_

#define MAX_KEY_LENGTH 10
#define MAX_VALUE_LENGTH 100

struct key_value_store {
    char key_s[MAX_KEY_LENGTH];
    char value_s[MAX_VALUE_LENGTH];
};

int put(char *key, char *value, int fileDescriptor, struct key_value_store *key_val);
int get(char *key, int fileDescriptor, struct key_value_store *key_val);
int del(char *key, int fileDescriptor, struct key_value_store *key_val);

#endif
