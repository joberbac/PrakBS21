#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Array/Array.h"
#include "Store.h"

Array valueStore;

void init(){
    Array_Init(&valueStore);
}

int put(int key, char *value) {

}

int get(int key, char *res) {
    Entry searchResult = getEntry(&valueStore, key);
    if (searchResult.key == -1) {
        printf("No such entry");
        return -1;
    }
    else {
        strcpy(res, searchResult.value);
        return 0;
    }
}

int del(int key) {

}