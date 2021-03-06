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
#define BEST_FIT 0
#define WORST_FIT 1

struct n			
{
	void* mem;				//pointer to memory
	size_t free_mem;		//amount of free memory
	struct n* next; 		//can be replaced by mem+free_mem		
	struct n* prev;
	int allocated;
};
typedef struct n node;

/* global variables */

node* bfm_head = NULL;
node* wfm_head = NULL;

/* Functions */

// Debug function to print out linked list
void print_ll(int setting);

//dealloc module
void dealloc(void *ptr, int setting);
//extfrag module
int count_extfrag(size_t size, int setting);


// Four Byte Allign the memory
size_t FBA(size_t size)
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
    
    // initilize the linked list
	bfm_head = bfm;
	bfm_head->free_mem = FBA(size-sizeof(node));
	bfm_head->mem = bfm_head + sizeof(node)+1;
	bfm_head->prev = NULL;
	bfm_head->next = NULL;
	bfm_head->allocated = FALSE;
	return 0;

}

int worst_fit_memory_init(size_t size)
{
	void* wfm = malloc(size);
	//if malloc fails or not enough size to store management stucture return false;
	if (wfm == 0 || size < sizeof(node)){
		return -1;
	}
    
    //initilize the linked list
	wfm_head = wfm;
	wfm_head->free_mem = FBA(size-sizeof(node));
	wfm_head->mem = wfm_head+sizeof(node)+1;
	wfm_head->prev = NULL;
	wfm_head->next = NULL;
	wfm_head->allocated = FALSE;
	return 0;	
}

