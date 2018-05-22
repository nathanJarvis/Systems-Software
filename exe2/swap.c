/**
 * Source name     : swap.c
 * Executable name : swap 
 * Version         : 1.0
 * Created date    : 01/26/2017
 * Author          : I-Ting Angelina Lee
 * Description     : This file define the swap function
 *                   that swaps values of two memory locations.
 *                   This file also contains testing code for the
 *                   swap functions. 
 **/

/** 
 * Input: Two memory locations that store 4-byte integers
 *
 * This function swaps values stored in the two memory locations. 
 **/
void swap(int *x, int *y) {
    int val_x, val_y;
    val_x = *x;
    val_y = *y;
    *y = val_x;
    *x = val_y;
;
    
}
