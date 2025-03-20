#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


#include "macros.h"

/* Fonction pour ouvrir et tester l'existence et droits d'access au fichier */
FILE* loadFile(const char* filename, const char* mode);

/*---------------------*/
/* ----- Lecture -----*/
/*---------------------*/

/* Fonction pour lire des chaines de caracteres */
void readStr(char* buffer, int max_buffer);












#endif