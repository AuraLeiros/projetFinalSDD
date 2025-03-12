#include "testsHachage.h"

void test_simple_hash(){
    /* Cas 1 : str vide */
    unsigned long cas1 = simple_hash("");
    assert(cas1 == -1);

    /* Cas 2 : str */
    unsigned long cas2 = simple_hash("hello");
    assert(cas2 == 11);

    printf("simple_hash(): 2/2 tests reussis\n");

    return;
}
