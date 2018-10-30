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

double g_time[2];

void printTime();

void doProducerWork();
void doConsumerWork();

//message queue's


mqd_t qdes;
char *qname = "/mailbox";

mqd_t pdes;
char *pname = "/pmailbox";

int pid;
int producer_id = 0;
int consumer_id = 0;

int main(int argc, char *argv[])
{
	int num;
	int maxmsg;
	int num_p;
	int num_c;
	int i;
	struct timeval tv;

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
	
	printTime();
		
	
	
	
	//message queue setup. Each child process will see the same message box
	char  quit = '\0';
	mode_t mode = S_IRUSR | S_IWUSR;
	struct mq_attr attr;

	attr.mq_maxmsg  = maxmsg;
	attr.mq_msgsize = sizeof(struct point);
	attr.mq_flags   = 0;		/* a blocking queue  */
	
	qdes  = mq_open(qname, O_RDWR | O_CREAT, mode, &attr);
	if (qdes == -1 ) {
		perror("mq_open() failed");
		exit(1);
	}
	
	//message queue setup for producers. Each producer process will see the number of items left to produce.
	char  pquit = '\0';
	mode_t pmode = S_IRUSR | S_IWUSR;
	struct mq_attr pattr;

	pattr.mq_maxmsg  = 1;
	pattr.mq_msgsize = sizeof(int);
	pattr.mq_flags   = 0;		/* a blocking queue  */
	
	pdes  = mq_open(pname, O_RDWR | O_CREAT, pmode, &pattr);
	if (pdes == -1 ) {
		perror("mq_open() failed");
		exit(1);
	}
	
	//TODO: Send num to the pdes.




	printf("Creating Producers.\n");
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
		printf("Created Producer with PID %d and ID %d.\n", PID, i);
	}
	
	if(pid!=0){
		printf("Creating Consumers.\n");
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
			printf("Created Consumer with PID %d and ID %d.\n", PID, i);
		}
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
	int produce = -1;
	
	//while the num in pdes is not 0 keep creating items
}
void doConsumerWork(){
	//check the qdes for items

}

