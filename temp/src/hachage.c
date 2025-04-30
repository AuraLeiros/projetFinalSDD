#include "../include/hachage.h"


unsigned long simple_hash(const char* str){
    if (!str || str[0] == '\0'){
        fprintf(stderr, "Erreur dans les parametres\n");
        return ULONG_MAX;
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

HashMap* hashmap_create(){

    /* Allocation memoire d'un nouveau HashMap */
    HashMap* h = (HashMap*)malloc(sizeof(HashMap));
    if (!h){
        fprintf(stderr, "Erreur d'allocation memoire d'un nouveau HashMap\n");
        goto erreur;
    }

    h->size = TABLE_SIZE;

    /* Allocation memoire et initialisation (a 0 / NULL) des elements du HashMap */
    h->table = (HashEntry*)calloc(TABLE_SIZE, sizeof(HashEntry));
    if (!h->table){
        fprintf(stderr, "Erreur dans l'initialization memoire de la table du HashMap");
    }

    return h;

    erreur:
        if (h) hashmap_destroy(h);
        return NULL;
}


int hashmap_insert(HashMap* map, const char* key, void* value){
    if (!map || (!key || key[0] == '\0') || !value){
        fprintf(stderr, "Erreur dans les parametres");
        return EXIT_FAILURE;    
    }

    /* Calcul du premier hash */
    unsigned long hash = simple_hash(key);
    unsigned long originalHash = hash;

    do {

        if ((map->table[hash].value == NULL) || (map->table[hash].value == TOMBSTONE)){

            /* On connait pas la taille, on ne peut pas faire une deep copy de la valeur */
            map->table[hash].key = strdup(key);
            if (!map->table[hash].key){
                fprintf(stderr, "Erreur dans l'allocation d' une nouvelle cle\n");
                return EXIT_FAILURE;
            }

            map->table[hash].value = value;
            return EXIT_SUCCESS;
        }

        /* Si la case est deja rempli mais la cle est identique */
        if (strcmp(map->table[hash].key, key) == 0){
            /* TODO: Possible memory leak, we may need to deallocate the previous value if error*/
            map->table[hash].value = value;
            return EXIT_SUCCESS;
        }

        /* Linear probing pour trouver la premiere case vide */
        hash = (++hash) % TABLE_SIZE;

    } while (hash != originalHash);

    printf("L'element n'a pas pu etre insere\n");
    return EXIT_FAILURE;
}

void* hashmap_get(HashMap* map, const char* key){
    if (!map || (!key || key[0] == '\0')){
        fprintf(stderr, "Erreur dans les parametres\n");
        return NULL;
    }

    unsigned long hash = simple_hash(key);
    unsigned long originalHash = hash;

    do {

        /* Verifier si l'element courant est l'element recherche */
        if (strcmp(map->table[hash].key, key) == 0) return map->table[hash].value;

        /* Linear probing pour avancer dans le HashMap*/
        hash = (++hash) % TABLE_SIZE;

    } while (hash != originalHash);

    printf(stderr, "Aucun element avec la cle utilise a ete trouve\n");

    return NULL;
}

int hashmap_remove(HashMap* map, const char* key){
    if (!map || (!key || key[0] == '\0')){
        fprintf(stderr, "Erreur dans les parametres\n");
        return EXIT_FAILURE;
    }

    unsigned long hash = simple_hash(key);
    unsigned long originalHash = simple_hash(key);

    do {

        /* On a trouve l'element a effacer */
        if (strcmp(map->table[hash].key, key) == 0){

            /* Liberation de la memoire alloue a la cle */
            free(map->table[hash].key);

            /* Replacement de la valeur par un TOMBSTONE */
            map->table[hash].value = TOMBSTONE;

            return EXIT_SUCCESS;
        }

        /* Linear probing pour avancer dans le HashMap */
        hash = (++hash) % TABLE_SIZE;

    } while (hash != originalHash);

    printf(stderr, "Aucun element avec la cle utilise a ete trouve\n");

    return EXIT_FAILURE;
}


void hashmap_destroy(HashMap* map){
    if (!map) return;

    /* Liberation de tous les elements dans le tableau */
    /* On ne peut pas liberer les value vu qu'on sait pas s'on été alloues */
    if (map->table){
        for (int i=0; i < TABLE_SIZE; i++){
            if (map->table[i].key) free(map->table[i].key);
        }

        free(map->table);
    }

    free(map);

    return;
}