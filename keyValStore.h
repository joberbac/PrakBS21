#ifndef KEYVALSTORE_H_
#define KEYVALSTORE_H_

int put(char *key, char *value, int *fileDescriptor);
int get(char *key, int *fileDescriptor);
int del(char *key, int *fileDescriptor);

#endif