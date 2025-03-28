#ifndef HACHAGE_H
#define HACHAGE_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "macros.h"

typedef struct hashentry {
    char* key;
    void* value;
} HashEntry;

typedef struct hashmap {
    int size;
    HashEntry* table;
} HashMap;


unsigned long simple_hash(const char* str);

HashMap* hashmap_create();

int hashmap_insert(HashMap* map, const char* key, void* value);

void* hashmap_get(HashMap* map, const char* key);

int hashmap_remove(HashMap* map, const char *key);

void hashmap_destroy(HashMap* map);

#endif