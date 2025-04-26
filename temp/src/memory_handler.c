#include "../include/memory_handler.h"


MemoryHandler* memory_init(int size) {
    if (size <= 0){
        fprintf(stderr, "La taille passe en parametre est invalide\n");
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


    /* Verifier qu'il existe un segment libre suffisant */
    Segment* prev = NULL;

    Segment* seg = find_free_segment(handler, start, size, &prev);
    if (!seg){
        fprintf(stderr, "Il n'y a pas de memoire libre suffisante\n");
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
        fprintf(stderr, "Erreur dans la gestion de la memoire\n");
        goto erreur;
    }

    /* On doit considerer tous les cas pour l'allocation de memoire */
    /* cas 1: le segment a allouer passe parfaitement dans un segment de la free_list */
    if (seg->start == start && seg->size == size) {
        if (prev){
            prev->next = seg->next;
        } else {
            handler->free_list = seg->next;
        }
    }
    /* cas 2: le segment a inserer commence au meme indice que le segment de la free_list */
    else if (seg->start == start){
        seg->start = start + size;
        seg->size -= size;
    }
    /* cas 3: le segment a inserer fini au meme indice que le segment libre */
    else if (seg->start + seg->size == start + size){
        seg->size -= size;
    }
    /* cas 4: le segment a inserer est au milieu du segment libre */
    else {
        Segment* tmp = seg;
        tmp->start = start + size;
        tmp->size = seg->size + seg->start - tmp->start;
        seg->size -= (size + tmp->size);
        seg->next = tmp;
    }

    fprintf(stderr, "Le segment a bien ete cree et insere dans la memoire\n");
    return EXIT_SUCCESS;

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
        fprintf(stderr, "Le segment %s n'est pas present dans le MemoryHandler\n", name);
        return EXIT_SUCCESS; // Il n'existe pas, donc rien à modifier
    }

    if (hashmap_remove(handler->allocated, name) == EXIT_FAILURE){
        fprintf(stderr, "Erreur dans l'effacement du segment dans le HashMap\n");
        return EXIT_FAILURE;
    }
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



/* ------------------------- */
/* - Fonctions auxiliaires - */
/* ------------------------- */

void remove_free_list(Segment* s){
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

/* Pour allouer et initialiser un segment */
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

