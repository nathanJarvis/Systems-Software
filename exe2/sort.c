/**
 * Source name     : sort.c
 * Executable name : sort 
 * Version         : 1.0
 * Created date    : 02/02/2017
 * Author          : I-Ting Angelina Lee
 * Description     : A program that, given an input <n>, 
 *                   generate an array of length n with random values
 *                   and sort the values using bubble sort 
 **/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "swap.h"

#define TRUE 1
#define FALSE 0

enum INPUT_TYPE { RANDOM, REVERSE, REPEAT };

/** 
 * Input: a string (an array of chars that is '\0' terminated).
 *
 * This function takes the string and converts it into an int
 *  using strtol library function.  It reports an error and 
 *  exits the program if an error is encountered.
 **/
static int parse_input(char *input) {
    errno = 0;
    int ret = strtol(input, NULL, 10); 
    if(errno != 0) {
        perror("Failed to parse input: ");
        exit(0);
    }
    if(ret == 0) {
        fprintf(stderr, "%s is not a valid input.\n", input);
        exit(0);
    }
    return ret;
}

/**
 * Input: an array of length n
 * 
 * This function generates inputs to fill the array.
 * If the INPUT_TYPE is RANDOM, the values are chosen randomly 
 *      between [0, n) to fill the array. 
 * If the INPUT_TYPE is REVERSE, the array is filled with
 *      value [n-1 .. 0] 
 * If the INPUT_TYPE is REPEAT , the array is filled with
 *      value in range [0, n/10] with likely repeats 
 **/
static void generate_input(int *array, int n, enum INPUT_TYPE type) {
    int i = 0;

    switch(type) {
    case RANDOM:
        for(i=0; i < n; i++) {
            array[i] = rand() % n;
        }
        break;
        
    case REVERSE:    
        for(i=0; i < n; i++) {
            array[i] = n-i;
        }
        break;

    case REPEAT:
        for(i=0; i < n; i++) {
            array[i] = rand() % ((n/10)+1);
        }
        break;
    }

    return;
}

/**
 * Input: array of length n containing random values
 * 
 * This function sorts the input array so that at the 
 * end of the function execution, the array contains values
 * in increasing order (i.e., smallest to largest).
 **/
static void sort_input(int *array,  int n) {
    int i, j;
    for(i = 0; i < n; i++){
       for(j =(i+1); j < n; j++){
	if(array[i] > array[j]){
	     //swap
	     swap(&array[i], &array[j]);	
	}	
      }
   }     
}

/**
 * Input: array of length n
 * This function prints out the array content.
 * (Probably not a good idea to use when the array is large!)
 **/
static void print_array(int *array, int n) {
    int i = 0;
    for(i=0; i < n; i++) {
        fprintf(stderr, "%d ", array[i]);
    }
    fprintf(stderr, "\n");
}
 
/**
 * Input: arary of length n
 * This function checks to see if the array is sorted.
 * If so, return TRUE; otherwise return FALSE.
 **/
static void check_array_sorted(int *array, int n) {
    int pass = TRUE;
    int i = 0;

    for(i=0; i < (n-1); i++) {
        if(array[i] > array[i+1]) {
            pass = FALSE; 
            break;
        }
    }

    if(pass) {
        printf("%d elements sorted successfully.\n", n);
    } else {
        printf("Sorting failed.\n\n");
        print_array(array, n);
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    
    if(argc < 2) {
        fprintf(stderr, "Usage: ./sort <n>\n");
        exit(0);
    } 
    errno = 0;

    int n = parse_input(argv[1]);
    int array[n];

    generate_input(array, n, RANDOM);
    sort_input(array, n);
    check_array_sorted(array, n);

    generate_input(array, n, REPEAT);
    sort_input(array, n);
    check_array_sorted(array, n);

    generate_input(array, n, REVERSE);
    sort_input(array, n);
    check_array_sorted(array, n);

    return 0;
}

