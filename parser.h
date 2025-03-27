#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#include "macros.h"
#include "hachage.h"
#include "utils.h"


typedef struct _instruction {
    char* mnemonic;
    char* operand1;
    char* operand2;
} Instruction;


typedef struct _ParserResult {
    Instruction** data_instructions;
    int data_count;
    Instruction** code_instructions;
    int code_count;
    HashMap* labels;
    HashMap* memory_locations;
} ParserResult;


Instruction* parser_data_instruction(const char* line, HashMap* memory_locations);

/* Fonction pour parser un fichier  */
Instruction* parser_code_instruction(const char* line, HashMap* labels, int code_count);

/* Parser un fichier contenant des instructions en assambleur */
ParserResult* parse(const char* filename);

/* Liberation de toute la memoire alloue a un parser et ses elements */
void free_parser_result(ParserResult* p){

/*------------------------*/
/* Fonctions auxiliaires */
/*------------------------*/

/* Fonction auxiliaire pour allouer une nouvelle instruction */
Instruction* nouvelleInstruction(char* mnemonic, char* operand1, char* operand);

/* Fonction auxiliaire pour liberer la mémoire d'une instruction */
void libererInstruction(Instruction* i);

/* Fonction auxiliaire pour creer et initialiser un nouveau parser */
ParserResult* nouveauParser();


#endif