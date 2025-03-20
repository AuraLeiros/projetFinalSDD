#include "parser.h"



Instruction* parser_data_instruction(const char* line, HashMap* memory_locations){

    char var[ASS_MAX_VAR_NAME];
    char label[ASS_MAX_LABEL_LEN];
    char ligne[ASS_MAX_NB];
    int ligneNum;

    int count;
    
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


    /* Calcul du dernier element */
    const char *elem = strrchr(ligne, ',');
    if (elem) {
        ligneNum = atoi(elem + 1);
    } else {
        ligneNum = atoi(ligne);        
    }

    while (*ligne) {
        if (*ligne == ','){
           count++; 
        }
        
        *ligne++;
    }


    /* Insertion dans la table d'hachage */

    



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