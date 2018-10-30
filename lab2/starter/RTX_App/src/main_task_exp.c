/**
 * @brief: ECE254 Keil ARM RL-RTX Task Management Lab starter file that calls os_tsk_get()
 * @file: main_task_exp.c
 * @date: 2015/09/13
 */
/* NOTE: This release is for students to use */

#include <LPC17xx.h>
#include "uart_polling.h" 
#include <RTL.h>
#include "../../RTX_CM3/INC/RTL_ext.h" /* extended interface header file */
#include <stdio.h>
#include <string.h>

#define NUM_NAMES 7

#define BOX_SIZE 12
#define NUM_BOXES 5

#define FALSE 0
#define TRUE 1

_declare_box(memory_pool, BOX_SIZE, NUM_BOXES);


struct func_info {
  void (*p)();      /* function pointer */
  char name[16];    /* name of the function */
};

extern void os_idle_demon(void);
__task void taskXXX(void);
__task void task1(void);
__task void task2(void);
__task void task3(void);
__task void task4(void);
__task void init (void);
 
extern char *state2str(unsigned char state, char *str);
char *fp2name(void (*p)(), char *str);

OS_MUT g_mut_uart;
OS_MUT g_mut_evnt;
OS_TID g_tid = 255;


int  g_counter = 0;  // a global counter
char g_str[16];
char g_tsk_name[16];

void* mem_box;

//this flag is to let the tasks know if they are allocating memory without getting blocked or after getting blocked
int flag  = FALSE;


struct func_info g_task_map[NUM_NAMES] = \
{
  /* os_idle_demon function ptr to be initialized in main */
  {NULL,  "os_idle_demon"}, \
  {taskXXX, "taskXXX"},   \
  {task1, "task1"},   \
  {task2, "task2"},   \
	{task3, "task3"},   \
	{task4, "task4"},   \
  {init,  "init" }
};


/*--------------------------- taskXXX ---------------------------------*/
/* take all memory blocks except 1					                           */
/*---------------------------------------------------------------------*/
__task void taskXXX(void){
	mem_box = os_mem_alloc(memory_pool);
  printf("Taking all available memory \n");
    
	while (((U32)mem_box - (U32)memory_pool) / 12 < (NUM_BOXES-1)) {
        mem_box = os_mem_alloc(memory_pool);
	}
}
	

/*--------------------------- task1 -----------------------------------*/
/* try to take memory block and printf result                          */
/*---------------------------------------------------------------------*/
__task void task1(void){
	
	U32 box_num =0;
	
	while(1){
	
		/****/os_mut_wait(g_mut_uart, 0xFFFF);
		printf("Task 1 is trying to take memory.\n");
		/****/os_mut_release(g_mut_uart);
		flag = FALSE;
		mem_box = os_mem_alloc(memory_pool);
		box_num = ((U32)mem_box-(U32)memory_pool)/12;
		/****/os_mut_wait(g_mut_uart, 0xFFFF);
		if(flag == TRUE){
			
				printf("Task 1 took block %d after getting blocked.\n", box_num);
			
		}else{
				printf("Task 1 took block %d without getting blocked.\n", box_num);
			
		}
		/****/os_mut_release(g_mut_uart);
		os_dly_wait(2);
	}
}


/*--------------------------- task2 -----------------------------------*/
/* checking states of all tasks in the system                          */
/*---------------------------------------------------------------------*/
__task void task2(void){
	U8 i=1;
	RL_TASK_INFO task_info;
	/****/os_mut_wait(g_mut_evnt, 0xFFFF);
	
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("Task2: I am Doing Stuff that must occur before Task 1 runs\n");
	os_mut_release(g_mut_uart);
	
	/****/os_mut_release(g_mut_evnt);
	
  for(;;) {
		os_mut_wait(g_mut_uart, 0xFFFF);
		printf("\nTID\tNAME\t\t\t\tPRIO\tSTATE\n");
		os_mut_release(g_mut_uart);
			os_mut_wait(g_mut_uart, 0xFFFF); 
		for(i = 0; i <NUM_NAMES-1; i++) { // this is a lazy way of doing loop.
			if (os_tsk_get(i+1, &task_info) == OS_R_OK) {
				os_mut_wait(g_mut_uart, 0xFFFF);  
				printf("%d\t%s\t\t\t\t%d\t%s\n", \
							 task_info.task_id, \
							 fp2name(task_info.ptask, g_tsk_name), \
							 task_info.prio, \
							 state2str(task_info.state, g_str));
				os_mut_release(g_mut_uart);
			} 
		}
			
		if (os_tsk_get(0xFF, &task_info) == OS_R_OK) {
			os_mut_wait(g_mut_uart, 0xFFFF);  
			printf("%d\t%s\t\t\t%d\t%s\n\n", \
						 task_info.task_id, \
						 fp2name(task_info.ptask, g_tsk_name), \
						 task_info.prio, \
						 state2str(task_info.state, g_str));
			os_mut_release(g_mut_uart);
		} 
		os_mut_release(g_mut_uart);
		
		os_dly_wait(10);
		
	}
}

