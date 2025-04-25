#include "../include/memory_handler.h"


MemoryHandler* memory_init(int size) {
    if (size <= 0){
        fprintf(stderr, "La taille passe en parametre est invalide");
        return NULL;
    }

    /* Allocation d'une nouvelle structure MemoryHandler */
    MemoryHandler* mh = (MemoryHandler*)malloc(sizeof(MemoryHandler));
    if (!mh) {
        fprintf(stderr, "Erreur dans l'allocation memoire d'un nouveau MemoryHandler\n");
        goto erreur;
    }

    /* Note : size est la taille total de la memoire geree par le MemoryHandler */
    mh->total_size = size;

    /* Allocation et initialisation du segment de memoire initial */
    Segment* s = nouveau_segment(0, size);
    if (!s) {
        fprintf(stderr, "Erreur dans la creation d'un nouveau segment\n");
        goto erreur;
    }

    mh->free_list = s;
    
    /* Aucun element present dans le tableau de memoire alloue */
    mh->memory = NULL;

    /* Creation d'une nouvelle table de hachage pour la memoire allouee */
    HashMap* h = hashmap_create();
    if (!h) {
        fprintf(stderr, "Erreur dans la creation d'un nouveau HashMap\n");
        goto erreur;
    }

    mh->allocated = h;


    return mh;

    erreur:
        if (mh) remove_memory_handler(mh);
        if (s) free_liste_segment(s);
        if (h) hashmap_destroy(h);
        return NULL;

}

Segment* find_free_segment(MemoryHandler* handler, int start, int size, Segment** prev){
    if (!handler || (start < 0) || (size <= 0) || (!prev)) {
        fprintf(stderr, "Erreur dans les parametres\n");
        return NULL;
    }

    /* Iteration sur les elements de free_list pour trouver un space qui satisfait les conditions */
    Segment* tmp = handler->free_list;

    while (tmp) {

        if ((tmp->start <= start) && ((tmp->start + tmp->size) >= (start + size))){
            return tmp;
        }

        prev = &tmp;
        tmp = tmp->next;
    }

    return NULL;

}


int create_segment(MemoryHandler* handler, const char* name, int start, int size){
    if (!handler || (!name || name[0] == '\0') || (start < 0) || (size < 0)){
        fprintf(stderr, "Erreur dans les parametres\n");
        return EXIT_FAILURE;
    }


    /* Verifier qu'il existe un segment libre suffisante */
    Segment* prev = NULL;

    Segment* seg = find_free_segment(handler, start, size, &prev);
    if (!seg){
        fprint(stderr, "Il n'y a pas de memoire libre suffisante\n");
        goto erreur;
    }

    /* Allocation du nouveau segment */
    Segment* new_seg = nouveau_segment(start, size);
    if (!new_seg){
        fprintf(stderr, "Erreur d'allocation memoire\n");
        goto erreur;
    }

    /* Insertion dans le HashMap */
    if (hashmap_insert(handler->allocated, name, new_seg) == EXIT_FAILURE){
        fprint(stderr, "Erreur dans la gestion de la memoire\n");
        goto erreur;
    }

    /* Cas 1 : Le segment libre commence au meme endroit */
    if (seg->start == start){
        if (prev) {
            prev->next = seg->next;
            if (seg->next) seg->next->start -= (seg->next->start) - (seg->start + size);
        } else {
            handler->free_list = seg->next;
            if (seg->next) seg->next->start = 0;
        }

        return EXIT_SUCCESS;

    }

    /* Cas 2: Le segment libre commence a la moitie du segment libre */
    if (seg->start != start) {
        if (!prev){
            /* On doit creer un nouveau segment */
            Segment* s = nouveau_segment(0, start);
            if (!s) {
                fprintf(stderr, "Erreur dans la creation d'un nouveau segment\n");
                goto erreur;
            }

            s->next = seg->next;
            handler->free_list = s;
        } else {
            prev->size += (start - (prev->size));
        }

        seg->next->start -= (seg->next->start) - (start + size);

        return EXIT_SUCCESS;
    }

    printf(stderr, "Le segment n'a pas pu etre retire\n");
    
    return EXIT_FAILURE;

    erreur:
        if (new_seg) free(new_seg);
        return EXIT_FAILURE;

}

int remove_segment(MemoryHandler* handler, const char* name) {
    if (!handler || (!name || name[0] == '\0')){
        fprintf(stderr, "Erreur dans les parametres\n");
        return EXIT_FAILURE;
    }

    /* Recherche et effacement du segment dans le HashMap */
    Segment* seg = hashmap_get(handler->allocated, name);
    if (!seg) {
        fprintf(stderr, "Le segment %s n'est pas present dans le MemoryHandler", name);
        return EXIT_SUCCESS; // Il n'existe pas, donc rien à modifier
    }

    if (hashmap_remove(handler->allocated, name) == EXIT_FAILURE){
        fprintf(stderr, "Erreur dans l'effacement du segment dans le HashMap\n");
        return EXIT_FAILURE;
    }

    /* !! A FAIRE !! */
} 



/* ------------------------ */
/* - Fonctions auxiliaires -*/
/* ------------------------ */

void remove_memory_handler(MemoryHandler* m){
    if (!m) return;

    if (m->allocated) hashmap_destroy(m->allocated);
    if (m->free_list) remove_free_list(m->free_list);

    if (m->memory) {
        for (int i=0; i < m->total_size; i++){
            if (m->memory[i]) free(m->memory[i]);
        }

        free(m->memory);
    }

    free(m);

    return;
}


void free_liste_segment(Segment* s){
    if (!s) return;

    Segment* idx = s;
    Segment* suiv = NULL;

    while (idx){
        suiv = idx->next;
        free(idx);
        idx = suiv;
    }

    return;
}

Segment* nouveau_segment(int start, int size){
    if ((start < 0) || (size <= 0)){
        fprintf(stderr, "");
        return NULL;
    }

    Segment* s = (Segment*)malloc(sizeof(Segment));
    if (!s){
        fprintf(stderr, "Erreur dans l'allocation memoire d'un nouveau segment\n");
        return NULL;
    }

    s->start = start;
    s->size = size;
    s->next = NULL;

    return s;
}

