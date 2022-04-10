#include <stdio.h>
#include <string.h>

#include "sub.h"

#define MAX_KEY_LENGTH 10
#define MAX_VALUE_LENGTH 100
#define MAX_KEY_VALUE_STORE_SIZE 10

struct key_value_store {
    char key_s[MAX_KEY_LENGTH];
    char value_s[MAX_VALUE_LENGTH];
};

struct key_value_store key_val[MAX_KEY_VALUE_STORE_SIZE];


int put(char *key, char *value) {
    if (strcmp(value, "") == 0) {
        printf("No Value\n");
        return 0;
    }

    for (int i = 0; i < MAX_KEY_VALUE_STORE_SIZE; i++) {
        if (memcmp(key_val[i].key_s, key, sizeof (key_val[i].key_s)-2) == 0) {
            strcpy(key_val[i].value_s, value);
            printf("PUT:%s:%s:override\n", key_val[i].key_s, key_val[i].value_s);
            return 1;
        }
    }
    for (int i = 0; i < MAX_KEY_VALUE_STORE_SIZE; i++) {
        if (strlen(key_val[i].key_s) == 0) {
            strcpy(key_val[i].key_s, key);
            strcpy(key_val[i].value_s, value);
            printf("PUT:%s:%s\n", key_val[i].key_s, key_val[i].value_s);
            return 0;
        }
    }
    error_exit("Fehler bei der Eingabe\n");
    return -2;
}

int get(char *key) {
    for (int i = 0; i < MAX_KEY_VALUE_STORE_SIZE; i++) {
        if (strcmp(key_val[i].key_s, key) == 0) {
            printf("GET:%s:%s\n", key_val[i].key_s, key_val[i].value_s);
            return 0;
        }
    }
    printf("GET:%s:key_nonexistent\n", key);
    return -1;
}

int del(char *key) {
    char temp[MAX_KEY_LENGTH];
    for (int i = 0; i < MAX_KEY_VALUE_STORE_SIZE; i++) {
        //if (memcmp(key_val[i].key_s, key, strlen(key_val[i].key_s)) == 0) {
        if (strcmp(key_val[i].key_s, key) == 0) {
            strcpy(temp, key_val[i].key_s);
            strcpy(key_val[i].key_s, "");
            strcpy(key_val[i].value_s, "");
            printf("DEL:%s:key_deleted\n", temp);
            return 0;
        }
    }
    printf("DEL:%s:key_nonexistent\n", key);
    return -1;
}

//WRITE