/*--------------------------- task3 -----------------------------------*/
/* try to take memory block and printf result                          */
/*---------------------------------------------------------------------*/
__task void task3(void){
	
	U32 box_num =0;
	
	while(1){
	
		/****/os_mut_wait(g_mut_uart, 0xFFFF);
		printf("Task 3 is trying to take memory.\n");
		/****/os_mut_release(g_mut_uart);
		flag = FALSE;
		mem_box = os_mem_alloc(memory_pool);
		box_num = ((U32)mem_box-(U32)memory_pool)/12;
		
		/****/os_mut_wait(g_mut_uart, 0xFFFF);
		if(flag == TRUE){
			
				printf("Task 3 took block %d after getting blocked.\n", box_num);
			
		}else{
			
				printf("Task 3 took block %d without getting blocked.\n", box_num);
		
		}
			/****/os_mut_release(g_mut_uart);
	}
}

/*--------------------------- task4 -----------------------------------*/
/* try to dealocate memory block                                       */
/*---------------------------------------------------------------------*/
__task void task4(void){
	OS_RESULT check_val;
	while(1){
				
				check_val = os_mem_free (memory_pool, mem_box);
				
				if(check_val == (OS_R_OK)){
					/****/os_mut_wait(g_mut_uart, 0xFFFF);
					printf("Memory is freed successfully.\n");
					/****/os_mut_release(g_mut_uart);
					flag = TRUE;
				}
				else{
					/****/os_mut_wait(g_mut_uart, 0xFFFF);
					printf("Memory is freed unsuccessfully.\n");
					/****/os_mut_release(g_mut_uart);
				}
		}
}
/*--------------------------- init ------------------------------------*/
/* initialize system resources and create other tasks                  */
/*---------------------------------------------------------------------*/
__task void init(void)
{
	
	_init_box(memory_pool,sizeof(memory_pool),BOX_SIZE);
	
	os_mut_init(&g_mut_uart);
	os_mut_init(&g_mut_evnt);
	
	
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: the init TID is %d\n", os_tsk_self());
	os_mut_release(g_mut_uart);
	
	g_tid = os_tsk_create(taskXXX, 10);  /* task XXX at priority 10 */
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: created taskXXX with TID %d\n", g_tid);
	os_mut_release(g_mut_uart);
  
	g_tid = os_tsk_create(task1, 4);  /* task 1 at priority 1 */
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: created task1 with TID %d\n", g_tid);
	os_mut_release(g_mut_uart);
  
	g_tid = os_tsk_create(task2, 3);  /* task 2 at priority 1 */
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: created task2 with TID %d\n", g_tid);
	os_mut_release(g_mut_uart);
	
	g_tid = os_tsk_create(task3, 4);  /* task 3 at priority 2 */
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: created task3 with TID %d\n", g_tid);
	os_mut_release(g_mut_uart);
	
	g_tid = os_tsk_create(task4, 2);  /* task 4 at priority 4 */
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: created task4 with TID %d\n", g_tid);
	os_mut_release(g_mut_uart);
  
	os_tsk_delete_self();     /* task MUST delete itself before exiting */
}

/** 
 * @brief: get function name by function pointer
 * @param: p the entry point of a function (i.e. function pointer)
 * @param: str the buffer to return the function name
 * @return: the function name string starting address
 */
char *fp2name(void (*p)(), char *str)
{
	int i;
  
	for ( i = 0; i < NUM_NAMES; i++) {
		if (g_task_map[i].p == p) {
			str = strcpy(str, g_task_map[i].name);  
			return str;
		}
	}
	strcpy(str, "ghost");
	return str;
}

int main(void)
{
	
	
	SystemInit();         /* initialize the LPC17xx MCU */
	uart0_init();         /* initialize the first UART */
  
  
	/* fill the fname map with os_idle_demon entry point */
	g_task_map[0].p = os_idle_demon;
  
	printf("\nCalling os_sys_init()...\n");
	os_sys_init(init);    /* initialize the OS and start the first task */
}
