#ifndef HACHAGE_H
#define HACHAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "macros.h"

typedef struct hashentry {
    char* key;
    void* value;
} HashEntry;

typedef struct hashmap {
    int size;
    HashEntry* table;
} HashMap;


/* Hashage d'une clé pour l'obtention d'une case dans la table de hachage */
unsigned long simple_hash(const char* str);

/* Creation d'un nouveau HashMap */
HashMap* hashmap_create();

/* Insertion d'un nouvel element dans le HashMap */
/* La fonction ecrase la valeur si la cle est deja utilise */
int hashmap_insert(HashMap* map, const char* key, void* value);

/* Recuperer un element du HashMap */
/* Retourne NULL si la cle n'est pas presente */
void* hashmap_get(HashMap* map, const char* key);

/* Effacer un element du HashMap */
/* EXIT_SUCCESS si l'element a ete efface, EXIT_FAILURE si non*/
int hashmap_remove(HashMap* map, const char* key);

/* Effacer toute la memoire alloue a un HashMap */
/* Sans liberer la memoire des valeurs*/
void hashmap_destroy(HashMap* map);


#endif