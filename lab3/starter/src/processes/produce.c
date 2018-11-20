#define _POSIX_SOURCE


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
void doProducerWork();
void doConsumerWork();
void myhandle(int mysignal);
void busy_loop(int iters);

//message queue's
mqd_t qdes;
char *qname = "/z5mohamm";

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
	
	int producerArray[num_p];	// array of producer P_ID
	int consumerArray[num_c];	// array of consumer P_ID
	
	//message queue setup. Each child process will see the same message box
	//Produers will send to qdes and consumers will receive form qdes
	mode_t mode = S_IRUSR | S_IWUSR;
	struct mq_attr attr;

	attr.mq_maxmsg  = maxmsg;
	attr.mq_msgsize = sizeof(int);
	attr.mq_flags   = 0;		/* a blocking queue  */
	
	qdes  = mq_open(qname, O_RDWR | O_CREAT, mode, &attr);
	if (qdes == -1 ) {
		printf("mq_open() failed. \n");
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
		// wait for producers to exit
		for(int i =0; i < (num_p); i++){
			waitpid(producerArray[i], &tmp, 0); 
		}
		//tight poll for message queue to have 0 items inside
		struct mq_attr a;
		mq_getattr(qdes, &a);
		while(a.mq_curmsgs !=0){
			mq_getattr(qdes, &a);
		}
		// kill and clean up  processes
		for(int i =0; i < (num_c); i++){
			kill( (pid_t)consumerArray[i] , SIGTERM);
			waitpid((pid_t)consumerArray[i], &tmp, 0); 
		}
		if (mq_close(qdes) == -1) {
			perror("mq_close() failed");
			exit(2);
		}
		
        gettimeofday(&tv, NULL);
        g_time[1] = (tv.tv_sec) + tv.tv_usec/1000000.;
        
        printf("System execution time: %.6lf seconds\n", \
               g_time[1] - g_time[0]);
	}
	
	exit(0);
}


void doProducerWork(){
	
	for(int i =0; i< num; i++){
		busy_loop(50000);
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
	sigset_t waiting_mask;

	signal(SIGTERM, myhandle); //register signal handeler
	
	while(1){
			//receive value from buffer
			if (mq_receive(qdes, (char *) &work, sizeof(int), 0) == -1) {
				
				//close mailbox before leaving
				if (mq_close(qdes) == -1) {
					exit(2);
				}
				exit(0);
			}
			// block signals after receiving from the mailbox
			sigsetmask(SIGINT);
			busy_loop(50000);
			temp = sqrt(work);
			if(temp*temp == work && work !=-1){
				printf("%d,%d,%d\n", consumer_id, work, temp);
			}
			// check and signals that were sent after blocking
			sigpending (&waiting_mask);
			if (sigismember (&waiting_mask, SIGINT)) {
			myhandle(0);

			}

	}
}
void myhandle(int mysignal){

	if (mq_close(qdes) == -1) {
		exit(2);
	}
	exit(0);
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