/* memory allocators */
void *best_fit_alloc(size_t size)
{
	size = FBA(size);
	node* traverse = bfm_head;
	size_t temp = 0;
	node* temp_node = NULL;
    
    // loop through the linked list to find suitable block of memory for best fit
	while(traverse!=NULL){
		if( traverse->allocated == FALSE && traverse->free_mem >= size){
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
		traverse = traverse->next;
	}
	if(temp_node == NULL){
		return NULL;
	}
	else
	{
		temp_node->allocated = TRUE;
        
        // if not enough space for a new node return
		if(temp <= size + sizeof(node)){
			return temp_node->mem;
		}
        // if there is space for a new node, split the temp node
		else
		{
			node *new_node = temp_node->mem +size;
			
			new_node->mem = new_node + sizeof(node) + 1;
			new_node->free_mem = temp_node->free_mem - size -sizeof(node);
			new_node->allocated = FALSE;
			new_node->prev = temp_node;
			new_node->next = temp_node->next;

			if(new_node->next!=NULL){
				new_node->next->prev = new_node;
			}
			
			temp_node->free_mem = size;
			temp_node->mem = temp_node + sizeof(node)+1;
			temp_node->next = new_node;
            
			return temp_node->mem;
		}
	}

	return NULL;
}


void *worst_fit_alloc(size_t size)
{
	size = FBA(size);
	node* traverse = wfm_head;
	size_t temp = 0;
	node* temp_node = NULL;
    // loop through the linked list to find suitable block of memory for worst fit
	while(traverse!=NULL){
		if( traverse->allocated == FALSE && traverse->free_mem >= size){
			if(temp == 0)
			{
				temp = traverse->free_mem;
				temp_node = traverse;
			}
			else
			{
				if(traverse->free_mem>temp){
					temp = traverse->free_mem;
					temp_node = traverse;
				}
			}
		}
		traverse = traverse->next;
	}
	if(temp_node == NULL){
		return NULL;
	}
	else
	{
		temp_node->allocated = TRUE;
        // if not enough space for a new node return
        if(temp <= size + sizeof(node)){
			return temp_node->mem;
		}
        // if there is space for a new node, split the temp node
		else
		{
			node *new_node = temp_node->mem +size;
			
			new_node->mem = new_node + sizeof(node) + 1;
			new_node->free_mem = temp_node->free_mem - size -sizeof(node);
			new_node->allocated = FALSE;
			new_node->prev = temp_node;
			new_node->next = temp_node->next;

			if(new_node->next!=NULL){
				new_node->next->prev = new_node;
			}
			
			temp_node->free_mem = size;
			temp_node->mem = temp_node + sizeof(node)+1;
			temp_node->next = new_node;

			return temp_node->mem;
		}
	}

	return NULL;
}

/* memory de-allocator */
void best_fit_dealloc(void *ptr) 
{
	dealloc(ptr, BEST_FIT);
	return;
}

void worst_fit_dealloc(void *ptr) 
{
	dealloc(ptr, WORST_FIT);
	return;
}

void dealloc(void *ptr, int setting)
{
	if(ptr == NULL)
	{
		return;
	}
	node* temp_node = NULL;
    if(setting == BEST_FIT)
	{
		temp_node = bfm_head;
	}
	else if(setting == WORST_FIT)
	{
		temp_node = wfm_head;
	}

    // loop through the linked list to find the node to de-allocate
    while(temp_node->mem != ptr){
        temp_node = temp_node->next;
        //if it cannot be found return NULL
        if(temp_node == NULL) {
            return;
        }
    }
    //Set the block to unallocted
    temp_node->allocated = FALSE;
	
    node *next_node = temp_node->next;
    node *prev_node = temp_node->prev;
    
    //Recombine the current block with the next block if the next block is free block
    if((next_node != NULL) && (next_node->allocated == FALSE)){
        temp_node->free_mem = temp_node->free_mem + sizeof(node) + next_node->free_mem;
        
		if(next_node->next != NULL)
		{
			next_node->next->prev = temp_node;
		}
        temp_node->next = next_node->next;
    }
       
    //Recombine the current block with the previous block if the previous block is free block
    if((temp_node->prev != NULL) && (temp_node->prev->allocated == FALSE)){
        prev_node->free_mem = temp_node->free_mem + sizeof(node) + prev_node->free_mem;
        
        prev_node->next = temp_node->next;
        if(temp_node->next != NULL){
            temp_node->next->prev = prev_node;
        }
        temp_node = prev_node;
    }
	return;
}

/* memory algorithm metric utility function(s) */

/* count how many free blocks are less than the input size */ 
int best_fit_count_extfrag(size_t size)
{
    return (count_extfrag(size, BEST_FIT));
}

int worst_fit_count_extfrag(size_t size)
{
	// To be completed by students
    return (count_extfrag(size, WORST_FIT));
}

int count_extfrag(size_t size, int setting)
{
	node *temp_node = NULL;
	int count = 0;
	if(setting == BEST_FIT)
	{
		temp_node = bfm_head;
	}
	else if(setting == WORST_FIT)
	{
		temp_node = wfm_head;
	}
    // loop through linked list and count external fragmentation
	while(temp_node != NULL){
		
        if((temp_node->allocated == FALSE) && (temp_node->free_mem < size)){
            count++;
        }
        temp_node = temp_node->next;
    }
	return count;

}

//Print out the blocks information
void print_ll(int setting)
{
	node *traverse;
	printf("*************************************\n");
	printf("Mode: ");
	if(setting == BEST_FIT)
	{
		traverse = bfm_head;
		printf("BEST FIT \n");
	}
	else if(setting == WORST_FIT)
	{
		traverse = wfm_head;
		printf("WORST FIT\n");
	}
	else
	{
		printf("Invalid Setting, exiting\n");
		return;
	}
	int i=1;
	while(traverse!= NULL)
	{
		printf("Node %d\n", i);
		printf("\t Memory: %zu\n", traverse->free_mem);
		printf("\t Memory ptr: %p\n", traverse->mem);
		printf("\t Allocated: %d\n", traverse->allocated);
		printf("\t Node Address: %p\n", traverse);
		printf("\t Next Ptr: ");
		(traverse->next == NULL)? printf("NULL\n"): printf("%p\n", traverse->next);
		printf("\t Prev Ptr: ");
		(traverse->prev == NULL)? printf("NULL\n"): printf("%p\n", traverse->prev);
		traverse =  traverse->next;
		i++;	
	}
	printf("*************************************\n");
}
