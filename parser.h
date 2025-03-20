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

Instruction* parser_code_instruction(const char* line, HashMap* labels, int code_count);


/* Fonctions auxiliaires */

/* Fonction auxiliaire pour allouer une nouvelle instruction */
Instruction* nouvelleInstruction(char* mnemonic, char* operand1, char* operand);

/* Fonction auxiliaire pour liberer la mémoire d'une instruction */
void libererInstruction(Instruction* i);


#endif