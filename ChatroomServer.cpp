#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

#define MAX_THREADS 30

pthread_t threads[MAX_THREADS];
int rc;
int server_socket;

typedef struct listener_thread_data {
	int arg_count;
	char const* arg_var;

} listen_data_t;

void *listener_thread(void *thread_arg)
{
	struct listener_thread_data *listener_args;
	listener_args = (listener_thread_data *) thread_arg;
	char const* port = listener_args->arg_var;
	
	struct sockaddr_in client;
	int client_socket;
	unsigned int len = sizeof(sockaddr_in);
		
	while(true)
	{

		if((client_socket = accept(server_socket, (struct sockaddr *)&client, &len)) == -1)
		{
			perror("accept error: ");
			exit(-1);
		}else{
			cout << "client accepted" << endl;
		}

	}

	
	pthread_exit(NULL);
}

void *dispatcher_thread(void *thread_arg)
{
	while(true)
	{

	}	
	pthread_exit(NULL);
}

void *chatAdmin_thread(void *thread_arg)
{
	while(true)
	{

	}
	pthread_exit(NULL);
}

void *chatAdminSend_thread(void *thread_arg)
{
	while(true)
	{

	}
	pthread_exit(NULL);
}

void *chatAdminRecv_thread(void *thread_arg)
{
	while(true)
	{

	}
	pthread_exit(NULL);
}

int main(int argc, char const* argv[]) {
	
	//Port Check
	if(argc != 2) // if argc (length of argv) != 3, error & quit
	{
		cout << endl << "ERROR: Invalid Port" << endl << endl;
		cout << endl << "Usage: " << argv[0] << " <port>" << endl << endl;
		return -1;
	}





	//while(true){
		//int server_socket;
		
		if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			perror("socket: ");
			exit(-1);
		}

		struct sockaddr_in server;
		server.sin_family = AF_INET;
		server.sin_port = htons((u_short)atoi(argv[1]));
		server.sin_addr.s_addr = INADDR_ANY;

		if((bind(server_socket, (struct sockaddr *)&server, sizeof(struct sockaddr_in)))== -1)
		{
			perror("bind error: ");
			exit(-1);
		}

		if((listen (server_socket, 8)) == -1)
		{
			perror("listen error: ");
			exit(-1);
		}

		

		cout << endl << "========== Chat Server Active ==========" << endl << endl;
	//}


	//init vars
	pthread_t threads[MAX_THREADS];
	
	struct listener_thread_data listener;
	listener.arg_count = argc;
	listener.arg_var = argv[1];

	rc = pthread_create(&threads[0], NULL, listener_thread, &listener);

	if(rc)
	{
		cout << "ERR: Unable to create listener thread, " << rc << endl;
		exit(1);
	}else{
		cout << "Listener thread created" << endl;
	}

	while(true)
	{

	}

	//close(server_socket);
	return 0;
}
