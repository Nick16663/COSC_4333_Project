#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define NUM_THREADS   2
#define BUFFSIZE      256
#define DEFAULT_PORT  55555
#define INPUT_PROMPT  "Input    : "
#define RECVD_PROMPT  "Received : "

using namespace std;

/*
typedef struct send_thread_data {
	int thread_id;
	int num_Of_Iterations;
} sThreadData;
*/

typedef struct recv_thread_data {
	int thread_id;
	int num_Of_Iterations;
} rThreadData;

void *worker_thread(void *thread_arg)
{
	return NULL;
}

bool exit_flag = false;
int client_socket;
int port;
char username[BUFFSIZE];
int rc;

int main(int argc, char const* argv[]) {
	
	//debug prints
	cout << "IP Address is : " << argv[1] << endl;
	cout << "Port is : " << argv[2] << endl << endl;
	
	if(argc != 3) // if argc (length of argv) != 3, error & quit
	{
		cout << endl << "ERROR: Invalid or Missing IP or Port" << endl << endl;
		cout << endl << "Usage: " << argv[0] << " <hostname> <port>" << endl << endl;
		return -1;
	}

	pthread_t threads[NUM_THREADS]; //create thread array

	int valread, client_fd;
	struct sockaddr_in client;

	client_socket = 0;
	
	
	if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n\n");
		return -1;
	}

	cout << "Client Socket creation success" << endl;

	client.sin_family = AF_INET;
	//client.sin_addr.s_addr = atoi(argv[1]);
	client.sin_addr.s_addr=INADDR_ANY;
	//client.sin_port = atoi(argv[2]);
	client.sin_port=htons(atoi(argv[2]));
	
	
	cout << "Before connect" << endl;
	cout << client_socket << endl;
	if((connect(client_socket, (struct sockaddr *)&client, sizeof(struct sockaddr_in)))==-1)
	{
		cout << "In connect if" << endl;
		perror("connecting... ");
		exit(-1);
	}
	
	cout << "after connect if" << endl;
	
	//signal(SIGINT, catch_ctrl_c);
	cout << endl << "==========================================" << endl << endl << "Enter a username : "; //make username
	cin.getline(username, BUFFSIZE);

	cout << "Username is : " << username << endl << endl; //debug print


	cout <<"main() : creating thread" << endl;
		
	//rc = pthread_create(&threads[i], NULL, worker_thread, &thread_params);

	//sleep(0.2);

	
	if(rc){
		cout << "ERR: Unable to create thread, " << rc << endl;
		exit(1);
	}
	
	pthread_exit(NULL);
	
}

void catch_ctrl_c(int signal)
{
	char str[BUFFSIZE] = "#exit";
	send(client_socket, str, sizeof(str), 0);
	exit_flag = true;
	//t_send.detach();
	//t_recv.detach();
	close(client_socket);
	exit(signal);
}

int eraseText(int cnt)
{
	char back_space = 8;
	for(int i = 0; i < cnt; i++)
	{
		cout << back_space;
	}

	return 1;
}

void send_msg(int client_socket)
{
	while(true)
	{
		cout << INPUT_PROMPT;
		char str[BUFFSIZE];
		cin.getline(str, BUFFSIZE);
		send(client_socket, str, sizeof(str), 0);
		if(strcmp(str, "#exit") == 0)
		{
			exit_flag = true;
			//t_recv.detach();
			close(client_socket);
			return;
		}
	}
}

void recv_message(int client_socket)
{
	while(true)
	{
		if(exit_flag)
			return;

		char name[BUFFSIZE], str[BUFFSIZE];
		int bytes_recvd = recv(client_socket, name, sizeof(name), 0);

		if(bytes_recvd <= 0)
			continue;

		recv(client_socket, str, sizeof(str), 0);

		eraseText(6);

		if(strcmp(name, "#NULL") != 0)
			cout << name << " : " << str << endl;
		else
			cout << str << endl;

		cout << INPUT_PROMPT;
		fflush(stdout);
	}
}
