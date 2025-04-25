#include "testsHachage.h"

void test_simple_hash(){

    /* Cas 1 : str vide */
    assert(simple_hash("") == ULONG_MAX);

    /* Cas 2 : NULL char*/
    assert(simple_hash(NULL) == ULONG_MAX);

    /* Cas 3 : Chaîne valide */
    assert(simple_hash("hello") == 11);

    /* Cas 4 : Chaînes differentes */
    assert(simple_hash("hello") != simple_hash("Hello"))

    printf("simple_hash(): tous les tests on reussi\n");

    return;
}

void test_hashmap_insert(){

    HashMap* h = hashmap_create();
    if (!h) {
        fprintf(stderr, "Erreur dans la creation d'un nouveau hashmap\n");
        return;
    } 

    

    /* Cas 1 : Pas de hashmap passe en parametre */
    assert(hashmap_insert(NULL, "Key", &5) == EXIT_FAILURE);
    

}