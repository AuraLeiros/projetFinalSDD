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

    HashMap* poolHM = hashmap_create();
    if (!poolHM){
        fprintf(stderr, "Erreur dans l'allocation d'un nouveau constant_pool\n");
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
    newCPU->constant_pool = poolHM;

    return newCPU;


    erreur:
        if (newCPU) cpu_destroy(newCPU);
        if (newMemoryHandler) remove_memory_handler(newMemoryHandler);
        if (newHashMap) hashmap_destroy(newHashMap);
        if (poolHM) hashmap_destroy(poolHM);
        return NULL;
}

void cpu_destroy(CPU* cpu){
    if (!cpu) return;

    /* Liberer la memoire des structures reliees */
    if (cpu->memory_handler) remove_memory_handler(cpu->memory_handler);
    if (cpu->context) hashmap_destroy(cpu->context);
    if (cpu->constant_pool) hashmap_destroy(cpu->constant_pool);

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
    if (!cpu || !data_instructions || data_count < 0){
        fprintf(stderr, "Erreur dans les parametres\n");
        return;
    }

    int totalMemory = 0;
    int* tempList = malloc(sizeof(int))
    
    for (int x=0; x < data_count; x++){
        if (strcmp(data_instructions[x]->mnemonic, "arr") == 0){
            int count = 1;
            char* ptr = data_instructions[x]->operand2;
            while (*ptr){
                if (strcmp(ptr, ',') == 0) count++;
                ptr++;
            }
            totalMemory += count;
        } else {
            totalMemory++;
        }
    }

    int s = create_segment(cpu->memory_handler, "DS", 0, totalMemory);
    if (!s) {
        fprintf(stderr, "Erreur dans la creation d' un nouveau segment\n");
        return;
    }

    int i = 0;
    for (int x=0; x < data_count; x++) {
        if (strcmp(data_instructions[x]->mnemonic, "arr") == 0){
            char* ptr = data_instructions[x]->operand2;
            while (*ptr){
                if (*ptr != ','){
                    cpu->memory_handler->memory[i] = (int)(*ptr);
                    i++;   
                }

                ptr++;
            }
        } else {
            cpu->memory_handler->memory[i] = (int)data_instructions[x]->operand2;
            i++;
        }
    }

    return;
}


void print_data_segment(CPU* cpu){
    if (!cpu){
        fprintf(stderr, "Erreur dans les parametres\n");
    }

    Segment* s = (Segment*)hashmap_get(cpu->memory_handler->allocated, "DS");
    if (!s){
        fprintf(stderr, "Erreur dans les parametres\n");
        return;
    }

    int start = s->start;
    int end = s->size - s->start;

    for (int x=start; x < end; x++) printf("%d\t", cpu->memory_handler->memory[x]);
    printf("\n");

    return;
}


int matches(const char *pattern, const char *string) {

    regex_t regex;

    int result = regcomp(&regex, pattern, REG_EXTENDED);
    if (result) {
        fprintf(stderr, "Regex compilation failed for pattern: %s\n", pattern);
        return 0;
    }

    result = regexec(&regex, string, 0, NULL, 0);
    regfree(&regex);
    return result == 0;
}








