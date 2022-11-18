/*

COSC 4333 Project
November 18, 2022
Nicholas Wade
Josh Mallari

*/

#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <signal.h>
#include <mutex>
#define MAX_LEN 200
#define NUM_COLORS 6
#define PORTNUM 50000 //default port

using namespace std;

bool exit_flag=false;
thread t_send, t_recv;
int client_socket;
string def_col="\033[0m";

//colors for messages
string colors[]={"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m"};

void catch_ctrl_c(int signal);
string color(int code);
int eraseText(int cnt);
void send_message(int client_socket);
void recv_message(int client_socket);

int main(int argc, char const* argv[]) {
	
	if(argc != 2) // if argc (length of argv) != 3, error & quit
	{
		cout << endl << "ERROR: Invalid or Missing Port" << endl;
		cout << endl << "------ Specify port or type DEFAULT for default port " << PORTNUM << endl << endl;
		cout << endl << "Usage: " << argv[0] << " <port>" << endl << endl;
		return -1;
	}
	
	//debug prints
	//cout << "IP Address is : " << argv[1] << endl;
	//cout << "Port is : " << argv[1] << endl << endl;

	cout <<" \n\n\t  ====== Welcome to the Chatroom Lobby ======   \n" << endl;

	if((client_socket=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket: ");
		exit(-1);
	}

	struct sockaddr_in client;
	client.sin_family=AF_INET;

	if(argc = 2 && strcmp(argv[1], "DEFAULT") != 0 )
		client.sin_port=htons((unsigned)atoi(argv[1])); // Port number of server
	else{
		cout << "\n\t  === note: using default port "<< PORTNUM <<" ===" << endl << endl;
		client.sin_port=htons(PORTNUM); // Port number of server
	}

	client.sin_addr.s_addr=INADDR_ANY;
	bzero(&client.sin_zero,0);

	//connects to the server
	if((connect(client_socket,(struct sockaddr *)&client,sizeof(struct sockaddr_in)))==-1)
	{
		perror("connect: ");
		exit(-1);
	}
	signal(SIGINT, catch_ctrl_c);
	char name[MAX_LEN];

	//prompt to enter username
	cout<<"Enter your name : ";
	cin.getline(name,MAX_LEN);
	send(client_socket,name,sizeof(name),0);

	char chatroom[MAX_LEN];
	
	//prompt to enter chatroom
	cout<< endl << "   ==== Enter a chatroom name if it exists. If not, it will be created. ====" << endl;
	cout<<"\nChatroom name : ";
	cin.getline(chatroom,MAX_LEN);
	send(client_socket,chatroom,sizeof(chatroom),0);

	//chatroom welcome prompt
	cout<<colors[NUM_COLORS-1]<<"\n\n\t  ====== Welcome to " << chatroom << " ======   \n"<<endl<<def_col;

	//start sending thread and receiving thread
	thread sendThread(send_message, client_socket);
	thread recvThread(recv_message, client_socket);

	t_send=move(sendThread);
	t_recv=move(recvThread);

	if(t_send.joinable())
		t_send.join();
	if(t_recv.joinable())
		t_recv.join();
			
	return 0;
}

// Handler for "Ctrl + C"
void catch_ctrl_c(int signal) 
{
	char str[MAX_LEN]="LEAVE";
	send(client_socket,str,sizeof(str),0);
	exit_flag=true;
	t_send.detach();
	t_recv.detach();
	close(client_socket);
	exit(signal);
}

string color(int code)
{
	return colors[code%NUM_COLORS];
}

// Erase text from terminal
int eraseText(int cnt)
{
	char back_space=8;
	for(int i=0; i<cnt; i++)
	{
		cout<<back_space;
	}
	return 0;	
}

// Send message to everyone
void send_message(int client_socket)
{
	while(1)
	{
		cout<<colors[1]<<"You : "<<def_col;
		char str[MAX_LEN];
		cin.getline(str,MAX_LEN);

		//sends message to server
		send(client_socket,str,sizeof(str),0);

		//exits when user types LEAVE
		if(strcmp(str,"LEAVE")==0)
		{
			exit_flag=true;
			t_recv.detach();	
			close(client_socket);
			return;
		}	
	}		
}

// Receive message
void recv_message(int client_socket)
{
	while(1)
	{
		if(exit_flag)
			return;
		char name[MAX_LEN], str[MAX_LEN];
		int color_code;
		int bytes_received=recv(client_socket,name,sizeof(name),0);
		if(bytes_received<=0)
			continue;

		//receives message from server
		recv(client_socket,&color_code,sizeof(color_code),0);
		recv(client_socket,str,sizeof(str),0);
		eraseText(6);
		if(strcmp(name,"#NULL")!=0)
			cout<<color(color_code)<<name<<" : "<<def_col<<str<<endl;
		else
			cout<<color(color_code)<<str<<endl;
		cout<<colors[1]<<"You : "<<def_col;
		fflush(stdout);
	}	
}
