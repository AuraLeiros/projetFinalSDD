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

    /* Lecture d'une ligne avec les deux possibilites*/
    int res = sscanf(line, "%s %s %s", var, label, ligne);
     
    if (res == 3){

        if (sscanf(ligne, "%[^,] %s", operand1, operand2) < 1) {
            fprintf(stderr, "Erreur dans l'extraction des opérandes\n");
            return NULL;
        }

        i = nouvelleInstruction(label, operand1, operand2);

        hashmap_insert(labels, var, code_count);

    } else if (res == 2){

        if (sscanf(label, "%[^,] %s", operand1, operand2) < 1) {
            fprintf(stderr, "Erreur dans l'extraction des opérandes\n");
            return NULL;
        }

        i = nouvelleInstruction(var, operand1, operand2);

    } else {
        fprintf(stderr, "Erreur dans la lecture\n");
        return NULL;
    }
    
    if (!i){
        fprintf(stderr, "Erreur dans la creation d'une nouvelle Instruction\n");
        return NULL;
    }

    return i;

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