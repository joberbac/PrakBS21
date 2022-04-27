#include <stdio.h>
#include <string.h>

#include "sub.h"

#define MAX_KEY_LENGTH 10
#define MAX_VALUE_LENGTH 100
#define MAX_KEY_VALUE_STORE_SIZE 10
#define MAX_OUTPUT_LENGTH 100


int put(char *key, char *value, int *connection_fd, struct key_value_store *shar_mem, struct subscribe *sub) {
    char message[MAX_OUTPUT_LENGTH] = {};
    if (strcmp(key, "") == 0) {
        snprintf(message, sizeof message, "No key\n");
        output(connection_fd, message);
        return 1;
    }

    if (strcmp(value, "") == 0) {
        snprintf(message, sizeof message, "No value\n");
        output(connection_fd, message);
        return 1;
    }

    for (int i = 0; i < MAX_KEY_VALUE_STORE_SIZE; i++) {
        if (strcmp(shar_mem[i].key_s, key) == 0) {
            strcpy(shar_mem[i].value_s, value);
            snprintf(message, sizeof message, "PUT:%s:%s:override\n", shar_mem[i].key_s, shar_mem[i].value_s);
            notify(sub, shar_mem, key);
            output(connection_fd, message);
            return 0;
        }
    }

    for (int i = 0; i < MAX_KEY_VALUE_STORE_SIZE; i++) {
        if (strlen(shar_mem[i].key_s) == 0) {
            strcpy(shar_mem[i].key_s, key);
            strcpy(shar_mem[i].value_s, value);
            snprintf(message, sizeof message, "PUT:%s:%s\n", shar_mem[i].key_s, shar_mem[i].value_s);
            notify(sub, shar_mem, key);
            output(connection_fd, message);
            return 0;
        }
    }
    error_exit("Error at input\n");
    return -1;
}

int get(char *key, int *connection_fd, struct key_value_store *shar_mem) {
    char message[MAX_OUTPUT_LENGTH] = {};
    if (strcmp(key, "") == 0) {
        snprintf(message, sizeof message, "No key\n");
        output(connection_fd, message);
        return 1;
    }

    for (int i = 0; i < MAX_KEY_VALUE_STORE_SIZE; i++) {
        if (strcmp(shar_mem[i].key_s, key) == 0) {
            snprintf(message, sizeof message, "GET:%s:%s\n", shar_mem[i].key_s, shar_mem[i].value_s);
            output(connection_fd, message);
            return 0;
        }
    }
    snprintf(message, sizeof message, "GET:%s:key_nonexistent\n", key);
    output(connection_fd, message);
    return -1;
}

int del(char *key, int *connection_fd, struct key_value_store *shar_mem, struct subscribe *sub) {
    char message[MAX_OUTPUT_LENGTH] = {};
    char temp[MAX_KEY_LENGTH];
    if (strcmp(key, "") == 0) {
        snprintf(message, sizeof message, "No key\n");
        output(connection_fd, message);
        return 1;
    }

    for (int i = 0; i < MAX_KEY_VALUE_STORE_SIZE; i++) {
        if (strcmp(shar_mem[i].key_s, key) == 0) {
            strcpy(temp, shar_mem[i].key_s);
            strcpy(shar_mem[i].key_s, "");
            strcpy(shar_mem[i].value_s, "");
            snprintf(message, sizeof message, "DEL:%s:key_deleted\n", temp);
            notify(sub, shar_mem, key);
            output(connection_fd, message);
            return 0;
        }
    }
    snprintf(message, sizeof message, "DEL:%s:key_nonexistent\n", key);
    output(connection_fd, message);
    return -1;
}





