#ifndef MEMORY_HANDLER_H
#define MEMORY_HANDLER_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "macros.h"
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

//Initialisation du gestionnaire de memoire
MemoryHandler *memory_init(int size);

//Retourne le segment s'il existe dans la free_list avec seg->start <= start et seg->end >= start+size
Segment* find_free_segment(MemoryHandler* handler, int start, int size, Segment** prev);

//Allocation d'un segment dans la HashMap en retirant cet espace de la free_list
int create_segment(MemoryHandler* handler, const char* name, int start, int size);

//Retirer un segment de la memoire
int remove_segment(MemoryHandler* handler, const char* name);


void remove_memory_handler(MemoryHandler* m);

void remove_free_list(Segment* seg) 

#endif //MEMORY_HANDLER_H
