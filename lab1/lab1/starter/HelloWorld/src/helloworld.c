/**
* @file: helloworld.c
* @brief: Two simple tasks running pseduo-parallelly
*/
#include <LPC17xx.h>
#include <RTL.h>
#include <stdio.h>
#include "uart_polling.h"
#include "../../RTX_CM3/INC/RTL_ext.h"


#define TASK_COUNT 6
#define DELAY 10


char* stateToString(U8 State, char* state_string){
	//char state_string[16];
	switch(State){
		
		case INACTIVE:
			strcpy(state_string, "INACTIVE");
			break;
		case READY:
			strcpy(state_string, "READY   ");
			break;
		case RUNNING:
			strcpy(state_string, "RUNNING ");
			break;
		case WAIT_DLY:
			strcpy(state_string, "WAIT_DLY");
			break;
		case WAIT_SEM:
			strcpy(state_string, "WAIT_SEM");
			break;
		case WAIT_MUT:
			strcpy(state_string, "WAIT_MUT");
			break;
		default:
			strcpy(state_string, "UNKOWN");
			break;
	}
	return state_string;
}
__task void task1(){
	printf("Task 1\n");
	os_dly_wait(DELAY);
	while(1){}
}
__task void task2(){
	printf("Task 2\n");
	os_dly_wait(DELAY);
	while(1){}
}
__task void task3(){
	printf("Task 3\n");
	os_dly_wait(DELAY);
	while(1){}
	
}

__task void testTask(){

	// Fill stack with identifying values
	U8 t[440];
	U32 i = 0;
	// Fill the stack with 0xAA's (see in Memory window of debugger)
	t[0] = 0xAA;
	for(i=1;i<sizeof(t)/sizeof(U8);i++) {
		t[i] = t[i-1];
	}
	// Keep the task running forever
	for(i=0;; i++){}
}

__task void printTask(){
	RL_TASK_INFO info;
	int i;
	char state[16];
	int id;
	int priority;
	int stack_usage;

	// Keep the task running forever
	while(1){
			//ID	STATE		PRIORITY		STACKUSAGE
			printf("ID\tSTATE\t\t\tPRIORITY\t\tSTACKUSAGE\n");
			printf("-------------------------------------------------------\n");
			for(i=1; i<=TASK_COUNT; i++){
				if(os_tsk_get(i, &info) == OS_R_OK){
				stateToString(info.state, state);
				id = info.task_id;
				priority = info.prio;
				stack_usage = info.stack_usage;
					printf("%d\t%s\t\t%d\t\t\t%d%%\n", \
									 id, \
									 state, \
									 priority, \
									stack_usage);
				
			}
		}
			if(os_tsk_get(255, &info) == OS_R_OK){
				//TODO FIX TEST FUNCTION
				stateToString(info.state, state);
				id = info.task_id;
				priority = info.prio;
				stack_usage = info.stack_usage;
					printf("%d\t%s\t\t%d\t\t\t%d%%\n\n", \
									 id, \
									 state, \
									 priority, \
									stack_usage);
				
			}
		os_dly_wait(DELAY);
	}
}
__task void init(void)
{
	os_tsk_create(task1, 1); // task2 at priority 1
	os_tsk_create(task2, 1); // task2 at priority 1
	os_tsk_create(task3, 1); // task2 at priority 1
	os_tsk_create(testTask, 1); // task2 at priority 1
	os_tsk_create(printTask, 1); // task2 at priority 1
	os_tsk_delete_self(); // must delete itself before exiting
}
int main ()
{
	SystemInit();
	uart0_init();
	os_sys_init(init);
}
