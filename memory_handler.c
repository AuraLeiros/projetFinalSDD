#include "memory_handler.h"

MemoryHandler* memory_init(int size) {
    if (size <= 0){
        fprintf(stderr, "Le size passé en parametre est nul");
        return NULL;
    }

    /* Allocation d'une nouvelle structure MemoryHandler */
    MemoryHandler* m = (MemoryHandler*)malloc(sizeof(MemoryHandler));
    if (!m) {
        fprintf(stderr, "Erreur dans l'allocation d'un nouveau memory handler\n");
        return NULL;
    }

    /* NOTE: la taille maximale de memoire que l'on peut allouer plus tard */
    m->total_size = size;
    
    /* Allocation et initialisation de la free_list */
    m->free_list = (Segment*)malloc(sizeof(Segment));
    if (!m->free_list) {
        fprintf(stderr, "Erreur dans l'allocation d'un nouveau segment\n");
        goto erreur;
    }

    m->free_list->size = size;
    m->free_list->start = 0;
    m->free_list->next = NULL;

    /* On initialise le pointer a la memoire allouee comme NULL */
    m->memory = NULL;

    return m;


    erreur:
        remove_memory_handler(m);
        return NULL;
}

Segment* find_free_segment(MemoryHandler* handler, int start, int size, Segment** prev) {
    if (!handler || !handler->free_list) {
        fprintf(stderr, "Erreur dans les parametres, veuillez verifier que handler n'est pas NULL\n");
        return NULL;
    }

    Segment* tmp = handler->free_list;
    while (!tmp) {

        if (tmp->start <= start && tmp->start + tmp->size >= start + size) {
            return tmp;
        }

        prev = &tmp;
        tmp = tmp->next;
    }

    return NULL;
}

int create_segment(MemoryHandler* handler, const char* name, int start, int size) {
    if (!handler || !handler->free_list || !handler->free_list->size) {
        fprintf(stderr, "Parametres invalides\n");
        return EXIT_FAILURE;
    }

    Segment* prev = NULL;

    Segment* seg = find_free_segment(handler, start, size, &prev);
    if (!seg) {
        fprintf(stderr, "Il n'y a pas de memoire libre suffisante\n");
        return EXIT_FAILURE;
    }

    /* Allocation et insertion dans la hashmap du nouveau segment */
    Segment* new_seg = malloc(sizeof(Segment));
    if (!new_seg) {
        fprintf(stderr, "Erreur d'allocation memoire\n");
        return EXIT_FAILURE;
    }
    new_seg->start = start;
    new_seg->size = size;
    new_seg->next = NULL;

    int insert = hashmap_insert(handler->allocated, key = name, value = new_seg);
    if (insert == EXIT_FAILURE) {
        fprintf(stderr, "Erreur dans la gestion de la memoire\n");
        return EXIT_FAILURE;
    }


    /* On doit considerer tous les cas pour l'allocation de memoire */
    /* cas 1: le segment a allouer passe parfaitement dans un segment de la free_list */
    if (seg->start == start && seg->size == size) {
        if (!prev){
        while (handler->free_list != prev){
            handler->free_list = handler->free_list->next;
        }
        handler->free_list->next = seg->next;
        } else {
            handler->free_list = seg->next;
        }
    }
    /* cas 2: le segment a inserer commence au meme indice que le segment de la free_list */
    else if (seg->start == start){
        while (handler->free_list != seg){
            handler->free_list = handler->free_list->next;
        }
        seg->start = start + size;
        seg->size -= size;
        handler->free_list = seg;
    }
    /* cas 3: le segment a inserer fini au meme indice que le segment libre */
    else if (seg->start + seg->size == start + size){
        while (handler->free_list != seg){
            handler->free_list = handler->free_list->next;
        }
        seg->size -= size;
        handler->free_list = seg;
    }
    /* cas 4: le segment a inserer est au milieu du segment libre */
    else {
        while (handler->free_list != seg){
            handler->free_list = handler->free_list->next;
        }
        Segment* tmp = seg;
        tmp->size = start - seg->start;
        seg->start = start + size;
        seg->size -= size + tmp->size;
        tmp->next = seg;
        handler->free_list = tmp;
    }

    fprintf(stderr, "Le segment a bien ete cree et insere dans la memoire\n");
    return EXIT_SUCCESS;
}

int remove_segment(MemoryHandler* handler, const char* name) {
    if (!handler || !name) {
        fprintf(stderr, "Erreur dans les parametres\n");
        return EXIT_FAILURE;
    }

    /* Recherche et suppression du segment dans la memoire allouee */
    Segment* seg = hashmap_get(handler->allocated, name);
    if (!seg) {
        fprintf(stderr, "Le segment n'a pas pu etre trouve\n");
        return EXIT_FAILURE;
    }
    hashmap_remove(handler->allocated, name);

    /* Reliberation du segment dans la free_list (en fait une reinsertion qui code pour de l'espace libre) */
    Segment* tmp = handler->free_list;
    /* Creation de deux pointeurs un vers le segment avant et l'autre vers celui apres le segment a reinserer */
    Segment *prev, *suiv = NULL;
    while (tmp->start < seg->start) {
         prev = tmp;
         suiv = tmp->next;
         tmp = tmp->next;
    }
    /* On doit se charger de 4 cas de reinsertion d'un segment dans la free_list */
    /* cas 1: le segment se colle directement aux limites de prev et suiv, il faut donc les fusionner */
    if (prev->start + prev->size == seg->start && suiv->start == seg->start + seg->size) {
          prev->next = suiv->next;
          prev->size += seg->size + suiv->size;
          free(suiv); free(seg);
    }
    /* cas 2: le segment doit etre fusionne avec prev mais ne touche pas aux limites de suiv */
    else if (prev->start + prev->size == seg->start) {
          prev->size += seg->size;
          free(seg);
    }
    /* cas 3: le segment doit eytre fusionne avec suiv et commence quelque part apres la fin de prev */
    else if (suiv->start == seg->start + seg->size) {
          suiv->start = seg->start;
          suiv->size += seg->size;
          free(seg);
    }
    /* cas 4: le segment s'insere sans fusion */
    else {
          prev->next = seg;
          seg->next = suiv;
    }

    fprintf(stderr, "Le segment a été supprimé avec succès et la mémoire a été relibérée.\n");
    return EXIT_SUCCESS;

}


/* Fonctions auxiliaires */


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

void remove_free_list(Segment* seg) {
    if (!seg) return;

    Segment* curr = seg;
    Segment* next;

    while (curr) {
        next = curr->next;
        free(curr);
        curr = next;
    }
}