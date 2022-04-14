#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include "sub.h"


#define MAX_KEY_LENGTH 10
#define MAX_VALUE_LENGTH 100
#define MAX_KEY_VALUE_STORE_SIZE 10
#define MAX_OUTPUT_LENGTH 100


/*struct key_value_store {
    char key_s[MAX_KEY_LENGTH];
    char value_s[MAX_VALUE_LENGTH];
};*/


int put(char *key, char *value, int fileDescriptor, struct key_value_store *key_val) {
    char output[MAX_OUTPUT_LENGTH] = {};
    if (strcmp(key, "") == 0) {
        snprintf(output, sizeof output, "No Key\n");
        printf("%s", output);
        send(fileDescriptor, output, sizeof (output), 0);
        return 1;
    }

    if (strcmp(value, "") == 0) {
        snprintf(output, sizeof output, "No Value\n");
        printf("%s", output);
        send(fileDescriptor, output, sizeof (output), 0);
        return 1;
    }

    for (int i = 0; i < MAX_KEY_VALUE_STORE_SIZE; i++) {
        if (strcmp(key_val[i].key_s, key) == 0) {
            strcpy(key_val[i].value_s, value);
            snprintf(output, sizeof output, "PUT:%s:%s:override\n", key_val[i].key_s, key_val[i].value_s);
            printf("%s", output);
            send(fileDescriptor, output, sizeof (output), 0);
            return 0;
        }
    }

    for (int i = 0; i < MAX_KEY_VALUE_STORE_SIZE; i++) {
        if (strlen(key_val[i].key_s) == 0) {
            strcpy(key_val[i].key_s, key);
            strcpy(key_val[i].value_s, value);
            snprintf(output, sizeof output, "PUT:%s:%s\n", key_val[i].key_s, key_val[i].value_s);
            printf("%s", output);
            send(fileDescriptor, output, sizeof (output), 0);
            return 0;
        }
    }
    error_exit("Fehler bei der Eingabe\n");
    return -1;
}

int get(char *key, int fileDescriptor, struct key_value_store *key_val) {
    char output[MAX_OUTPUT_LENGTH] = {};
    if (strcmp(key, "") == 0) {
        snprintf(output, sizeof output, "No Key\n");
        printf("%s", output);
        send(fileDescriptor, output, sizeof (output), 0);
        return 1;
    }

    for (int i = 0; i < MAX_KEY_VALUE_STORE_SIZE; i++) {
        if (strcmp(key_val[i].key_s, key) == 0) {
            snprintf(output, sizeof output, "GET:%s:%s\n", key_val[i].key_s, key_val[i].value_s);
            printf("%s", output);
            send(fileDescriptor, output, sizeof (output), 0);
            return 0;
        }
    }
    snprintf(output, sizeof output, "GET:%s:key_nonexistent\n", key);
    printf("%s", output);
    send(fileDescriptor, output, sizeof (output), 0);
    return -1;
}

int del(char *key, int fileDescriptor, struct key_value_store *key_val) {
    char output[MAX_OUTPUT_LENGTH] = {};
    char temp[MAX_KEY_LENGTH];
    for (int i = 0; i < MAX_KEY_VALUE_STORE_SIZE; i++) {
        if (strcmp(key_val[i].key_s, key) == 0) {
            strcpy(temp, key_val[i].key_s);
            strcpy(key_val[i].key_s, "");
            strcpy(key_val[i].value_s, "");
            snprintf(output, sizeof output, "DEL:%s:key_deleted\n", temp);
            printf("%s", output);
            send(fileDescriptor, output, sizeof (output), 0);
            return 0;
        }
    }
    snprintf(output, sizeof output, "DEL:%s:key_nonexistent\n", key);
    printf("%s", output);
    send(fileDescriptor, output, sizeof (output), 0);
    return -1;
}

