/*
 * mm.c -
This malloc package uses an explict free list
ALLOCATED BLOCK
_________
|Header |
|NEXT   |
|PREV   |
|PAYLOAD|<-bp  
|FOOTER |
_________

FREE BLOCK
__________
|HEADER   |
|PREV_FREE|<-bp
|NEXT_FREE|
|OLD DATA |
|FOOTER   |
___________

free_list is added to the from the front. Blocks are removed and spliced in for updating size
in coalesce and allocation. The free_list aways ends at the prologue header which has an allocated bit
the list is traversed until this singular block with allcoated bit is hit
if any other block with allocated bit then the free list is corrupted


 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "CSE361",
    /* First member's full name */
    /* First member's WUSTL key */
    /* Second member's full name (leave blank) */
    "",
    /* Second member's WUSTL key (leave blank) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

//constants
#define WSIZE 4                                                                             
#define DSIZE 8                                                                             
#define CHUNKSIZE 1<<12  //intial heapsize                                                                     
#define MINBLOCK 16 
#define DEBUG 0
#define ALLOCATED 1
#define FREE 0

/*Macros*/                                                                  
#define MAX(x ,y)  ((x) > (y) ? (x) : (y))                                                
#define PACK(size, alloc)  ((size) | (alloc))                                               
#define GET(p)  (*(size_t *)(p))                                                            
#define PUT(p, value)  (*(size_t *)(p) = (value))                                           
#define GET_SIZE(p)  (GET(p) & ~0x7)                                                        
#define GET_ALLOC(p)  (GET(p) & 0x1)                                                        
#define HDRP(bp)  ((void *)(bp) - WSIZE)                                                    
#define FTRP(bp)  ((void *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)                               
#define NEXT_BLKP(bp)  ((void *)(bp) + GET_SIZE(HDRP(bp)))                                  
#define PREV_BLKP(bp)  ((void *)(bp) - GET_SIZE(HDRP(bp) - WSIZE))

//get next or prev pointer from free block
#define NEXT_FREEP(bp)  (*(void **)(bp + WSIZE))   //create a void *                                        
#define PREV_FREEP(bp)  (*(void **)(bp))         
                                           


//GLobal Variables                                                               
static void *start_list = NULL;
static void *end_list = NULL;



//explicit list helpers
static void add_free_list(void *bp);
static void remove_free_list(void *bp);
static void* find_block(size_t asize);

//extranious helper functions
static size_t align_size(size_t size);
static void *extend_heap(size_t words);
static void place_block(void *bp, size_t size);
static void *coalesce(void *bp);
static int mm_check(void);




/* 
 * mm_init - initialize the malloc package.
 create proglogue header and footer to more easily traverse stack
 like in the book
 have the free list point to the first free block
 extend heap by CHUNKSIZE
 */
