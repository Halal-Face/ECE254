/**
 * @file memory.c
 * @brief: ECE254 Lab: memory allocation algorithm comparison template file
 * @author: Zahin & Virgil
 * @date: 2015-11-20
 */

/* includes */
#include <stdio.h> 
#include <stdlib.h> 
#include "mem.h"

/* defines */

/* global variables */
void* bfm = NULL;			//pointer to entire block of malloc'd memory

typedef struct node			//pointer to internal blocks of memory
{
	void* mem;				//pointer to memory
	size_t free_mem;		//amount of free memory
							// struct node* next; can be replaced by mem+free_mem		
	struct node* prev;
	int allocated;
} bfm_head*, wfm*;
/* Functions */
size_t FBA(size_t size)		// 4 Byte alligned
{ 
	(size%4 == 0)? return size: return (size +(4-size%4)) 
} 

/* memory initializer */
int best_fit_memory_init(size_t size)
{
	bfm = malloc(size);
	//if malloc fails or not enough size to store management stucture return false;
	if (bfm == 0 || size < sizeof(node)){
		return -1;
	}

	bfm_head = bfm;
	bfm_head->free_mem = FBA(size-sizeof(node));
	bfm_head->mem = bfm_head+1;
	bdm_head->prev = NULL;
	return 0;

}

int worst_fit_memory_init(size_t size)
{

	// To be completed by students
	// You call malloc once here to obtain the memory to be managed.
	return 0;

}

/* memory allocators */
void *best_fit_alloc(size_t size)
{
	size = FBA(size);

	if(bfm_head!=null && )

	return NULL;
}


void *worst_fit_alloc(size_t size)
{
	// To be completed by students
	return NULL;
}

/* memory de-allocator */
void best_fit_dealloc(void *ptr) 
{

	// To be completed by students
	return;
}

void worst_fit_dealloc(void *ptr) 
{

	// To be completed by students
	return;
}

/* memory algorithm metric utility function(s) */

/* count how many free blocks are less than the input size */ 
int best_fit_count_extfrag(size_t size)
{
	// To be completed by students
    node temp_node = bfm_head;
    int count = 0;
    
    while(temp_node != NULL){
        if((temp_node) && (temp_node->free_mem < size)){
            
        }
    }
    
    
	return 0;
}

int worst_fit_count_extfrag(size_t size)
{
	// To be completed by students
	return 0;
}
