#ifndef CPU_H
#define CPU_H


#include <stdio.h>
#include <string.h>
#include <regex.h>

#include "memory_handler.h"
#include "parser.h"
#include "hachage.h"

typedef struct _CPU {
    MemoryHandler* memory_handler;
    HashMap* context; /* Registres AX BX CX DX */
    HashMap* constant_pool;
} CPU;

/* Allocation et initialisation d'un nouveau CPU */
CPU* cpu_init(int memory_size);

/* Efface toute la memoire allouee */
void cpu_destroy(CPU* cpu);

/* Stocker une donnée dans un segment*/
void* store(MemoryHandler* handler, const char* segment_name, int pos, void* data);

/* Retrouver le data associe a une position d'un segment */
void* load(MemoryHandler* handler, const char* segment_name, int pos);

/* Ajoute dans la memoire toutes les variables .DATA */
void allocate_variables(CPU* cpu, Instruction** data_instructions, int data_count);


/*------------------------*/
/*    Fonctions REGEX     */
/*------------------------*/

/* Teste une expression reguliere */
int matches(const char *pattern, const char *string);

/* Traitement d'une operande avec immediate addressing, retourne un pointeur a la memoire */
void* immediate_addressing(CPU* cpu, const char* operand);

/* Traitement d'une operande avec register addressing, retourne le registre s'il existe dans le contexte */
void* register_addressing(CPU* cpu, const char* operand);

/* Traitement d'une operande avec le memory_direct_addressing, donc chercher directement a la position donnee en operande dans le DS */
void* memory_direct_addressing(CPU* cpu, const char* operand);

/* Traitement d'une operande avec un register_indirect_addressing */
void* register_indirect_addressing(CPU* cpu, const char* operand);

/* Teste toutes les methodes d'adressage et retourne une valeur */
void* resolve_addressing(CPU* cpu, const char* operand);

/* Remplace les constantes et les etiquettes par les addresses */
int resolve_constants(ParserResult* result);

/* Simulation de l'instruction assambleur MOV */
void handle_MOV(CPU* cpu, void* src, void* dest);

/* Fonction donnée pour mettre en place l'environnement */
CPU* setup_test_environment();

/* Traite une liste de instructions et les ajoute dans le segment CS*/
void allocate_code_segment(CPU* cpu, Instruction** code_instructions, int code_count);

/* Effectuer des operations */
int handle_instructions(CPU* cpu, Instruction* instr, void* src, void* dest);

/* Execute une instruction */
int execute_instructions(CPU* cpu, Instruction* instr);

/* Reenvoie un pointeur vers la prochaine instruction dans CS */
Instruction* fetch_next_instruction(CPU* cpu);

/* Execute les instructions du CS et affiche l'etat du CPU dans le STDIN */
int run_program(CPU* cpu);

/* Manipulation de la pile */
int push_value(CPU* cpu, int value);

int pop_value(CPU* cpu, int* dest);

/*-------------------------*/
/* Fonctions auxiliaires*/
/*-------------------------*/

int handle_ADD(CPU* cpu, void* src, void* dest);

int handle_CMP(CPU* cpu, void* src, void* dest);

int handle_JMP(CPU* cpu, void* src);

int handle_JZ(CPU* cpu, void* src);

int handle_JNZ(CPU* cpu, void* srcs);

int handle_HALT(CPU* cpu);

int handle_PUSH(CPU* cpu, void* src);

int handle_POP(CPU* cpu, void* dest);

/* Affiche l'etat du CPU au instant T*/
void affichageCPU(CPU* cpu);


/* todo */
int alloc_es_segment(CPU* cpu);

int free_es_segment(CPU* cpu);





#endif