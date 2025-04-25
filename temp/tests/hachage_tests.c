#include "../include/hachage.h"
#include <assert.h>

// printf("%s : Test x - Valide\n", __func__);

/* 
void test_func_name(){

    printf("---- %s ----\n", __func__);


    printf("%s : Test x - Valide\n", __func__);


    printf("%s : Test x - Valide\n", __func__);


    printf("--- end ---\n\n");
}
*/

void test_simple_hash(const char* str){

    printf("---- %s ----\n", __func__);

    /* Cas 1 : une chaine quelconque */
    assert(simple_hash("Ola") == 79);
    printf("%s : Test 1 - Valide\n", __func__);

    /* Cas 2 : Une chaine quelconque n'est pas dans la meme case que son equivalent */
    /* avec une lettre minuscule */
    assert(simple_hash("Ola") != simple_hash("ola"));
    printf("%s : Test 2 - Valide\n", __func__);

    /* Cas 3: Chaine vide*/
    assert(simple_hash(NULL) == ULONG_MAX);
    printf("%s : Test 3 - Valide\n", __func__);

    printf("%s : Tous les tests on ete validees !\n", __func__);
    	
    printf("--- end ---\n\n");
}

void test_hashmap_create(){

    printf("---- %s ----\n", __func__);

    HashMap* h = hashmap_create();
    if (!h){
        fprintf(stderr, "Erreur dans la creation du nouveau HashMap");
        return;
    }

    /* Cas 1: le HashMap et ses elements ne sont pas vides */
    assert(h != NULL);
    printf("%s : Test 1 - Valide\n", __func__);

    /* Le tableau de HashEntrys n'est pas NULL*/
    assert(h->table != NULL);
    printf("%s : Test 2 - Valide\n", __func__);

    /* Le size est 0 */
    assert(h->size == 0);
    printf("%s : Test 3 - Valide\n", __func__);

    printf("--- end ---\n\n");

}

void test_hashmap_insert(){
    
    
    HashMap* h = hashmap_create();
    if (!h){
        fprintf(stderr, "Erreur dans la creation du nouveau HashMap\n");
        return;
    }

    int var = 6;

    /* Cas 1 : Insertion d'un element avec la clé nulle */
    assert(hashmap_insert(h, NULL, &var) == EXIT_FAILURE);
    printf("%s : Test 1 - Valide\n", __func__);

    /* Cas 2 : Insertion d'un element avec la valeur nulle */
    assert(hashmap_insert(h, "cle", NULL) == EXIT_FAILURE);
    printf("%s : Test 2 - Valide\n", __func__);

    /* Cas 3 : Cle et valeur NULL */
    assert(hashmap_insert(h, NULL, NULL) == EXIT_FAILURE);
    printf("%s : Test 3 - Valide\n", __func__);

    /* Cas 4 : Les deux elements existent */
    assert(hashmap_insert(h, "cle", &var) == EXIT_SUCCESS);

    unsigned long hash = simple_hash("cle");
    if ((strcmp(h->table[hash].key, "cle") == 0) && (*(int*)(h->table[hash].value))){
        printf("%s : Test 4 - Valide\n", __func__);
    } else {
        printf("%s : Test 4 - Erreur\n", __func__);
    }

}

void test_hashmap_get(){

}


void test_hashmap_remove(){
    
}