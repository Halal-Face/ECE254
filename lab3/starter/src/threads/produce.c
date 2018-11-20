#define _POSIX_SOURCE

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include <semaphore.h>
#include <pthread.h>

double g_time[2];

void printTime();
void* producer();
void* consumer();
void busy_loop(int iters);


int num;
int maxmsg;
int num_p;
int num_c;

struct timeval tv;

int* buffer;
// buffer_ptr should always point to the next slot of the buffer where
// items can be added
int buffer_ptr = 0;
int num_consumed =0;

//for buffer operations
sem_t sem_buffer;
//for blocking and releasing producer if buffer is full
sem_t sem_buffer_add;
//for printing to the console
sem_t sem_printf;

int main(int argc, char *argv[])
{
	if (argc != 5) {
		printf("Usage: %s <N> <B> <P> <C>\n", argv[0]);
		exit(1);
	}
	
	num = atoi(argv[1]);	/* number of items to produce */
	maxmsg = atoi(argv[2]); /* buffer size                */
	num_p = atoi(argv[3]);  /* number of producers        */
	num_c = atoi(argv[4]);  /* number of consumers        */
    
    gettimeofday(&tv, NULL);
    g_time[0] = (tv.tv_sec) + tv.tv_usec/1000000.;

	pthread_t producers[num_p];			// array of producer threads
	pthread_t consumers[num_c];			// array of consumer threads
	
	sem_init( &sem_buffer, 0, 1 );
	sem_init( &sem_buffer_add, 0, maxmsg );
	sem_init( &sem_printf, 0, 1 );

	//initilize size of buffer
	buffer = (int*) malloc(maxmsg*sizeof(int));
	
	
	//create producers
	for(int i =0; i<num_p; i++){		
		pthread_create(&producers[i], NULL, producer, i);
	}
	//create consumers
	for(int i =0; i<num_c; i++){
		pthread_create(&consumers[i], NULL, consumer, i);
	}

	

	//join all consumer threads before exiting
	for(int i =0; i<num_c; i++){
		pthread_join(consumers[i], NULL);
	}
	//join all producer threads before exiting
	for(int i =0; i<num_p; i++){
		pthread_join(producers[i], NULL);	
	}

	//deallocate any heap allocated variables
	free(buffer);
    gettimeofday(&tv, NULL);
    g_time[1] = (tv.tv_sec) + tv.tv_usec/1000000.;
    
    printf("System execution time: %.6lf seconds\n", \
           g_time[1] - g_time[0]);
	
	
	exit(0);
}


void* producer(void *param){
	//get producer ID
	int ID = (int )(param);

	
	for(int i =0; i<num; i++){
		busy_loop(40000);
		if(i%num_p == ID){
			sem_wait(&sem_buffer_add);
			sem_wait(&sem_buffer);
			
			buffer[buffer_ptr] = i;
			buffer_ptr++;
			
			sem_post(&sem_buffer);
		}
	}
	
	pthread_exit(0);
}

void* consumer(void *param){
	int work = -1;
	int consumer_id = (int )(param); //cast to int ptr then dereference
	

	while(1){
		sem_wait(&sem_buffer);
		if(num_consumed == num){
			sem_post(&sem_buffer);
			
			pthread_exit(0);
		}
		else if(buffer_ptr > 0){ //if buff ptr >0 then there are items waiting to be consumed
			buffer_ptr--;
			work = buffer[buffer_ptr];
			num_consumed++;
			sem_post(&sem_buffer_add);
			sem_post(&sem_buffer);
		}else{
			sem_post(&sem_buffer);
			continue;	
		}
		busy_loop(40000);
		int temp = sqrt(work);
		if(temp*temp == work){
			sem_wait(&sem_printf);
			printf("%d,%d,%d\n", consumer_id, work, temp);
			sem_post(&sem_printf);
		}

		
	}

	
}
void busy_loop(int iters)
{
  volatile int sink;
  do
  {
    sink = 0;
  } while (--iters > 0);
  (void)sink;
}
