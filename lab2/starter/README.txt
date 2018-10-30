Modified the rt_MemBox_ext.c file and main_task_exp.c

The functions in rt_MemBox_ext contain rt_mem_alloc which allocates a memory box and rt_mem_free which frees a memory box.
The main_task_exp.c contains test cases to test rt_MemBox_ext.c. 

To run the project, install Keil microvision 4, download the zip file, and open RTX_APP.uvproj. After, batch build the project, select SIM as the target, and run in debug mode. 
The results will be displayed on UART console. If hardware is available, you can use the RAM Target instead.