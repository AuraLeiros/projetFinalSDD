#include "../include/parser.h"

Instruction* parser_data_instruction(const char* line, HashMap* memory_locations){
    if ((!line || line[0] == '/0') || !memory_locations){
        fprintf(stderr, "Erreur dans les parametres\n");
        goto erreur;
    }

    char nomVar[ASSEMBLER_MAX_BUFFER];
    char type[ASSEMBLER_MAX_BUFFER];
    char val[ASSEMBLER_MAX_BUFFER];

    /* Lecture de la ligne et decoupage en mots*/
    if (sscanf(line, "%s %s %[^\n]", nomVar, type, val) < 3) {
        fprintf(stderr, "Erreur dans la lecture de la ligne contenant le .DATA\n");
        goto erreur;
    }

    /* Allocation et initialisation d'une nouvelle instruction */
    Instruction* i = nouvelleInstruction(nomVar, type, val);
    if (!i) {
        fprintf(stderr, "Erreur dans l'allocation d'une nouvelle instruction\n");
        goto erreur;
    }

    /* Calcul du nombre d'elements dans la variable */
    char* ptr = val;
    int count = 1;

    while (*ptr) {
        if ((*ptr) == ',') count++;
        ptr++;
    }

    /* Insertion dans le HashMap et m-a-j du nombre d'elements */
    if (hashmap_insert(memory_locations, nomVar, (memory_locations->size)) == EXIT_FAILURE){
        fprintf(stderr, "Erreur dans l'insertion dans le HansMap\n");
        goto erreur;
    }

    memory_locations->size += count;

    return i;

    erreur:
        if (i) libererInstruction(i);
        return NULL;
}

Instruction* parse_code_instructions(const char *line, HashMap* labels, int code_count){
    if ((!line || line[0] == '\0') || !labels) {
        fprintf(stderr, "Erreur dans les parametres\n");
        return NULL;
    }

    char t1[ASSEMBLER_MAX_BUFFER];
    char t2[ASSEMBLER_MAX_BUFFER];
    char t3[ASSEMBLER_MAX_BUFFER];

    char op1[ASSEMBLER_MAX_BUFFER];
    char op2[ASSEMBLER_MAX_BUFFER];

    Instruction* i;

    /* Lecture de la ligne avec les deux possibilites */
    int res = sscanf(line, "%s %s %s", t1, t2, t3);
    if (res <= 1) {
        fprintf(stderr, "Erreur dans la lecture d'une ligne de .CODE\n");
        goto erreur;
    }

    /* Cas 1 : Il y a une etiquette*/
    if (res == 3) {
        if (sscanf(t3, "%[^,] %s", op1, op2) < 1) {
            fprintf(stderr, "Erreur dans l'extraction des opérandes\n");
            goto erreur;
        }

        i = nouvelleInstruction(t2, op1, op2);

        if (hashmap_insert(labels, t1, code_count) == EXIT_FAILURE) {
            fprintf(stderr, "Erreur dans l'insertion dans le HashMap\n");
            goto erreur;
        }
    /* Cas 2 : Il n'y a pas d'etiquette */
    } else {
        if (sscanf(t2, "%[^,] %s", op1, op2) < 1) {
            fprintf(stderr, "Erreur dans l'extraction des opérandes\n");
            goto erreur;
        }

        i = nouvelleInstruction(t1, op1, op2);
    }

    if (!i) {
        fprintf(stderr, "Erreur dans la creation d'une nouvelle Instruction\n");
        goto erreur;
    }

    return i;

    erreur:
        if (i) libererInstruction(i);
        return NULL;

}

