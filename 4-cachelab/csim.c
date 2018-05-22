/*Nathan Jarvis
 443499*/

#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>

#define true 1
#define false 0
#define DEBUG 0

int verbose = false;

typedef int bool;

typedef struct
{
   int B;
   int b;
   int E;
   int S;
   int s;
   int tag_size;

}properties;


typedef struct
{
   int hits;
   int misses;
   int evictions;

}cache_monitor;

typedef struct
{
   int access_num;
   int valid;
   unsigned long long tag;

}set_line;

typedef struct{
   int lines_used;
   int max;
   set_line *lines;

}cache_set;


typedef struct{
   cache_set *sets;

}cache;


cache create_cache(int E, int S){
   int j = 0;
   int i = 0;
   cache_set set;
   cache new_cache;
   set_line line;

   //create a zeroed set
   set.max = 0;
   set.lines_used = 0;


   //create a zeroed set line 
   line.access_num = -1;
   line.tag = -1;
   line.valid = false; 
 
   //allocate array of sets
   new_cache.sets = (cache_set*) malloc(sizeof(cache_set) * S);
   
   for(i = 0; i < S; i++){

      //allocate array of lines for each set
      set.lines = (set_line*) (malloc(sizeof(set_line) * E));
      new_cache.sets[i] = set;

      for(j = 0; j < E; j++){
          set.lines[j] = line;
          
      }
   }
   
   return new_cache;

}


void free_cache(cache *C, int S){

   int set_index = 0;
   for(set_index = 0; set_index < S ; set_index++){
      //free all lines 
      free(C->sets[set_index].lines);

   }
   //free all cache_sets
   free(C->sets);
   return;

}

void print_set(cache_set *set, int E){
   int i = 0;

   for(i = 0; i < E; i++){
      printf("\t VALID %d", set->lines[i].valid);
      printf(" TAG %llu", set->lines[i].tag);
      printf(" ACCESSES NUM  %d \n", set->lines[i].access_num);

   }


}

bool check_hit(cache_set *set, int E, unsigned long long check_tag){
   int line_index = 0;
   int hit = false;
   set_line* current_line;
   for(line_index = 0; line_index < E; line_index++){
       current_line = &(set->lines[line_index]);
      
      if( current_line->valid && (current_line->tag == check_tag)){
          current_line->access_num = set->max;
          set->max++;
          if(DEBUG) print_set(set, E);
          hit = true;
      }

   }

   return hit;
   

}

void place_line(cache_set *set, unsigned long long new_tag, int E){
     int index = set->lines_used;
     if(DEBUG){
        printf("BEFORE PLACING LINE\n");
        print_set(set, E);
     }
     
     set->lines[index].tag = new_tag;
     set->lines[index].valid = true;
     set->lines[index].access_num = set->max;
     set->max++;
     set->lines_used++;
   
     if(DEBUG){
        printf("AFTER PLACING LINE\n");
        print_set(set, E);

     }
    return;
}

void evict_line(cache_set *set, unsigned long long new_tag, int E){

   int i = 0;
   int min_index = 0;
   int min_num = set->lines[0].access_num;
   for(i = 1; i < E; i++){
      if(set->lines[i].valid){
     	 if(set->lines[i].access_num < min_num){
        	min_index = i;
                min_num = set->lines[i].access_num;
          }
      }
   }
   if(DEBUG){
     printf("BEFORE EVICT\n");
     print_set(set, E);
   }
   set->lines[min_index].tag = new_tag;
   set->lines[min_index].access_num = set->max;
   set->max++;
   if(DEBUG){ 
     printf("AFTER EVICT\n");
     print_set(set, E); 
   } 
   return;



}



