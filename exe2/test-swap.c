/**
 * Source name     : test-swap.c
 * Executable name : test-swap 
 * Version         : 1.0
 * Created date    : 01/26/2017
 * Author          : I-Ting Angelina Lee
 * Description     : This file define the testing funciton 
 *                   for testing whether swap is implemented correctly
 *                   (i.e., it should swap values stored in
 *                    two memory locations).
 **/

#include <assert.h>
#include <stdlib.h>
#include "swap.h"

/**
 * This is a simple main that test the swap function.
 **/
int main(void) {
    
    int x, old_x;
    int y, old_y;

    x = old_x = rand();
    y = old_y = rand(); 
    
    // Make your call to swap here!
    swap(&x, &y); 
    assert(x == old_y);
    assert(y == old_x);

    return 0;
}