int mm_init(void)
{
    //pointer to first block
    void *heap;
    void *tmp;

    heap = mem_sbrk(4 * DSIZE);
    if(heap == (void *)-1 ){
        printf("%s\n", "PROBLEM INITIALIZING MM.c");
        return -1;
    }

    
    PUT(heap, 0);//alignment
    PUT(heap + WSIZE, PACK(DSIZE, ALLOCATED));//prologue header
    PUT(heap + DSIZE, PACK(DSIZE, ALLOCATED)); //prologue footer 
    PUT(heap + 3*WSIZE, PACK(0, ALLOCATED)); //footer header
    
   
    //heap now points to first free block
    start_list = heap + DSIZE;
    heap = start_list;
    end_list = start_list + 3*DSIZE;

    tmp = extend_heap(CHUNKSIZE/WSIZE);
    
    if(tmp == NULL){
      printf("%s\n", "PROBLEM INITIALIZING MM.c");
      return -1;
    }
    
    //free list empty with no free blocks
    //if extend heap failed it should return -1   
    return 0;
    
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.

 make sure that the size request is not 0 and that the mm.c has been intialized

 if the requested size is less than the minimum block size bump up the size to acommodate
 look for free block in free list

 if found then allocate that free block

 if no sutiable block found then extend heap and place block
 */
void *mm_malloc(size_t size)
{
   size_t asize;
   size_t extend;
   void *bp;

   
   if(size == 0){

      return NULL;

   }


   //align size using formula from book
   asize =  align_size(size);

   asize = MAX(asize, MINBLOCK);

   bp = find_block(asize);

   //failed to find block must extend heap
   if( bp == NULL){

       extend = MAX(asize, CHUNKSIZE);
       bp = extend_heap(extend / WSIZE);
       //failed to extend heap
       if(bp == NULL){
          return NULL;
       }
    }
       

   //appropriate block has been found
   place_block(bp, asize);
   
   return bp;
}

  

/*
 * mm_free - Freeing a block does nothing.
 make sure that it is a valid pointer

 if valid pointer then get size of block so that allocated bit can be set to 0
 coalesce now that block is freed
 */
void mm_free(void *ptr)
{
   //from book
   size_t size;
   size = GET_SIZE(HDRP(ptr));
   PUT(HDRP(ptr), PACK(size, FREE));
   PUT(FTRP(ptr), PACK(size, FREE));
   coalesce(ptr);
   
   
   return;
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 make sure that size request is valid

 if size 0 then just free ptr
 if ptr NULL then just just malloc

  otherwise use malloc to and copy to block
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *new_ptr;
    size_t asize;

    //align size using formula from book
    asize =  align_size(size);
    asize = MAX(MINBLOCK, asize);

    //old_size = GET_SIZE(HDRP(ptr));
    //printf("OLD SIZE %d", old_size)

    if(size == 0){
      mm_free(ptr);
      return ptr;
    }

    else if(ptr == NULL){
      ptr = mm_malloc(size);
      return ptr;
    }
    
   
    new_ptr = mm_malloc(asize);

    if(new_ptr){
      memcpy(new_ptr, ptr, asize);
      mm_free(ptr);
    }
    return new_ptr;
    
  
}

//quick function to find aligned size for any given size
//defined in book
static size_t align_size(size_t size){

   return DSIZE * ((size + (DSIZE) + (DSIZE - 1))/ DSIZE);

}


/*
extend heap by n words
make sure that the size is DSIZE aligned otherwise add padding

if valid then move heap ptr use mem_sbrk and create free block of aligned size
update epilogue header in heap for the find_fit function


*/
static void *extend_heap(size_t words){
   void *bp;
   size_t size; //aligned byte size
   
   //from book
   size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
   bp = mem_sbrk(size);

   if((long)bp == -1){
     return NULL;
   }

   PUT(HDRP(bp), PACK(size, FREE)); //new block header
   PUT(FTRP(bp), PACK(size, FREE)); //new block footer
   PUT( HDRP(NEXT_BLKP(bp)) , PACK(0, ALLOCATED)); //new epilogue header 

    //add new block to beginning of free_list
   add_free_list(bp);

   if(DEBUG){
         mm_check();
   }
   
   return bp; 

   
                                             
}

/*

combine any neighboring free blocks in free_list into a larger block

*/

static void *coalesce(void *bp){
    //from book
    //case 1: prev and next block are both allocated
    //case 2: prev block is allocated while next block  is free
    //case 3: prev block is free while  next block is allocated  
    //case 4: both previous and next blocks are free

     //orginal size of block to be coalesced
    size_t size = GET_SIZE(HDRP(bp));

    /*allocation of previous and next blocks of bp
    edge cases of 
    -end of list will be at prologue footer and have a block of size 0 2 words down

    */
    size_t next_alloc = GET_ALLOC( HDRP(NEXT_BLKP(bp)) ) || NEXT_BLKP(bp) == NULL;
    size_t prev_alloc =  (bp == end_list) || GET_ALLOC( FTRP(PREV_BLKP(bp))) || PREV_BLKP(bp) == NULL;

    
       //case 1
    if(next_alloc && prev_alloc){
       add_free_list(bp);
       return bp;
    } 
    //case 2
    else if(!next_alloc && prev_alloc){
       remove_free_list( NEXT_BLKP(bp) ); //remove next block free list
       size += GET_SIZE( HDRP(NEXT_BLKP(bp))); 
       PUT(HDRP(bp), PACK(size, FREE)); //combine blocks
       PUT(FTRP(bp), PACK(size, FREE));

    }
    //case 3
    else if(next_alloc && !prev_alloc){
       remove_free_list( PREV_BLKP(bp) ); //remove prev block free list to update
       size += GET_SIZE( HDRP(PREV_BLKP(bp)));
       bp = PREV_BLKP(bp); 
       PUT(HDRP(bp), PACK(size, FREE)); //combine blocks
       PUT(FTRP(bp), PACK(size, FREE));


    }
    
    //case 4
    
    else if(!next_alloc && !prev_alloc){
       remove_free_list( PREV_BLKP(bp) ); //remove prev block free list to update
       remove_free_list( NEXT_BLKP(bp) ); //remove next block free list
       size += ( GET_SIZE( HDRP(PREV_BLKP(bp))) + GET_SIZE(HDRP(NEXT_BLKP(bp))) );
       bp = PREV_BLKP(bp);      
       PUT(HDRP(bp), PACK(size, FREE)); //combine blocks
       PUT(FTRP(bp), PACK(size, FREE));

    }
   
   else{
     //default case which should not be reached!
   }
   
    //add updated block to beginning of free_list
   add_free_list(bp);
   if(DEBUG){
      mm_check();
    }

   return bp;
}

//first fit traversal of free_list
   //prologue footer should have allocated bit which is always 1
   //end of the list
   //it is never freed
   //if no properly size block then return NULL

static void* find_block(size_t asize){
   void *tmp = start_list;
   void *bp = NULL;
   while(GET_ALLOC(HDRP(tmp)) != ALLOCATED){
      if(asize <=  GET_SIZE(HDRP(tmp))){
         
          bp = tmp;
          break;
      }
      //find next block
      tmp = NEXT_FREEP(tmp);

   }
   return bp;

}

//add block to beginning of free list

static void add_free_list(void *bp){
   if( bp != NULL ){
     PREV_FREEP(start_list) = bp;
     NEXT_FREEP(bp) = start_list;
     PREV_FREEP(bp) = NULL;
     start_list = bp;
     return;
  }
  else{
    printf("%s\n", "trying to add non allocated block to free list");
    exit(-1);
  }

}


//remove_free_list
/*

only edge case is removing first item in list where previous of bp is NULL
in that case make the next of bp the beginning of free list

the previous of the next block should then be the previous of the bp
this works with NULL

*/
static void remove_free_list(void *bp){
   void* bp_prev;
   bp_prev = PREV_FREEP(bp);

    if(bp_prev == NULL){                                                  
       //If there is no previous block
        start_list = NEXT_FREEP(bp); //Set the free list to the next block
    }                                    
    else{                                                                  
         NEXT_FREEP(bp_prev) = NEXT_FREEP(bp);
         
    }

    
    PREV_FREEP(NEXT_FREEP(bp)) = bp_prev;
  
    return;
}


/*
place block should take in an aligned block size

if the differenc between size of the block being used 
and the size needed are greater then the minimum block size
then split the block and add to free list

otherwise just allocated block and remove block from free list
*/
static void place_block(void *bp, size_t asize){
   void *new;
   size_t block_size = GET_SIZE(HDRP(bp));
   size_t extra = block_size - asize;

   //split block only if big enough
   if(extra < MINBLOCK){
       PUT( HDRP(bp),  PACK(block_size, ALLOCATED) );
       PUT( FTRP(bp),  PACK(block_size, ALLOCATED) );
       remove_free_list(bp);
       return;  
 
   }
   //spit and update blocks
   PUT(HDRP(bp), PACK(asize, ALLOCATED));
   PUT(FTRP(bp), PACK(asize, ALLOCATED));
   new = NEXT_BLKP(bp);
   PUT(HDRP(new), PACK(extra, FREE));
   PUT(FTRP(new), PACK(extra, FREE));
   // coalesce adds to free list
   remove_free_list(bp);
   coalesce(new);
   return;
   
    

}

static int mm_check(void ){
   void *bp = start_list;

   while(GET_ALLOC(HDRP(bp)) == 0){
       printf("BLOCK %p", bp );
       if(GET(HDRP(bp)) != GET(FTRP(bp))){
          printf("HDR and FTR do not match");
       }
       if(GET_ALLOC(HDRP(bp))){
          printf("ALLOCATED BLOCK IN FREE LIST\n");
       }
       if(GET_SIZE(HDRP(bp)) < MINBLOCK){
          printf("FREE BLOCK SIZE IS LESS THEN MINBLOCK");
       }
       printf("\n");
      //find next block
       bp = NEXT_FREEP(bp);

   }
   
   return 0;
}





