#include "hachage.h"

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
    double kA = cle * A;
    double multip = m * (kA - floor(kA));
    unsigned long hash = floor(multip);

    return hash % TABLE_SIZE;
}

HashMap* hashmap_create(){
    HashMap* newHashMap = (HashMap*)malloc(sizeof(HashMap));
    if (!newHashMap){
        fprintf(stderr, "Erreur d'allocation memoire d'un nouveau HashMap\n");
        return NULL;
    }

    newHashMap->size = TABLE_SIZE;
    newHashMap->table = (HashEntry*)malloc(TABLE_SIZE * sizeof(HashEntry));
    if (!newHashMap->table){
        fprintf(stderr, "Erreur d'allocation memoire d'un nouveau tableau HashEntry\n");
        free(newHashMap);
        return NULL;
    }

    return newHashMap;
}
