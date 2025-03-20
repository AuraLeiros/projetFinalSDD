#ifndef MACROS_H
#define MACROS_H

/* Macros pour les fonctions de hachage */

#define TABLE_SIZE 128
#define TOMBSTONE ((void*) - 1)

/* Nombre premier utilise pour l'algorithme FNV-1a (64 bits) */
#define FNV_OFFSET_BASIS_64  0xCBF29CE484222325UL
#define FNV_PRIME_64         0x100000001B3UL

/* Macros pour le parser */
#define ASS_MAX_VAR_NAME 256 //Similaire aux assamblateurs actuels; 255 + 1 (le \0)
#define ASS_MAX_LABEL_LEN 256
#define ASS_MAX_NB 64

#endif