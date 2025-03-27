#include "cpu.h"

CPU* cpu_init(int memory_size){
    if (memory_size <= 0){
        fprintf(stderr, "Erreur dans le memory_size\n");
        return NULL;
    }

    /* Allocation d'un nouveau CPU*/
    CPU* newCPU = (CPU*)malloc(sizeof(CPU));
    if (!newCPU) {
        fprintf(stderr, "Erreur dans l'allocation d'un nouveau CPU\n");
        goto erreur;
    }

    /* Allocation d'un nouveau memory_handler*/
    MemoryHandler* newMemoryHandler = memory_init(memory_size);
    if (!newMemoryHandler) {
        fprintf(stderr, "Erreur dans l'allocation d'un nouveau Memory Handler\n");
        goto erreur;
    }

    /* Allocation d'un nouveau hash_map pour le contexte*/
    HashMap* newHashMap = hashmap_create();
    if (!newHashMap) {
        fprintf(stderr, "Erreur dans l'allocation d'un nouveau hashmap\n");
        goto erreur;
    }

    /* Ajout des registres generaux dans le hashmap*/
    hashmap_insert(newHashMap, "AX", (void*)0);
    hashmap_insert(newHashMap, "BX", (void*)0);
    hashmap_insert(newHashMap, "CX", (void*)0);
    hashmap_insert(newHashMap, "DX", (void*)0);

    /* Relier les structures au CPU */
    newCPU->memory_handler = newMemoryHandler;
    newCPU->context = newHashMap;

    return newCPU;


    erreur:
        if (newCPU) cpu_destroy(newCPU);
        if (newMemoryHandler) remove_memory_handler(newMemoryHandler);
        if (newHashMap) hashmap_destroy(newHashMap);
        return NULL;
        

}

void cpu_destroy(CPU* cpu){
    if (!cpu) return;

    /* Liberer la memoire des structures reliees */
    if (cpu->memory_handler) remove_memory_handler(cpu->memory_handler);
    if (cpu->context) hashmap_destroy(cpu->context);

    /* Liberer le cpu */
    free(cpu);

    return;
}

void* store(MemoryHandler* handler, const char* segment_name, int pos, void* data){
    if (!handler || !segment_name || (pos < 0)){
        fprintf(stderr, "Erreur dans les variables\n");
        return NULL;
    }

    /* Trouver le segment  */
    Segment* s = (Segment*)hashmap_get(handler->allocated, segment_name);
    if (!s) {
        fprintf(stderr, "Le segment n'a pas ete trouve\n");
        return NULL;
    }

    /* Verification de que le segment peut contenir data */
    if ((s->start + s->size) < (s->start + pos)) {
        fprintf(stderr, "Il a pas de place\n");
        return NULL;
    }

    /* Ajout de data dans la memoire */
    handler->memory[(s->start+pos)] = data;

    return data;
}

void* load(MemoryHandler* handler, const char* segment_name, int pos){
    if (!handler || !segment_name || (pos < 0)){
        fprintf(stderr, "Erreur dans les variables\n");
        return NULL;
    }

    Segment* s = (Segment*)hashmap_get(handler->allocated, segment_name);
    if (!s) {
        fprintf(stderr, "Le segment n'a pas ete trouve\n");
        return NULL;
    }

    return handler->memory[(s->start+pos)];
    
} 

void allocate_variables(CPU* cpu, Instruction** data_instructions, int data_count){
    if (!cpu || !data_instructions){
        fprintf(stderr, "Erreur dans les parametres\n");
        return;
    }

    parser_data_instruction()
//ds will have size data_count and be inserted as first allocated segment in cpu->memoryHandler->memory
//the free-list of this memory handler then contains the free segment starting at index data_count....

}

void print_data_segment(CPU* cpu);









