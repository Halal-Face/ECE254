// Use this to see if a number has an integer square root
#define EPS 1.E-7

/*
	Processes
*/

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
#include <signal.h>

double g_time[2];

void printTime();

void doProducerWork();
void doConsumerWork();
void myhandle(int mysignal);

//message queue's
mqd_t qdes;
char *qname = "/mailbox";



int pid;
int producer_id = 0;
int consumer_id = 0;
struct timeval tv;
int num;
	int maxmsg;
	int num_p;
	int num_c;
	

int main(int argc, char *argv[])
{
	
	//int i;
	if (argc != 5) {
		printf("Usage: %s <N> <B> <P> <C>\n", argv[0]);
		exit(1);
	}

	num = atoi(argv[1]);	/* number of items to produce */
	maxmsg = atoi(argv[2]); /* buffer size                */
	num_p = atoi(argv[3]);  /* number of producers        */
	num_c = atoi(argv[4]);  /* number of consumers        */
	
	int producerArray[num_p];
	int consumerArray[num_c];
	
	
		
	
	//message queue setup. Each child process will see the same message box
	//Produers will send to qdes and consumers will consume form qdes
	mode_t mode = S_IRUSR | S_IWUSR;
	struct mq_attr attr;

	attr.mq_maxmsg  = maxmsg;
	attr.mq_msgsize = sizeof(int);
	attr.mq_flags   = 0;		/* a blocking queue  */
	
	qdes  = mq_open(qname, O_RDWR | O_CREAT, mode, &attr);
	if (qdes == -1 ) {
		printf("mq_open() failed");
		exit(1);
	}
	
	//create producer processes
	for(int i = 0; i < num_p; i++ ){
		pid = fork();
		if (pid < 0){
			printf("Error in forking producer.\n");
		}
		if (pid == 0){
			producer_id = i;
			doProducerWork();
			break;
		}
		producerArray[i] = pid;
	}
	
	if(pid!=0){
		//create consumer processes
		for(int i =0; i< num_c; i++){
			pid = fork();
			if(pid < 0){
				printf("Error in forking consumer.\n");
			}
			if(pid == 0){
				consumer_id = i;
				doConsumerWork();
				break;
			}
			consumerArray[i] = pid;
		}
	}
	
	if(pid!=0){
		int tmp =0;
		for(int i =0; i < (num_p); i++){
			waitpid(producerArray[i], &tmp, 0); 
		}

		for(int i =0; i < (num_c); i++){
			kill( (pid_t)consumerArray[i] , SIGTERM);
			waitpid((pid_t)consumerArray[i], &tmp, 0); 
		}
		if (mq_close(qdes) == -1) {
			perror("mq_close() failed");
			exit(2);
		}
		
		printTime();
	}
	
	exit(0);
}

void printTime(){
	gettimeofday(&tv, NULL);
	g_time[0] = (tv.tv_sec) + tv.tv_usec/1000000.;


    gettimeofday(&tv, NULL);
    g_time[1] = (tv.tv_sec) + tv.tv_usec/1000000.;

    printf("System execution time: %.6lf seconds\n", \
            g_time[1] - g_time[0]);
}

void doProducerWork(){
	
	for(int i =0; i< num; i++){
		if(i%num_p == producer_id){
			//send your i to the queue;
			if (mq_send(qdes, (char *)&i , sizeof(int), 0) == -1) {
				printf("mq_send() failed\n");
			}
		}
	}
	if (mq_close(qdes) == -1) {
		perror("mq_close() failed");
		exit(2);
	}
	exit(0);
}
void doConsumerWork(){
	
	int work = -1;
	int temp = 0;
	signal(SIGTERM, myhandle);
	
	while(1){
			//receive value from buffer
			if (mq_receive(qdes, (char *) &work, sizeof(int), 0) == -1) {
				
				//close mailbox before leaving
				if (mq_close(qdes) == -1) {
					exit(2);
				}
				exit(0);
			}

			temp = sqrt(work);
			if(temp*temp == work && work !=-1){
				printf("%d,%d,%d\n", consumer_id, work, temp);
			}

	}
}
void myhandle(int mysignal){

	if (mq_close(qdes) == -1) {
		exit(2);
	}
	exit(0);
}

