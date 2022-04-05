#include <stdio.h>
#include <stdlib.h>
#include "Array.h"

/* Initialise an empty array */
void Array_Init(Array *array) {
    Entry *entry_pointer;

    // ?
    entry_pointer = (Entry *) malloc(sizeof(Entry));

    if (entry_pointer == NULL) {
        printf("Unable to allocate memory, exiting.\n");
        free(entry_pointer);
        exit(0);
    } else {
        array->array = entry_pointer;
        array->size = 0;
    }
}

/* Dynamically add to end of an array */
void Array_Add(Array *array, Entry item) {
    Entry *entry_pointer;

    array->size += 1;

    entry_pointer = (Entry *) realloc(array->array, array->size * sizeof(Entry));

    if (entry_pointer == NULL) {
        printf("Unable to reallocate memory, exiting.\n");
        free(entry_pointer);
        exit(0);
    } else {
        array->array = entry_pointer;
        array->array[array->size - 1] = item;
    }
}

/* Delete from a dynamic array */
void Array_Delete(Array *array, int index) {
    int i;
    Array temp;
    Entry *entry_pointer;

    Array_Init(&temp);

    for (i = index; i < array->size; i++) {
        array->array[i] = array->array[i + 1];
    }

    array->size -= 1;

    for (i = 0; i < array->size; i++) {
        Array_Add(&temp, array->array[i]);
    }

    entry_pointer = (Entry *) realloc(temp.array, temp.size * sizeof(Entry));

    if (entry_pointer == NULL) {
        printf("Unable to reallocate memory, exiting.\n");
        free(entry_pointer);
        exit(0);
    } else {
        array->array = entry_pointer;
    }
}

/* Free an array */
void Array_Free(Array *array) {
    free(array->array);
    array->array = NULL;
    array->size = 0;
}

Entry getEntry(Array *a, int key) {
    int i;
    for (i = 0; i < a->size; i++) {
        if (key == a->array->key)
            return a->array[i];
    }
    Entry emptyElement;
    emptyElement.key = -1;
    return emptyElement;
}

