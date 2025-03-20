#include "utils.h"

/*---------------------*/
/* ----- Lecture -----*/
/*---------------------*/

/* Fonction pour ouvrir et tester l'existence et droits d'access au fichier */
FILE* loadFile(const char* filename, const char* mode){
    if (!filename || !mode){
        fprintf(stderr, "Erreur dans les parametres\n");
        return NULL;
    }

    if (access(filename, F_OK) == -1) {
        fprintf(stderr, "Erreur : Le fichier n'existe pas ou il n'est pas accessible\n");
        return NULL;
    }

    FILE* f = fopen(filename, mode);
    if (!f){
        fprintf(stderr, "Erreur dans l'ouverture du fichier\n");
        return NULL;
    }

    return f;

}




/* Fonction auxiliaire pour lire des chaines des characteres*/
void readStr(char* buffer, int max_buffer){
    if (max_buffer < 1){
        fprintf(stderr, "La taille du buffer n'est pas suffisante");
        return;
    }

    do {

        if (!fgets(buffer, max_buffer, stdin)){
            fprintf(stderr, "Erreur dans la lecture depuis le stdin\nVeuillez reesayer\n");
            continue;            
        }
        
        int len = strlen(buffer);

        /* Verifier s'il y a eu truncation (l'input dans le stdin etait plus long que max_buffer)*/
        if ((len == (max_buffer - 1)) && (buffer[len - 1] != '\n')){
            int ch;
            while ((ch = getchar()) != '\n');
            fprintf(stderr, "Erreur ! Overflow dans la lecture d'une chaine de caracteres\n");
            fprintf(stderr, "Veuillez reesayer\n");
            continue;
        }

        /* Effacer le \n */
        buffer[strcspn(buffer, "\n")] = '\0';
        break;

    } while (1);

}