void process_trace_line(cache *cache_inst, properties *cache_prop, cache_monitor *monitor, unsigned long long address)
{
   bool hit = false;
   int num_lines = cache_prop->E;
   unsigned long long  set_index = (address << cache_prop->tag_size)  >> (cache_prop->tag_size + cache_prop->b); 
   unsigned long long tag = address >> (cache_prop->b + cache_prop->s);
   cache_set *set = &(cache_inst->sets[set_index]);
   int lines_used = set->lines_used;
   
   if(DEBUG){
      printf("\tLine is in Set %llu with Tag %llu\n", set_index, tag);
      printf("\tSet %llu currently has %d lines used\n", set_index, set->lines_used);
   }
   
   hit = check_hit(set, num_lines, tag);
   
   if(hit){
       if(verbose) printf(" hit");
       if(DEBUG) printf("\n HIT \n");
       monitor->hits++;
       return;
   }
   else if (lines_used == num_lines){
      //need to evict with full lines
      if(DEBUG) printf("\n MISS EVICT \n");
      if(verbose) printf(" miss eviction");
      evict_line(set, tag, num_lines);
      monitor->misses++;
      monitor->evictions++;
      return;
   }
   else if(lines_used < num_lines){
       if(DEBUG) printf("\n MISS \n");
       if(verbose) printf(" miss");
       place_line(set, tag, num_lines);
       monitor->misses++;
       return;

   }
   else{
      printf("Trying to use more lines than available in set\n");
      exit(1);

   }
   return;
   

}



void print_help()
{

   printf("Usage: ./csim [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n");
   printf("-h: Optional help flag that prints usage info\n");
   printf("-v: Optional verbose flag that displays trace info\n");
   printf("-s <s>: Number of set index bits (S = 2s is the number of sets)\n");
   printf("-E <E>: Associativity (number of lines per set\n");
   printf("-b <b>: Number of block bits (B = 2b is the block size)\n");
   printf("-t <tracefile>: Name of the valgrind trace to replay\n");



}



int main(int argc, char** argv)
{
   properties properties;
   cache sim_cache;
   cache_monitor results;
   int num_bytes;
   int option;
   char* trace;
   char type;
   unsigned long long address;
   FILE *file = NULL;
   bool first_line = true;
   memset(&properties, 0, sizeof(properties)); 
   memset(&results, 0, sizeof(cache_monitor));

    while((option = getopt(argc, argv, "s:E:b:t:v::h::")) != -1){ 
          switch(option){
          case 'h':
             print_help();
             exit(0);
          case 'v':
             //cannot have both debug and verbose
             verbose = !DEBUG;
	     break;
          case 's':
             properties.s = atoi(optarg);
             //S = 2^s
             properties.S = (int) 2 << properties.s;
             break;
          case 'E':
             properties.E = atoi(optarg);
             break;
          case 'b':
             properties.b = atoi(optarg);
             //B = 2^b
             properties.B = (int) 2 << properties.b;
             break;
          case 't':
             trace = optarg;
             break;
          default:
              printf("Missing arguements or unknown option\n");
              print_help();
              exit(1);
          }


    }
        
    properties.tag_size = (64 - (properties.b + properties.s));
    
    file = fopen(trace, "r");
    if(!file){
       printf("failed to open trace\n");
       exit(1);
    }

    sim_cache = create_cache(properties.E, properties.S);
    //addresses are in Hex :(
    while(fscanf(file, " %c %llx,%d", &type, &address, &num_bytes) == 3){
      
      if(verbose || DEBUG){
         //formating for the first line
         if(first_line){
            printf("%c %llx,%d", type, address, num_bytes);
            first_line = false;
         }
         else{
            printf("\n%c %llx,%d", type, address, num_bytes); 
         }    
      }


      switch(type){
          //case L falls through to S
          //I should be ignored
          case 'L':
          case 'S':
             process_trace_line(&sim_cache, &properties, &results, address);
             break;
          case 'M':
             process_trace_line(&sim_cache, &properties, &results, address);
             process_trace_line(&sim_cache, &properties, &results, address);
             break;
          case 'I':
          default:
             break;        
      }

    }
    //formatting
    if(verbose) printf("\n");
    printSummary(results.hits, results.misses, results.evictions);
    fclose(file);
    free_cache(&sim_cache, properties.S);
    return 0;
}
