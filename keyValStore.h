#ifndef KEYVALSTORE_H_
#define KEYVALSTORE_H_

int put(char *key, char *value);
int get(char *key);
int del(char *key);

#endif