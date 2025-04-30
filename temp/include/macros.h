#ifndef MACROS_H
#define MACROS_H

/* Macros pour les fonctions de hachage */
#define TABLE_SIZE 128
#define TOMBSTONE ((void*) - 1)
#define ULONG_MAX 0xFFFFFFFFUL

/* Nombre premier utilise pour l'algorithme FNV-1a (64 bits) */
#define FNV_OFFSET_BASIS_64  0xCBF29CE484222325UL
#define FNV_PRIME_64         0x100000001B3UL

/* Macros pour le parser */
#define ASSEMBLER_MAX_BUFFER 512
#define CONSTANTS_MAX_BUFFER 128 

/* Macros pour les segments de cpu */
#define REGISTERS {"AX", "BX", "CX", "DX", "IP", "ZF", "SF", "SP", "BP"}
#define NUM_REGISTERS 9
#define SS_SIZE 128

#endif