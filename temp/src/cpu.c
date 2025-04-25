#include "../include/cpu.h"

CPU* cpu_init(int memory_size){
    if (memory_size <= 0){
        fprintf(stderr, "Erreur dans le memory_size\n");
        return NULL;
    }

    CPU* cpu = NULL;
    MemoryHandler* mh = NULL;
    HashMap* contextHM = NULL;
    HashMap* poolHM = NULL;
    HashMap* constant_pool = NULL;
    const char* reg[] = {"AX", "BX", "CX", "DX", "IP", "ZF", "SF"};;
    int numReg = 7;
    int* val = NULL;

    /* Allocation d'un nouveau CPU*/
    cpu = (CPU*)malloc(sizeof(CPU));
    if (!cpu) {
        fprintf(stderr, "Erreur dans l'allocation d'un nouveau CPU\n");
        goto erreur;
    }

    /* Allocation d'un nouveau memory_handler*/
    mh = memory_init(memory_size);
    if (!mh) {
        fprintf(stderr, "Erreur dans l'allocation d'un nouveau Memory Handler\n");
        goto erreur;
    }

    /* Allocation d'un nouveau hash_map pour le contexte*/
    contextHM = hashmap_create();
    if (!contextHM) {
        fprintf(stderr, "Erreur dans l'allocation d'un nouveau hashmap\n");
        goto erreur;
    }

    /* Allocation d'un nouveau HashMap pour les constantes */
    constant_pool = hashmap_create();
    if (!constant_pool) {
        fprintf(stderr, "Erreur dans l'allocation d'un nouveau hashmap\n");
        goto erreur;
    }

    /* Ajout des registres dans les HashMap */

    for (int x=0; x < numReg; x++) {
        val = (int*)malloc(sizeof(int));
        if (!val) {
            fprintf(stderr, "Erreur dans l'allocation memoire d'un val");
            goto erreur;
        }

        *val = 0;
        
        if (!hashmap_insert(contextHM, reg[x], val)){
            fprintf(stderr, "Erreur dasn l'insertion dans le HashMap\n");
            goto erreur;
        }
    }

    /* Relier les structures au CPU */
    cpu->memory_handler = mh;
    cpu->context = contextHM;
    cpu->constant_pool = poolHM;

    return cpu;

    erreur:
        if (contextHM) hashmap_destroy(contextHM);
        if (constant_pool) hashmap_destroy(constant_pool); 
        if (mh) remove_memory_handler(mh);
        if (cpu) free(cpu);  // not cpu_destroy, since subfields are already cleaned up
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
    if (!handler || (!segment_name || segment_name[0] == '\0') || pos < 0){
        fprintf(stderr, "Erreur dans les parametres\n");
        return NULL;
    }

    /* Verification de que segment_name est allouée*/
    Segment* s = (Segment*)hashmap_get(handler->allocated, segment_name);
    if (!s) {
        fprintf(stderr, "Le segment n'a pas ete trouve dans le tableau de memoire allouee\n");
        return NULL;
    }

    /* Verification de que la position est dans le rang */
    if ((s->start + s->size) <= (s->start + pos)) {
        fprintf(stderr, "Il a pas de place\n");
        return NULL;
    }

    /* Ajout des donnees dans la memoire */
    handler->memory[(s->start+pos)] = data;

    return data;
}

