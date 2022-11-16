//NOT DONE ==========================================================
#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

#define NUM_THREADS 2
#define BUFFSIZE 256
#define INPUT_PROMPT "Input    : "
#define RECVD_PROMPT "Received : "

typedef struct send_thread_data {
	int thread_id;
	int num_Of_Iterations;
} sThreadData;

typedef struct recv_thread_data {
	int thread_id;
	int num_Of_Iterations;
} rThreadData;

void *worker_thread(void *thread_arg)
{
	
}

int main(int argc, char const* argv[]) {
	pthread_t threads[NUM_THREADS];
	int sock = 0, valread, client_fd;
	int rc;
	int number = 10;

	
		
	cout <<"main() : creating thread, " << i << endl;
	struct thread_data thread_params;
	thread_params.thread_id = i;
	thread_params.num_Of_Iterations = number;
		
	rc = pthread_create(&threads[i], NULL, worker_thread, &thread_params);

	sleep(0.2);

	if(rc){
		cout << "ERR: Unable to create thread, " << rc << endl;
		exit(1);
	}
	
	pthread_exit(NULL);
}

