#include <stdio.h>
#include <stdlib.h>

#ifndef ARRAY_H
#define ARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int key;
    char *value;
} Entry;

typedef struct
{
    Entry *array;
    size_t size;
} Array;

void Array_Init(Array *array);
void Array_Add(Array *array, Entry item);
void Array_Delete(Array *array, int index);
void Array_Free(Array *array);
Entry getEntry(Array *a, int key);


#ifdef __cplusplus
}
#endif

#endif /* ARRAY_H */