ParserResult* parse(const char* filename){
    if (!filename || filename[0] == '\0'){
        fprintf(stderr, "Erreur dans les parametres\n");
        goto erreur;
    }

    /* Ouverture du fichier */
    FILE* f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Erreur dans l'ouverture d'un fichier\n");
        goto erreur;
    }

    /* Allocation memoire d'un nouveau parser */
    ParserResult* parser = nouveauParser();
    if (!parser) {
        fprintf(stderr, "Erreur dans l'allocation d'un nouveau parser\n");
        goto erreur;
    }

    char line[ASSEMBLER_MAX_BUFFER] = "";
    int section = -1;
    Instruction* i = NULL;
    Instruction** tmp = NULL;

    while (fgets(line, ASSEMBLER_MAX_BUFFER, f)){

        /* Échanger le saut de ligne pour un null-terminateur*/
        line[strcspn(line, "\n")] = '\0';

        /* Selection de la section .DATA / .CODE*/
        if (strcmp(line, ".DATA") == 0){
            section = 0;
            continue;
        }

        if (strcmp(line, ".CODE") == 0){
            section = 1;
            continue;
        }

        /* Traitement du parsing */
        switch(section){

            /* Parsing le .DATA*/
            case 0:
                i = parser_data_instruction(line, parser->memory_locations);
                if (!i){
                    fprintf(stderr, "Erreur dans le parsing\n");
                    goto erreur;
                }

                tmp = (Instruction**)realloc(parser->data_instructions, (++parser->data_count) * sizeof(Instruction*));
                if (!tmp){
                    fprintf(stderr, "Erreur dans la reallocation\n");
                    goto erreur;
                }

                parser->data_instructions = tmp;
                parser->data_instructions[(parser->data_count) - 1] = i;

                i = NULL;
                tmp = NULL;

                break;

            /* Parsing le .CODE*/
            case 1:
                i = parser_code_instruction(line, parser->labels, parser->code_count);
                if (!i){
                    fprintf(stderr, "Erreur dans le parsing\n");
                    goto erreur;
                }

                tmp = (Instruction**)realloc(parser->code_instructions, (++parser->code_count) * sizeof(Instruction*));
                if (!tmp){
                    fprintf(stderr, "Erreur dans la reallocation\n");
                    goto erreur;
                }

                parser->code_instructions = tmp;
                parser->code_instructions[(parser->code_count) - 1] = i;
            
                i = NULL; 
                tmp = NULL;

                break;

            default:
                fprintf(stderr, "L'input n'est pas valide\n");
                goto erreur;
        }

    }

    return parser;

    erreur:
        if (f) fclose(f);
        if (i) libererInstruction(i);
        if (parser) free_parser_result(parser);
        return NULL;
}



void free_parser_result(ParserResult* p){
    if (!p) return;


    int max = (p->data_count) >= (p->code_count) ? p->data_count : p->code_count;

    /* Liberer tous les elements dans chaque tableau */
    for(int i = 0; i < max; i++){
        if (p->data_instructions[i] && i < p->data_count) libererInstruction(p->data_instructions[i]);
        if (p->code_instructions[i] && i < p->code_count) libererInstruction(p->code_instructions[i]);
    }

    /* Liberation du pointer vers le tableau*/
    free(p->data_instructions);
    free(p->code_instructions);

    /* Liberaton des tables de hachage */
    hashmap_destroy(p->labels);
    hashmap_destroy(p->memory_locations);

    /* Liberation du parser*/
    free(p);

    return;
}

/*------------------------*/
/* Fonctions auxiliaires */
/*------------------------*/


Instruction* nouvelleInstruction(char* mnemonic, char* operand1, char* operand2){

    /* Allocation d'une nouvelle instruction */
    Instruction* i = (Instruction*)malloc(sizeof(Instruction));
    if (!i){
        fprintf(stderr, "Erreur dans l'allocation mémoire d'une nouvelle instruction\n");
        goto erreur;
    }

    i->mnemonic = strdup(mnemonic);
    if (!i->mnemonic){
        fprintf(stderr, "Erreur dans la copie du mnemonic\n");
        goto erreur;
    }

    i->operand1 = strdup(operand1);
    if (!i->operand1){
        fprintf(stderr, "Erreur dans la copie de l'operand1\n");
        goto erreur;
    }

    i->operand2 = strdup(operand2);
    if (!i->operand2){
        fprintf(stderr, "Erreur dans la copie de l'operand2\n");
        goto erreur;
    }

    return i;

    erreur:
        if (i) libererInstruction(i);
        return NULL;
}

