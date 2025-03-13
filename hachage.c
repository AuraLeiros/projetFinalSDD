#include "hachage.h"

/* Implementation avec l'algorithme FNV-1a */
unsigned long simple_hash(const char* str){
    /* On suppose que la machine d'execution est de 64-bits */
    
    if (!str || str[0] == '\0'){
        fprintf(stderr, "Aucune chaine de caracteres a ete passe en parametre\n");
        return -1;
    }

    uint64_t hash = FNV_OFFSET_BASIS_64;
    
    while (*str){

        /* Melanger les bits de hash avec les bits moins significatifs de str */
        hash ^= (uint8_t)(*str++);

        /* Effet avalanche */
        hash *= FNV_PRIME_64;
    }

    return (unsigned long)hash % TABLE_SIZE;
}

/* TO CHECK*/
/*
unsigned long simple_hash(const char* str){
    if (!str){
        fprintf(stderr, "Aucune chaine de caracteres a ete passe en parametre\n");
        return -1;
    }

    unsigned long sum = 0;
    char* idx = str;

    while(*idx) {
        sum += (unsigned long)(*idx);
        idx++;
    }

    const double A = (sqrt(5) - 1) / 2;
    double kA = sum * A;
    double multip = m * (kA - floor(kA));
    unsigned long hash = floor(multip);

    return hash % TABLE_SIZE;
}
*/

HashMap* hashmap_create(){

    /* Allocation de memoire pour un nouveau hashmap */
    HashMap* newHashMap = (HashMap*)malloc(sizeof(HashMap));
    if (!newHashMap){
        fprintf(stderr, "Erreur d'allocation memoire d'un nouveau HashMap\n");
        return NULL;
    }

    /* Initialization avec calloc pour avoir des valeurs NULL */
    newHashMap->size = TABLE_SIZE;
    newHashMap->table = (HashEntry*)calloc(TABLE_SIZE, sizeof(HashEntry));
    if (!newHashMap->table){
        fprintf(stderr, "Erreur d'allocation memoire d'un nouveau tableau HashEntry\n");
        free(newHashMap);
        return NULL;
    }

    return newHashMap;
}

int hashmap_insert(HashMap* map, const char* key, void* value){
    if (!map || !key || !value){
        fprintf(stderr, "Erreur dans les parametres\n");
        return EXIT_FAILURE;
    }

    unsigned long hash = simple_hash(key);

    while (hash <= TABLE_SIZE){
        if (map->table[hash].value == NULL || map->table[hash].value == TOMBSTONE){
            map->table[hash].key = strdup(key);
            map->table[hash].value = value;
            return EXIT_SUCCESS;
        }

        hash++;
    }

    fprintf(stderr, "L'element n'a pas pu etre insere dans la table de hachage\n");

    return EXIT_FAILURE;
    
}

void* hashmap_get(HashMap* map, const char* key) {
    //on part du principe que cet element est forcement dans la table?
    if (!map || !key){
        fprintf(stderr, "Erreur dans les parametres\n");
        return NULL;
    }

    unsigned long hash = simple_hash(key);
    while (hash <= TABLE_SIZE) {
        if (map->table[hash].key == key) {
            return map->table[hash].value;
        }

        hash++;
    }

    fprintf(stderr, "L'element n'a pas pu etre trouve dans la table de hachage\n");

    return NULL;

}

int hashmap_remove(HashMap* map, const char* key) {
    if (!map || !key) {
        fprintf(stderr, "Erreur dans les parametres\n");
        return EXIT_FAILURE;
    }

    unsigned long hash = simple_hash(key);
    while (hash <= TABLE_SIZE) {
        if (map->table[hash].key == key) {
            map->table[hash].value = TOMBSTONE;
            return EXIT_SUCCESS;
        }

        hash++;
    }

    fprintf(stderr, "L'element n'a pas pu etre trouve dans la table de hachage\n");

    return EXIT_FAILURE;
}

void hashmap_destroy(HashMap* map) {
    if (map) {
        if (!map->table) {
            free(map->table);
        } else {
            for (int i = 0; i < map->size; i++) {
                free(map->table[i].key);
                free(map->table[i].value);
            }
            free(map->table);
        }
    }
    free(map);
    return;
}