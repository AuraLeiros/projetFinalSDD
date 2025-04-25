#ifndef PARSER_H
#define PARSER_H

#include "hachage.h"
#include "macros.h"

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


/* Parse une ligne de .DATA, retourne une Instruction* */
Instruction* parser_data_instruction(const char* line, HashMap* memory_locations);

/* Parse une ligne de .CODE, retourne une Instruction* */
Instruction* parse_code_instructions(const char *line, HashMap* labels, int code_count);

/* Allocation et initialisation d'une structure ParserResult */
ParserResult* parse(const char* filename);

/* Liberation de la memoire allouee a un parseur */
void free_parser_result(ParserResult* p);

/* Trims a line */
char* trim(char* str);

/* Pre-traitement d'une ligne de .CODE */
int search_and_replace(char* str, HashMap* values);

/*------------------------*/
/* Fonctions auxiliaires */
/*------------------------*/

/* Alloue et initialise une nouvelle instruction */
Instruction* nouvelleInstruction(char* mnemonic, char* operand1, char* operand2);

/* Liberation de la memoire alloue a une instruction */
void libererInstruction(Instruction* i);

#endif