/* Fonction auxiliaire pour liberer la mémoire d'une instruction */
void libererInstruction(Instruction* i){
    if (!i) return;

    if (i->mnemonic) free(i->mnemonic);
    if (i->operand1) free(i->operand1);
    if (i->operand2) free(i->operand2);

    free(i);

    return;
}

char* trim(char* str) {
    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r')
        str++;

    char *end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        *end = '\0';
        end--;
    }

    return str;
}

int search_and_replace(char** str, HashMap* values) {
    if (!str || !*str || !values) return 0;

    int replaced = 0;
    char *input = str;

    // Iterate through all keys in the hashmap
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (values->table[i].key && values->table[i].key != TOMBSTONE) {
            char *key = (char *)values->table[i].key;
            int* value = (int*)values->table[i].value;

            // Find potential substring match
            char *substr = strstr(input, key);
            if (substr) {
                // Construct replacement buffer
                char replacement[64];
                snprintf(replacement, sizeof(replacement), "%d", value);

                // Calculate lengths
                int key_len = strlen(key);
                int repl_len = strlen(replacement);
                int input_len = strlen(substr + key_len);

                // Create new string
                char *new_str = (char *)malloc(strlen(input) - key_len + repl_len + 1);
                strncpy(new_str, input, substr - input);
                new_str[substr - input] = '\0';
                strcat(new_str, replacement);
                strcat(new_str, substr + key_len);

                // Free and update original string
                free(input);
                *str = new_str;
                input = new_str;
                replaced = 1;
            }
        }
    }

    // Trim the final string
    if (replaced) {
        char *trimmed = trim(input);
        if (trimmed != input) {
            memmove(input, trimmed, strlen(trimmed) + 1);
        }
    }

    return replaced;
}


/*------------------------*/
/* Fonctions auxiliaires  */
/*------------------------*/


/* Fonction auxiliaire pour allouer et initialiser une nouvelle instruction */
Instruction* nouvelleInstruction(char* mnemonic, char* operand1, char* operand2){

    /* Allocation d'une nouvelle instruction */
    Instruction* i = (Instruction*)malloc(sizeof(Instruction));
    if (!i){
        fprintf(stderr, "Erreur dans l'allocation mémoire d'une nouvelle instruction");
        goto erreur;
    }

    i->mnemonic = strdup(mnemonic);
    if (!i->mnemonic){
        fprintf(stderr, "Erreur dans la copie du mnemonic\n");
        goto erreur;
    }

    i->operand1 = strdup(operand1);
    if (!i->operand1){
        fprintf(stderr, "Erreur dans la copie de l'operand1");
        goto erreur;
    }

    i->operand2 = strdup(operand2);
    if (!i->operand2){
        fprintf(stderr, "Erreur dans la copie de l'operand2");
        goto erreur;
    }

    return i;

    erreur:
        libererInstruction(i);
        return NULL;
}

/* Fonction auxiliaire pour creer et initialiser un nouveau parser */
ParserResult* nouveauParser(){
        
    ParserResult* parser = (ParserResult*)malloc(sizeof(ParserResult));
    if (!parser){
        fprintf(stderr, "Erreur d'allocation d'un nouveau parser\n");
        return NULL;
    }
    
    /* Variables liees au .DATA */
    parser->data_instructions = NULL;
    parser->data_count = 0;

    /* Variables liees au .CODE */
    parser->code_instructions = NULL;
    parser->code_count = 0;

    /* Allocation du HashMap pour les labels */
    parser->labels = hashmap_create();
    if (!parser->labels) {
        fprintf(stderr, "Erreur dans la creation de labels\n");
        goto erreur;
    }

    /* Allocation du HashMap pour les directions en memoire */
    parser->memory_locations = hashmap_create();
    if (!parser->memory_locations) {
        fprintf(stderr, "Erreur dans la creation de memory_locations\n");
        goto erreur;
    }

    return parser;

    erreur:
        free_parser_result(parser);
        return NULL;

}

