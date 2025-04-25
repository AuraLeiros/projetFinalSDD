
#include "../include/memory_handler.h"
#include "assert.h"

// printf("%s : Test x - Valide\n", __func__);

/* 
void test_func_name(){

    printf("---- %s ----\n", __func__);


    printf("%s : Test x - Valide\n", __func__);


    printf("%s : Test x - Valide\n", __func__);


    printf("--- end ---\n\n");
}
*/

// printf("%s : Test x - Valide\n", __func__);


void test_memory_init(){

    printf("---- %s ----\n", __func__);

    /* Cas 1 : Size invalid (<= 0)*/
    assert(memory_init(0) == NULL);
    printf("%s : Test 1 - Valide\n", __func__);

    /* Cas 2: Valide */

    MemoryHandler* mh = memory_init(9);

    assert(mh != NULL);
    assert(mh->total_size == 9);
    assert(mh->free_list);
    
    printf("%s : Test 2 - Valide\n", __func__);


    printf("--- end ---\n\n");
}
