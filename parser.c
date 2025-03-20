#include "parser.h"


Instruction* parser_data_instruction(const char* line, HashMap* memory_locations){
    if (!memory_locations || !line){
        fprintf(stderr, "Erreur dans les parametres\n");
        return NULL;
    }

    char var[ASS_MAX_VAR_NAME];
    char label[ASS_MAX_LABEL_LEN];
    char ligne[ASS_MAX_NB];
    
    /* Lecture de la ligne et découpage en tokens z*/
    if (sscanf(line, "%s %s %[^\n]", var, label, ligne) < 3) {
        fprintf(stderr, "Erreur dans la lecture de la ligne contenant le .DATA\n");
        return NULL;
    }

    /* Creation d'une nouvelle instruction */
    Instruction* i = nouvelleInstruction(var, label, ligne);
    if (!i){
        fprintf(stderr, "Erreur dans la creation d'une nouvelle instruction\n");
        return NULL;
    }

    /* Calcul du nombre d'elements dans la variable */
    char* ptr = ligne;
    int count = 1;

    while (ptr){
        if (*ptr == ',') count++;
        ptr++;
    }

    /* Insertion de la variable et MAJ du size de la table d'hachage*/
    hashmap_insert(memory_locations, var, (memory_locations->size));
    memory_locations->size += count;

    return i;
}

Instruction* parser_code_instruction(const char* line, HashMap* labels, int code_count){
    if (!line || !labels){
        fprintf(stderr, "Erreur dans les paramètres\n");
        return NULL;
    }

    char var[ASS_MAX_VAR_NAME];
    char label[ASS_MAX_LABEL_LEN];
    char ligne[ASS_MAX_NB];

    char operand1[ASS_MAX_VAR_NAME];
    char operand2[ASS_MAX_LABEL_LEN] = "";
    Instruction* i = NULL;

    sscanf(x, y, label, var, operands)

    /* Lecture d'une ligne avec les deux possibilites*/
    int res = sscanf(line, "%s %s %s", var1, var2, operands);
    if (res < 1){
        fprintf(stderr, "Erreur dans la lecture de la ligne passe en parametre\n");
        return NULL;
    }

    if (res == 3){
        if (sscanf(operands, "%[^,] %s", operand1, operand2) < 1) {
            fprintf(stderr, "Erreur dans l'extraction des opérandes\n");
            return NULL;
        }

        i = nouvelleInstruction(var2, operand1, operand2);
        hashmap_insert(labels, var1, code_count);

    } else if (res == 2) {
        if (sscanf(var2, "%[^,] %s", operand1, operand2) < 1) {
            fprintf(stderr, "Erreur dans l'extraction des opérandes\n");
            return NULL;
        }

        i = nouvelleInstruction(var1, operand1, operand2);
    }

    if (!i){
        fprintf(stderr, "Erreur dans la creation d'une nouvelle Instruction\n");
        return NULL;
    }

    return i;

}



ParserResult* parse(const char* filename){
    if (!filename){
        fprintf(stderr, "Erreur dans les parametres\n");
        return NULL;
    }

    /* Ouvrir le fichier*/
    FILE* f = loadFile(filename, "r");
    if (!f){
        fprintf(stderr, "Erreur dans l'ouverture du fichier\n");
        goto erreur;
    }

    ParserResult* parser = nouveauParser();
    if (!parser){
        fprintf(stderr, "Erreur dans l'allocation d'un nouveau parser\n");
        return NULL;
    }


    char line[ASS_MAX_LINE_LEN] = "";
    int isData = 0;
    int isCode = 0;

    while (fgets(line, ASS_MAX_LINE_LEN, f)) {

        if (line == ".DATA\n") {
            isData = 1;
            isCode = 0;
        } else if (line == ".CODE\n") {
            isData = 0;
            isCode = 1;
        }

        if (isData){
            

        }
        


        if ()


    }






    erreur:
        fclose(f);

        return NULL;



    if (!f) {
        fprintf(stderr, "Erreur dans la lecture d'un fichier\n");
        return NULL;
    }

    /* Nouvelle structure ParserResult */
    ParserResult * parser = malloc

    char line[ASS_MAX_LINE_LEN];

    while (fgets(line, ASS_MAX_LINE_LEN, f)) {


    }
}




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

/* Fonction auxiliaire pour liberer la mémoire d'une instruction */
void libererInstruction(Instruction* i){
    if (!i) return;

    if (i->mnemonic) free(i->mnemonic);
    if (i->operand1) free(i->operand1);
    if (i->operand2) free(i->operand2);

    free(i);

    return;
}

/* Fonction auxiliaire pour creer et initialiser un nouveau parser */
ParserResult* nouveauParser(){
        
    ParserResult* parser = (ParserResult*)malloc(sizeof(ParserResult));
    if (!parser){
        fprintf(stderr, "Erreur d'allocation d'un nouveau parser\n");
        return NULL;
    }

    parser->data_instructions = (Instruction**)malloc(sizeof(Instruction*));
    if (!parser->data_instructions) {
        fprintf(stderr, "Erreur dans l'allocation du tableau de instructions .DATA\n");
        goto erreur;
    }

    parser->data_count = 0;

    parser->code_instructions = (Instruction**)malloc(sizeof(Instruction*));
    if (!parser->code_instructions){
        fprintf(stderr, "Erreur dans l'allocation du tableau de instructions .CODE\n");
        goto erreur;
    }

    parser->code_count = 0;

    parser->labels = hashmap_create();
    if (!parser->labels) {
        fprintf(stderr, "Erreur dans la creation de labels\n");
        goto erreur;
    }

    parser->memory_locations = hashmap_create();
    if (!parser->memory_locations) {
        fprintf(stderr, "Erreur dans la creation de memory_locations\n");
        goto erreur;
    }

    return parser;

    erreur:
        libererParser(parser);
        return NULL;

}

/* Liberation de toute la memoire alloue a un parser et ses elements */
void libererParser(ParserResult* p){
    if (!p) return;


    int max = (p->data_count) >= (p->code_count) ? p->data_count : p->code_count;

    for(int i = 0; i < max; i++){
        if (i < data_count) libererInstruction(p->data_instructions[i]);
        if (i < code_count) libererInstruction(p->code_instructions[i]);
    }

    free(data_instructions);
    free(code_instructions);

    hashmap_destroy(labels);
    hashmap_destroy(memory_locations);

    free(p);

    return;
}