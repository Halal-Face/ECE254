/**
 * @file memory.c
 * @brief: ECE254 Lab: memory allocation algorithm comparison template file
 * @author: 
 * @date: 2015-11-20
 */

/* includes */
#include <stdio.h> 
#include <stdlib.h> 
#include "mem.h"

/* defines */
#define FALSE 0
#define TRUE 1
/* global variables */
void *bfm; 				// pointer to best fit memory, return of malloc
typedef struct linked_list{
	size_t mem;
	uint8_t isAllocated;
	struct linked_list *next;
	struct linked_list *prev;
} *bfm_ll;
//size_t bf_size;			// size of best fit

size_t FBA(size){ return size +(4-size%4); } // 4 Byte alligned

/* Functions */

/* memory initializer */
int best_fit_memory_init(size_t size)
{
	bfm = malloc(size);
	if(bfm == FALSE){
		printf("Could not allocate %d bytes.\n", size);
		return -1
	}

	bfm_ll = bfm;
	bfm_ll->mem = FBA(bfm-FBA(sizeof(linked_list)));
	bfm_ll->isAllocated = FALSE;
	bfm_ll->next = NULL;
	bfm_ll->prev = NULL;

	return 0;

}

int worst_fit_memory_init(size_t size)
{
	
	
	return 0;

}

/* memory allocators */
/*
Instead of just walking through the list and splitting up the first block equal to or larger than N, we
could instead try to make a more intelligent decision. Considering all blocks, we choose the smallest block that is
at least as big as N. This produces the smallest remaining unallocated space at the end.
*/
void *best_fit_alloc(size_t size)
{	 

	linked_list *traverse = bfm_ll;		//pointer to first element
	linked_list *best_fit = NULL;
	size_t best_fit_mem = NULL;

	while(traverse!=NULL){
		if(traverse->isAllocated != && (traverse->mem) >= FBA(sizeof(linked_list) + size) && best_fit==NULL){
			best_fit_mem = traverse->mem - sizeof(linked_list) - size;
			best_fit = traverse;
		}
		else if(traverse->isAllocated != && (traverse->mem - (sizeof(linked_list) + size)) < best_fit_mem ){
			best_fit_mem = traverse->mem - sizeof(linked_list) - size;
			best_fit = traverse;
		}
	}
	if (best_fit == NULL){
		return NULL;
	}else{
		best_fit->prev->next = 
	}

	
}

/*The problem with best fit is that the leftover bits of memory are likely to be too small to be useful.
Rather than trying to find the smallest block that is of size N or greater, choose the largest block of free memory.
When the block is split, the remaining free block is, hopefully, large enough to be useful.
As with best fit, wemust either (1) check each available block; or (2) keep the block sorted by size, though decreasing
size this time. A max heap is appropriate, or a binomial or Fibonacci heap could also be appropriate [HZM14].
*/
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
	return 0;
}

int worst_fit_count_extfrag(size_t size)
{
	// To be completed by students
	return 0;
}
