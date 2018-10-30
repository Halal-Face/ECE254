/**
 * @file:   rt_Task_ext.c
 */
#include "rt_TypeDef.h"
#include "RTX_Config.h"
#include "rt_System.h"
#include "rt_Task.h"
#include "rt_List.h"
#include "rt_Robin.h"
#include "rt_HAL_CM.h"
#include "rt_Task_ext.h"

/*----------------------- rt_tsk_count_get --------------------------*/
/* added for ECE254 lab */
int rt_tsk_count_get (void) {
	// add your own code here
	int i = 0;
	int active_task_counter = 0;
	while(i < os_maxtaskrun){
		if(os_active_TCB[i] != NULL &&((P_TCB)(os_active_TCB[i]))->state != INACTIVE){
			active_task_counter++;
		}
		i++;
	}
	if(os_idle_TCB.state != INACTIVE){
			active_task_counter++;
	}

	return active_task_counter;
}

/*----------------------- rt_tsk_get --------------------------------*/
OS_RESULT rt_tsk_get (OS_TID task_id, RL_TASK_INFO *p_task_info) {
	//check if ID is valid
	P_TCB p_tcb;
	U32 bottom_of_local_stack;
	U32 last_written_stack_address = 0;
	U32 local_stack_freespace = 0;
	U32 local_stack_space_size = (U16)os_stackinfo;
	U8 stack_usage_percentage = 0;
	
	//ignore everything that is above os_maxtaskrun except idle task ID
	if(task_id >os_maxtaskrun && task_id != os_idle_TCB.task_id){
		return OS_R_NOK;
	}
	
	//OS idle is a valid task but not stored in os_active_TCB
	if(task_id == os_idle_TCB.task_id){
		//& retrieves the address for os_idle_TCB
		p_tcb = &os_idle_TCB;
	}
	else{
		p_tcb = (os_active_TCB[task_id-1]);
	}
	
	bottom_of_local_stack = (U32)(p_tcb->stack);
	if(p_tcb!=NULL){
		//Map Info
		p_task_info->state = p_tcb->state;
		p_task_info->prio = p_tcb->prio;
		p_task_info->task_id = p_tcb->task_id;
		p_task_info->ptask = p_tcb->ptask;
		
		/*DETERMINE STACK USAGE*/
		
		//If Running will need to check the PSP for last_written_stack_address
		if(p_task_info->state == RUNNING){
			last_written_stack_address = (U32)rt_get_PSP();
		}
		//If not running, can use regular tsk_stack variable
		else{
			last_written_stack_address = p_tcb->tsk_stack;
		}
		
		local_stack_freespace = last_written_stack_address - bottom_of_local_stack;
		stack_usage_percentage = (U8)(((local_stack_space_size - local_stack_freespace)*100)/(local_stack_space_size));
		p_task_info->stack_usage = stack_usage_percentage;


		return OS_R_OK;
	}
	return OS_R_NOK;
}
/* end of file */
