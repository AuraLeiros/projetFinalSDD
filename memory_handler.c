#include "memory_handler.h"

MemoryHandler* memory_init(int size) {

    /* Allocation et initialisation du memory_handler */
    MemoryHandler* mem = malloc(sizeof(MemoryHandler));
    if (!mem) {
        fprintf(stderr, "Erreur d'allocation memoire d'un nouveau Memoryhandler\n");
        return NULL;
    }

    /* NOTE: la taille maximale de memoire que l'on peut allouer plus tard */
    mem->total_size = size; 

    /* Allocation et initialisation de la free_list */
    mem->free_list = malloc(sizeof(Segment*) * size);
    if (!mem->free_list) {
        fprintf(stderr, "Erreur d'allocation memoire d'une free-list\n");
        return NULL;
    }

    mem->free_list->size = size;
    mem->free_list->start = 0;
    mem->free_list->next = NULL;


    mem->allocated = hashmap_create();

    return mem;
}

Segment* find_free_segment(MemoryHandler* handler, int start, int size, Segment** prev) {
    if (!handler || !handler->free_list) {
        fprintf(stderr, "Erreur dans les parametres, veuillez verifier que handler n'est pas NULL\n");
        return NULL;
    }

    Segment* tmp = handler->free_list;
    while (!tmp) {

        if (tmp->start <= start && tmp->start + tmp->size >= start + size) {
            return tmp;
        }

        prev = &tmp;
        tmp = tmp->next;
    }

    return NULL;
}

int create_segment(MemoryHandler* handler, const char* name, int start, int size) {
    if (!handler || !handler->free_list || !handler->free_list->size) {
        fprintf(stderr, "Parametres invalides\n");
        return EXIT_FAILURE;
    }

    Segment* prev = NULL;

    Segment* new_seg = find_free_segment(handler, start, size, &prev);
    if (!new_seg) {
        fprintf(stderr, "Il n'y a pas de memoire libre suffisante\n");
        return EXIT_FAILURE;
    }

   

    




}

