/*----------------------------------------------------------------------------
 *      ECE254 Lab Task Management
 *----------------------------------------------------------------------------
 *      Name:    RT_MEMBOX_ext.C
 *      Purpose: Interface functions for blocking 
 *               fixed memory block management system
 *      Rev.:    V4.60
 *----------------------------------------------------------------------------
 *      This code is extends the RealView Run-Time Library.
 *      Created by University of Waterloo ECE254 Lab Staff.
 *---------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------
 *      Includes
 *---------------------------------------------------------------------------*/
 
#include "rt_TypeDef.h"
#include "RTX_Config.h"
#include "rt_System.h"
#include "rt_MemBox.h"
#include "rt_HAL_CM.h"
#include "rt_List.h"
#include "rt_Task.h"       /* added in ECE254 lab keil_proc */ 
#include "rt_MemBox_ext.h" /* added in ECE254 lab keil_proc */   


/* ECE254 Lab Comment: You may need to include more header files */

/*----------------------------------------------------------------------------
 *      Global Variables
 *---------------------------------------------------------------------------*/
 struct OS_XCB waiting_list; //Queue of tasks waiting for messages

/*----------------------------------------------------------------------------
 *      Global Functions
 *---------------------------------------------------------------------------*/

/*==========================================================================*/
/*  The following are added for ECE254 Lab Task Management Assignmet       */
/*==========================================================================*/

/**  
 * os_mem_alloc(mem_pool) calls this function.
 * @brief: Request a block of memory from the memory pool and block the calling
 * 			task if no memory is available in the memory pool
 */
void *rt_mem_alloc (void *mem_pool) {
	/* Add your own code here. Change the following line accordingly */
	P_TCB task;
	int taskID;
	void* ptr;


	ptr	= rt_alloc_box(mem_pool);
	
	//NULL ptr implies there was not enough memory in the memory pool
	if(ptr == NULL){
		taskID = rt_tsk_self();
		task = os_active_TCB[taskID - 1];
		//put task into waiting list
		rt_put_prio(&waiting_list, task);
		
		//Change task state to WAIT_MEM, to wait for memory
		rt_block(0xffff, WAIT_MEM);
		
		return NULL;
	}else{
		return ptr;	
	}
}


/**
 * os_mem_free(mem_pool, box) calls this function.
 * free the memory pointed by box, it will unblock any task that might be 
 * 		waiting for memory.
 * @return: OS_R_OK on success and OS_R_NOK if box does not belong to mem_pool 
 */
OS_RESULT rt_mem_free (void *mem_pool, void *box) {
	/* Add your own code here. Change the following line accordingly */
	P_TCB task;

	if(rt_free_box(mem_pool, box) == 0){
		if(&waiting_list.p_lnk != NULL){
			task = rt_get_first(&waiting_list);
			task->ret_val = (U32)box; /*Set its TCB ret_val to box*/
			task->state = READY; /*Unblock to READY state*/
			rt_dispatch(task);
			return (OS_R_OK);
		}else{
			return (OS_R_NOK);
		}
	}else{
		return (OS_R_OK); /* If the box does not belong to mem_pool*/
	}
}
/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
