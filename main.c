#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#define HEAP_CAPACITY 65536
#define HEAP_ALLOC_CAPACITY 65536
#define HEAP_FREE_CAPACITY 65536


typedef struct{
    uint16_t size;
    char* chunk;
}space;

char HEAP[HEAP_CAPACITY] = {0} ;
uint16_t HEAP_SIZE = 0 ;

space HEAP_ALLOC[HEAP_ALLOC_CAPACITY] = {0};
uint16_t HEAP_ALLOC_SIZE = 0;

space HEAP_FREE[HEAP_FREE_CAPACITY] = {0} ;
uint16_t HEAP_FREE_SIZE = 0 ;


char* search_free_heap(uint16_t size){
    for(int i = 0 ; i < HEAP_FREE_SIZE ; i++){
        if(HEAP_FREE[i].size >= size) return HEAP_FREE[i].chunk;
        }
    return NULL;
}


void garbage_collection(void){
    for(int i = 0 ; i < HEAP_ALLOC_SIZE - 1 ; i++){
    // printf("I : HEAP_ALLOC[%d]: ptr : %p | size : %d \n",i,HEAP_ALLOC[i].chunk,HEAP_ALLOC[i].size);
        for(int j = 0 ; j < HEAP_ALLOC_SIZE ; j++){
            // printf("J : HEAP_ALLOC[%d] : ptr : %p \n",j,HEAP_ALLOC[j].chunk);
            if(HEAP_ALLOC[i].chunk + HEAP_ALLOC[i].size == HEAP_ALLOC[j].chunk){
                break;
            }
            if(j == HEAP_ALLOC_SIZE - 1){
                char * new_chunk = HEAP_ALLOC[i].chunk + HEAP_ALLOC[i].size;
                HEAP_FREE[HEAP_FREE_SIZE].chunk = new_chunk;
                uint16_t size = (uint16_t)65536;
                for(int i = 0 ; i < HEAP_ALLOC_SIZE ; i++){
                    if(HEAP_ALLOC[i].chunk > new_chunk){
                        if(HEAP_ALLOC[i].chunk - new_chunk < size){
                            size = HEAP_ALLOC[i].chunk - new_chunk;
                        }
                    }
                }
            }
        }
        
    } 
}


uint16_t remove_from_array(space array[], uint16_t size, char * chunk){
    uint16_t chunk_size = 0;
    for(int i = 0 ; i < size ; i++){
        if(chunk == array[i].chunk){
            chunk_size = array[i].size;
            for(int j = i ; j < size ; j++){
                array[j] = array[j + 1];
            }
            return chunk_size;
        }
    }
    return 0;
}


char* heap_alloc(uint16_t size){
    garbage_collection();
    char * chunk_ptr=  search_free_heap(size)  ;
    if(size > 0){
        if( size + HEAP_SIZE < HEAP_CAPACITY ) {
            if(chunk_ptr != NULL){
                HEAP_ALLOC[HEAP_ALLOC_SIZE].chunk = chunk_ptr;
                HEAP_ALLOC[HEAP_ALLOC_SIZE].size = size;
                HEAP_ALLOC_SIZE += 1;
                remove_from_array(HEAP_FREE, HEAP_FREE_SIZE, chunk_ptr);
                HEAP_FREE_SIZE -= 1;
            }else{
                chunk_ptr = &HEAP[ HEAP_SIZE ];
                HEAP_ALLOC[HEAP_ALLOC_SIZE].chunk = chunk_ptr;
                HEAP_ALLOC[HEAP_ALLOC_SIZE].size = size;
                HEAP_ALLOC_SIZE += 1;
                HEAP_SIZE += size;
                return chunk_ptr;
            }
        }
    }
    return NULL;
}


void heap_free(char* chunk_ptr){
    if(HEAP_ALLOC_SIZE > 0){
        uint16_t size = remove_from_array(HEAP_ALLOC, HEAP_ALLOC_SIZE, chunk_ptr);
        if(size > 0){
            HEAP_FREE[HEAP_FREE_SIZE].size = size ; 
            HEAP_FREE[HEAP_FREE_SIZE].chunk = chunk_ptr;
            HEAP_FREE_SIZE += 1;
            HEAP_ALLOC_SIZE -= 1;
            return;
        }
        assert(false && "THIS ADRESS DOES NOT BELONG TO THE HEAP \n");
    }
    assert(false && "THE HEAP IS EMPTY AND THERE IS NOTHING TO FREE \n");
}


void print_heap_alloc(void){
    
    for(int i = 0 ; i < HEAP_ALLOC_SIZE ; i++){
        printf("HEAP_ALLOC[%d] -> size :%d  | ptr = %p \n",i,HEAP_ALLOC[i].size,HEAP_ALLOC[i].chunk);
    }
}


void print_heap_free(void){
    for(int i = 0 ; i < HEAP_FREE_SIZE ; i++){
        printf("HEAP_FREE[%d] -> size :%d  | ptr = %p \n",i,HEAP_FREE[i].size,HEAP_FREE[i].chunk);
    }
}


int main(int argc, char** argv){
    for(int i = 0 ; i < 50 ; i++){
        heap_alloc(i);
    }

    print_heap_alloc();
    print_heap_free();
    char * ptr = 0;
    for(int i = 0 ; i < 20 ; i++){
        ptr = HEAP_ALLOC[i].chunk;
        heap_free(ptr);
    }
        
    print_heap_alloc();
    print_heap_free();

    for(int i = 41 ; i < 45784 ; i++){
        heap_alloc(i);
    }

}