void* load(MemoryHandler* handler, const char* segment_name, int pos){
    if (!handler || (!segment_name || segment_name[0] == '\0') || pos < 0){
        fprintf(stderr, "Erreur dans les parametres\n");
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
    if (!cpu || !data_instructions || (data_count < 0)){
        fprintf(stderr, "Erreur dans les sparametres\n");
        return;
    }

    char tmpString[ASSEMBLER_MAX_BUFFER];
    int** tabArrays;
    char* token;
    int idx = 0;

    /* Compter le nombre d'elements*/
    for (int x=0; x < data_count; x++){

        /* Tokenize the current .DATA line */

        /* Copier la ligne avec des arrays*/
        strncpy(tmpString, data_instructions[x]->operand2, ASSEMBLER_MAX_BUFFER - 1);

        /* Traitement des elements dans l'instruction */
        token = strtok(tmpString, ",");

        while (token){

            idx++;

            /* Passer au prochain element dans l'array (s'il existe) */
            token = strtok(NULL, ",");
        }
    }
    
    /* Creer un segment et l'ajouter au HashMap */
    int s = create_segment(cpu->memory_handler, "DS", 0, idx);
    if (!s) {
        fprintf(stderr, "Erreur dans la creation du nouveau segment\n");
        return;
    }

    /* Ajouter tous les elements dans la memoire */

    idx = 0;

    for (int x=0; x < data_count; x++){

        /* Tokenize the current .DATA line */

        /* Copier la ligne avec des arrays*/
        strncpy(tmpString, data_instructions[x]->operand2, ASSEMBLER_MAX_BUFFER - 1);

        /* Traitement des elements dans l'instruction */
        token = strtok(tmpString, ",");

        while (token){

            /* Creation d'un pointeur vers l'element actuel */
            int* num = (int*)malloc(sizeof(int));
            if (!num) {
                fprintf(stderr, "Erreur dans l'allocation memoire\n");
                return;
            }

            *num = atoi(token);

            /* Ajout dans la memoire */
            store(cpu->memory_handler, "DS", (idx++), num);
            
            /* Passer au prochain element dans l'array (s'il existe) */
            token = strtok(NULL, ",");
        }
    }
}


void print_data_segment(CPU* cpu) {
    if (!cpu) {
        fprintf(stderr, "Erreur dans les parametres\n");
        return;
    }

    Segment* s = (Segment*)hashmap_get(cpu->memory_handler->allocated, "DS");
    if (!s){
        fprintf(stderr, "Le segment DS n'est pas present\n");
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


void* inmediate_addressing(CPU* cpu, const char* operand){
    if (!cpu || (!operand || operand[0] == '\0')){
        fprintf(stderr, "Erreur dans les parametres\n");
        return NULL;
    }

    int* val = NULL;
    int* num;

    if (matches("^[0-9]+$", operand)) {
        val = (int*)hashmap_get(cpu->constant_pool, operand);

        if (!val){
            num = (int*)malloc(sizeof(int));
            *num = atoi(operand);

            if (hashmap_insert(cpu->constant_pool, operand, num)){
                return num;
            }
        }
    }

    return NULL;

}


void* register_addressing(CPU* cpu, const char* operand) {
    if (!cpu || (!operand || operand[0] == '\0')){
        fprintf(stderr, "Erreur dans les parametres\n");
        return NULL;
    }

    int* val = NULL;
    
    /* Verifier le REGEX*/
    if (matches("^[A-D]X$", operand)){

        /* Obtention de l'adresse */
        val = (int*)hashmap_get(cpu->context, operand);
    }

    return val;

}


void* memory_direct_addressing(CPU* cpu, const char* operand) {
    if (!cpu || (!operand || operand[0] == '\0')){
        fprintf(stderr, "Erreur dans les parametres\n");
        return NULL;
    }

    int valReg;
    
    if (matches("^\[[0-9]+\]", operand)){

        if (!(sscanf(operand, "[%d]", &valReg) == 1)) {
            fprintf(stderr, "Erreur dans la lecture de la valeur\n");
            return NULL;
        }

        return load(cpu->memory_handler, "DS", valReg);
    }

    return NULL;
}




void* register_indirect_addressing(CPU* cpu, const char* operand) {
    if (!cpu || (!operand || operand[0] == '\0')){
        fprintf(stderr, "Erreur dans les parametres\n");
        return NULL;
    }

    char tmp[ASSEMBLER_MAX_BUFFER];
    int* valReg = NULL;

    if (matches("^\[[A-D]X\]$", operand)){

        if (!(sscanf(operand, "[%s]", tmp) == 1)) {
            fprintf(stderr, "Erreur dans la lecture du registre\n");
            return NULL;
        }

        valReg = (int*)hashmap_get(cpu->context, tmp);
        if (!valReg) {
            fprintf(stderr, "Erreur dans la lecture du registe\n");
            return NULL;
        }

        return load(cpu->memory_handler, "DS", (*valReg));
    }

    return NULL;

}


void handle_MOV(CPU* cpu, void* src, void* dest){
    if (!cpu) {
        fprintf(stderr, "Erreur dans les parametres\n");
        return;
    }

    memcpy(dest, src, sizeof(int));

}

CPU* setup_test_environment() {
    // Initialiser le CPU
    CPU *cpu = cpu_init(1024);
    if (!cpu) {
        printf("Error: CPU initialization failed\n");
        return NULL;
    }

    // Initialiser les registres avec des valeurs spécifiques
    int *ax = (int *)hashmap_get(cpu->context, "AX");
    int *bx = (int *)hashmap_get(cpu->context, "BX");
    int *cx = (int *)hashmap_get(cpu->context, "CX");
    int *dx = (int *)hashmap_get(cpu->context, "DX");

    *ax = 3;
    *bx = 6;
    *cx = 100;
    *dx = 0;

    // Créer et initialiser le segment de données
    if (!hashmap_get(cpu->memory_handler->allocated, "DS")) {
        create_segment(cpu->memory_handler, "DS", 0, 20);

        // Initialiser le segment de données avec des valeurs de test
        for (int i = 0; i < 10; i++) {
            int *value = (int *)malloc(sizeof(int));
            *value = i * 10 + 5; // Valeurs 5, 15, 25, 35...
            store(cpu->memory_handler, "DS", i, value);
        }
    }

    printf("Test environment initialized.\n");
    return cpu;
}

void* resolve_addressing(CPU* cpu, const char* operand) {
    if (!cpu || (!operand || operand[0] == '\0')){
        fprintf(stderr, "Erreur dans les parametres\n");
        return NULL;
    }

    void* a = inmediate_addressing(cpu, operand);
    if (a) return a;

    void* b = register_addressing(cpu, operand);
    if (b) return b;

    void* c = memory_direct_addressing(cpu, operand);
    if (c) return c;

    void* d = register_indirect_addressing(cpu, operand);
    if (d) return d;

    fprintf(stderr, "Le operand ne correspond pas a aucun des methodes d'addressage\n");
    return NULL;

}

int resolve_constants(ParserResult* result) {
    if (!result) {
        fprintf(stderr, "Erreur dans les parametres\n");
        return EXIT_FAILURE;
    }

    /* Iteration sur tous les lignes de .CODE */
    for (int i=0; i < result->code_count; i++) {
        if (result->code_instructions[i]->operand2) {
            if (!search_and_replace(((result->code_instructions[i]->operand2)), result->memory_locations)){
                fprintf(stderr, "Erreur dans le reemplacement\n");
                return EXIT_FAILURE;
            }

            /* "i" -> "[i]" */

            int len = strlen(result->code_instructions[i]->operand2);

            char tmp[len + 3];
            tmp[0] = '[';
            tmp[1] = '\0';
            
            strcat(tmp, result->code_instructions[i]->operand2);
            tmp[len + 1] = ']';
            tmp[len + 2] = '\0';

            /* Copier la nouvelle chaine dans le parser */
            free(result->code_instructions[i]->operand2);
            result->code_instructions[i]->operand2 = strdup(tmp);

            if (!result->code_instructions[i]->operand2) {
                fprintf(stderr, "Erreur dans la copie de la nouvelle chaine\n");
                return EXIT_FAILURE;
            }

        }

    }

    return EXIT_SUCCESS;
}


