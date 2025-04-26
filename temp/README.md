Anna Stanichewsky
Aura Leirós García

/*
    /* Cas 1 : Le segment a allouer peut etre introduit parfaitement */

    /*TODO: NOTE:
    Cas 1: We have prev, then the next element in the free_list is after prev and after the segment
    Cas 2: We don't have prev, then the segment is the first one and the first element in the freelist is the next after our segment
    */
    if ((seg->start == start) && (seg->size == size)) {
        if (prev) {
            prev->next = seg->next;
        } else {
            handler->free_list = seg->next;
        }

        return EXIT_SUCCESS;
    }

    /* Cas 2: Le segment commence a la meme adresse mais pas exactement meme size */
    if ((seg->start) == start){
        if (prev) {
            prev->next = seg->next;
        } else {
            handler->free_list = seg->next;
        }

        /* Le segment d'après commence a la fin de la memoire allouee */
        if (seg->next) seg->next->start -= ((seg->next->start) - (size));

        return EXIT_SUCCESS;
    }
 */   



 # cpu.h

 void allocate_variables

 We need to allocate variables from .DATA, we know that the segment is called
 "DS"
 


 #TODO:

 - Exercice 5.7
 - Check the implementation of the Q.5.4 (in cpu.c)
 - check backslash regex