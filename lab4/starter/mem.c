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
			//pointer to entire block of malloc'd memory

struct n			//pointer to internal blocks of memory
{
	void* mem;				//pointer to memory
	size_t free_mem;		//amount of free memory
	struct n* next; 		//can be replaced by mem+free_mem		
	struct n* prev;
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
	void* bfm = malloc(size);
	//if malloc fails or not enough size to store management stucture return false;
	if (bfm == 0 || size < sizeof(node)){
		return -1;
	}

	bfm_head = bfm;
	bfm_head->free_mem = FBA(size-sizeof(node));
	bfm_head->mem = bfm_head+sizeof(node)+1;
	bfm_head->prev = NULL;
	bfm_head->next = NULL;
	bfm_head->allocated = FALSE;
	//printf("bfm head %p\n", bfm_head);
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
	//printf("Allocating %d bytes\n", size);
	node* traverse = bfm_head;
	size_t temp = 0;
	node* temp_node = NULL;

	while(traverse!=NULL){
		if( traverse->allocated!=TRUE && traverse->free_mem >= size){
			if(temp == 0)
			{
				temp = traverse->free_mem;
				//printf("how many times\n");
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
		//printf("traverse: %p\n", traverse);
		traverse = bfm_head->next;
	}
	if(temp_node == NULL){
		return NULL;
	}
	else
	{
		//printf("Best fit found a block of %d bytes with p=%p \n", temp, temp_node);
		temp_node->allocated = TRUE;
		if(temp <= size + sizeof(node)){
			return temp_node->mem;
			//printf("%d: Inserted without splitting\n", size);
		}
		else
		{
			//printf("%d: By Splitting\n", size);
			//temp_node->free_mem -=temp;
			//node *new_node = temp_node->mem + size+1;
			
			
			node *new_node = temp_node->mem +size;
			
			
			//printf("New node has adress %p\n", new_node);
			new_node->mem = new_node+sizeof(node)+1;
			new_node->free_mem = temp_node->free_mem - size -sizeof(node);
			new_node->prev = temp_node;
			new_node->next = NULL;
			
			temp_node->free_mem = size;
			temp_node->next = new_node;
			//printf("%d: Temp Node Free Mem %d\n", temp_node->free_mem); 
			return temp_node->mem;
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
    node* temp_node = ptr - sizeof(node);
    
    //Set the block to unallocted
    temp_node->allocated = FALSE;
	//printf("Ptr has %p mem\n", ptr);
	//printf("Size of node %d\n", sizeof(node));
	//printf("Node to be de-allocated has %p pointer\n", temp_node);
	//printf("Node to be de-allocated has %d mem\n", (int)temp_node->free_mem);
    node *next_node = temp_node->mem + temp_node->free_mem;
    //Recombine the current block with the next block if the next block is free block
    if((next_node != NULL) && (next_node->allocated == FALSE)){
        temp_node->free_mem = temp_node->free_mem + sizeof(node) + next_node->free_mem;
    }
        
    //Recombine the current block with the previous block if the previous block is free block
    if((temp_node->prev != NULL) && (temp_node->prev->allocated == 0)){
        temp_node->prev->free_mem = temp_node->prev->free_mem + sizeof(node) + temp_node->free_mem;
        temp_node = temp_node->prev;
    }
	return;
}

void worst_fit_dealloc(void *ptr) 
{
	// To be completed by students
    node *temp_node = ptr - sizeof(node);
    
    //Set the block to unallocted
    temp_node->allocated = 0;
    node *next_node = temp_node->mem + temp_node->free_mem;
    
    //Recombine the current block with the next block if the next block is free block
    if((next_node != NULL) && (next_node->allocated == 0)){
        temp_node->free_mem = temp_node->free_mem + sizeof(node) + next_node->free_mem;
    }
    
    //Recombine the current block with the previous block if the previous block is free block
    if((temp_node->prev != NULL) && (temp_node->prev->allocated == 0)){
        temp_node->prev->free_mem = temp_node->prev->free_mem + sizeof(node) + temp_node->free_mem;
        temp_node = temp_node->prev;
    }
	return;
}

/* memory algorithm metric utility function(s) */

/* count how many free blocks are less than the input size */ 
int best_fit_count_extfrag(size_t size)
{
	// To be completed by students
    node *temp_node = bfm_head;
    int count = 0;
    
    while(temp_node != NULL){
        if((temp_node->allocated == 0) && (temp_node->free_mem < size)){
            count++;
        }
        temp_node = temp_node->mem + temp_node->free_mem;
    }
	return count;
}

int worst_fit_count_extfrag(size_t size)
{
	// To be completed by students
    node *temp_node = bfm_head;
    int count = 0;
    
    while(temp_node != NULL){
        if((temp_node->allocated == 0) && (temp_node->free_mem < size)){
            count++;
        }
        temp_node = temp_node->mem + temp_node->free_mem;
    }
	return count;
}
