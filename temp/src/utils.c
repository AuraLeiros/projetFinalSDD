#include "../include/utils.h"


/* Fonction auxiliaire pour lire des chaines des characteres*/
void readStr(char* buffer, int max_buffer){

    if (!fgets(buffer, max_buffer, stdin)){
        fprintf(stderr, "Erreur dans la lecture depuis le stdin\n");
    }

    buffer[strcspn(buffer, "\n")] = '\0';
}