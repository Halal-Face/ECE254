/**
  * ECE254 Linux Dynamic Memory Management Lab
  * @file: main_test.c
  * @brief: The main file to write tests of memory allocation algorithms
  */ 

/* includes */
/* system provided header files. You may add more */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* non-system provided header files. 
   Do not include more user-defined header files here
 */
#include "mem.h"

#define BEST_FIT 0
#define WORST_FIT 1

void randomDeAlloc(int setting, void* ptr[], int i);
int randomAlloc(int setting, void* ptr[]);
void print_ll(int setting);

int main(int argc, char *argv[])
{
	int num = 0;
	srand ( time(NULL) );

	void* ptr[30];
	printf("BEST FIT,");
	best_fit_memory_init(1024);	// initizae 1KB, best fit
	for(int i=0; i<101; i++)
	{
		randomDeAlloc(BEST_FIT, ptr, randomAlloc(BEST_FIT, ptr));
		num = best_fit_count_extfrag(5);
		printf("%d,", num);
	}
	printf("\n");
		

	
	void* ptr2[30];
	printf("WORSTT FIT,");
	worst_fit_memory_init(1024);	// initizae 1KB, best fit
	for(int i=0; i<101; i++)
	{
		randomDeAlloc(WORST_FIT, ptr2, randomAlloc(WORST_FIT, ptr2));
		num = worst_fit_count_extfrag(5);
		printf("%d,", num);
	}
	printf("\n");
	

	return 0;
}

int randomAlloc(int setting, void* ptr[])
{
	int i=1;
	void* p=NULL;
	//printf("After Random Alloc\n");
	if(setting == BEST_FIT)
	{
		int n=0;
		while(1)
		{
			n = rand() % 13 + 1;
			p = best_fit_alloc(n);
			if(p == NULL)
			{
				break;
			}
			else
			{
				ptr[i] = p;
				i++;
			}
			 
		}
		
		//print_ll(BEST_FIT);
	}
	else if(setting == WORST_FIT)
	{
		
		int n=0;
		while(1)
		{
			n = rand() % 13 + 1;
			p = worst_fit_alloc(n);
			if(p == NULL)
			{
				break;
			}
			else
			{
				ptr[i] = p;
				i++;
			}
			 
		}
		//print_ll(WORST_FIT);
	}
	
	return i;
}
void randomDeAlloc(int setting, void* ptr[], int i)
{
	if(setting == BEST_FIT)
	{
		for(int k = 1; k<i+1; k+=2)
		{
			best_fit_dealloc(ptr[k]);
			ptr[k] = NULL;
		}
	}
	else if(setting == WORST_FIT)
	{
		for(int k = 1; k<i+1; k+=2)
		{
			worst_fit_dealloc(ptr[k]);
			ptr[k] = NULL;
		}
	}
	//printf("After Dealloc\n");
	//print_ll(setting);
}
