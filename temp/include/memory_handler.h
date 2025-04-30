#ifndef MEMORY_HANDLER_H
#define MEMORY_HANDLER_H

#include "hachage.h"


typedef struct segment {
    int start;
    int size;
    struct segment *next;
} Segment;

typedef struct memoryHandler {
    void **memory;
    int total_size;
    Segment *free_list;
    HashMap *allocated;
} MemoryHandler;

/* Allocation et initialisation d'un nouveau MemoryHandler */
MemoryHandler* memory_init(int size);

/* Retourne le premier segment libre qui satisfait les conditions, et sauvegarde le segment precedent dans prev */
Segment* find_free_segment(MemoryHandler* handler, int start, int size, Segment** prev);

/* Cree un nouveau segment et l'ajoute au HashMap allocated puis l'efface de la free_list */
int create_segment(MemoryHandler* handler, const char* name, int start, int size);

/* ------------------------ */
/* - Fonctions auxiliaires -*/
/* ------------------------ */

/* Liberation de la memoire allouee a un MemoryHandler */
void remove_memory_handler(MemoryHandler* mh);

/* Liberation de la memoire d'une liste de segments */
void remove_free_list(Segment* s);

/* Allocation et initialisation d'un nouveau Segment */
Segment* nouveau_segment(int start, int size);

#endif
