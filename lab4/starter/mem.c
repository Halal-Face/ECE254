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
#define TRUE 1
#define FALSE 0
/* global variables */
void* bfm = NULL;			//pointer to entire block of malloc'd memory

struct n			//pointer to internal blocks of memory
{
	void* mem;				//pointer to memory
	size_t free_mem;		//amount of free memory
							// struct node* next; can be replaced by mem+free_mem		
	struct node* prev;
	int allocated;
};
typedef struct n node;
node* bfm_head = NULL;
node* wfm_head = NULL;

/* Functions */
size_t FBA(size_t size)		// 4 Byte alligned
{ 
	if(size%4){
		return (size +(4-size%4));
	}
	return size;
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
	bfm_head->mem = bfm_head+sizeof(node);
	bfm_head->prev = NULL;
	bfm_head->allocated = FALSE;
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

	node* traverse = bfm_head;
	size_t temp = 0;
	node* temp_node = NULL;

	while(traverse!=NULL){
		if( traverse->allocated!=TRUE && traverse->free_mem >= size){
			if(temp == 0)
			{
				temp = traverse->free_mem;
				temp_node = traverse;
			}
			else
			{
				if(traverse->free_mem<temp){
					temp = traverse->free_mem;
					temp_node = traverse;
				}
			}
		}
		traverse = traverse->mem + traverse->free_mem;
	}
	if(temp_node == NULL){
		return NULL;
	}
	else
	{
		temp_node->allocated = TRUE;
		if(temp == size){
			return temp_node->mem;
		}
		else
		{

			//temp_node->free_mem -=temp;
			node *new_node = temp_node->mem + size;
			new_node->mem = new_node+sizeof(node);
			new_node->free_mem = temp_node->free_mem - size;
			temp_node->free_mem = size; 

		}
	}

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
	return 0;
}

int worst_fit_count_extfrag(size_t size)
{
	// To be completed by students
	return 0;
}