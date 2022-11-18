/*

COSC 4333 Project
November 18, 2022
Nicholas Wade
Josh Mallari

*/

#include <algorithm>
#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#define MAX_LEN 200
#define NUM_COLORS 6
#define PORTNUM 50000 //default port

using namespace std;

struct terminal
{
	int id;
	string name;
	int socket;
	thread th;
	string chatroom;
};


vector<terminal> clients;
vector<string> chatrooms;


string def_col="\033[0m";

//colors for different users
string colors[]={"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m","\033[36m"};
int seed=0;
mutex cout_mtx,clients_mtx;

string color(int code);
void set_name(int id, char name[]);
void shared_print(string str, bool endLine);
int multicast_message(string message, int sender_id, string sender_rm);
int multicast_message(int num, int sender_id, string sender_rm);
void end_connection(int id);
void handle_client(int client_socket, int id);

int main(int argc, char const* argv[]) {
	
	if(argc != 2) // if argc (length of argv) != 2, error & quit
	{
		cout << endl << "ERROR: Invalid or Missing Port" << endl;
		cout << endl << "------ Specify port or type DEFAULT for default port " << PORTNUM << endl << endl;
		cout << endl << "Usage: " << argv[0] << " <port>" << endl << endl;
		return -1;
	}

	//debug print
	//cout << "Port is : " << argv[1] << endl << endl;
	

	int server_socket;
	if((server_socket=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket: ");
		exit(-1);
	}

	struct sockaddr_in server;
	server.sin_family=AF_INET;
	
	//debug print
	//cout << "Port is : " << server.sin_port << endl << endl;

	if(argc = 2 && strcmp(argv[1], "DEFAULT") != 0 )
		server.sin_port=htons((u_short)atoi(argv[1])); // set port no. of server
	else{
		cout << "\n\t  === note: using default port "<< PORTNUM <<" ===" << endl;
		server.sin_port=htons(PORTNUM); // set port no. of server
	}

	server.sin_addr.s_addr=INADDR_ANY;
	bzero(&server.sin_zero,0);

	if((bind(server_socket,(struct sockaddr *)&server,sizeof(struct sockaddr_in)))==-1)
	{
		perror("bind error: ");
		exit(-1);
	}

	if((listen(server_socket,8))==-1)
	{
		perror("listen error: ");
		exit(-1);
	}

	struct sockaddr_in client;
	int client_socket;
	unsigned int len = sizeof(sockaddr_in);

	cout << colors[NUM_COLORS-1] << "\n\t  ====== Chatroom Server Log Started ======   \n" << endl << def_col;

	while(1)
	{
		//establish connection with client
		if((client_socket=accept(server_socket,(struct sockaddr *)&client,&len)) == -1)
		{
			perror("accept error: ");
			exit(-1);
		}
		seed++;
		//starts client handler thread
		thread t(handle_client,client_socket,seed);
		lock_guard<mutex> guard(clients_mtx);
		clients.push_back({seed, string("Anonymous"),client_socket,(move(t))});
	}

	for(int i=0; i<clients.size(); i++)
	{
		if(clients[i].th.joinable())
			clients[i].th.join();
	}

	close(server_socket);
	return 0;
}

string color(int code)
{
	return colors[code%NUM_COLORS];
}

// Set name of client
void set_name(int id, char name[])
{
	for(int i=0; i<clients.size(); i++)
	{
			if(clients[i].id==id)	
			{
				clients[i].name=string(name);
			}
	}	
}

void set_room(int id, char room[])
{
	for(int i=0; i<clients.size(); i++)
	{
			if(clients[i].id==id)	
			{
				clients[i].chatroom=string(room);
				for(int j = 0; j < chatrooms.size(); i++)
					if(chatrooms.size() > 0 && clients[i].chatroom.compare(chatrooms[i]) == 0)
					{
						cout << "Room already exists" << endl;
					}
					else
					{
						chatrooms.push_back(clients[i].chatroom); 
						cout << "Room created" << endl;
					}
			}
	}	
}

// For synchronisation of cout statements
void shared_print(string str, bool endLine = true)
{	
	lock_guard<mutex> guard(cout_mtx);
	cout<<str;
	if(endLine)
			cout << endl;
}

// Broadcast message to all clients except the sender
int broadcast_message(string message, int sender_id)
{
	char temp[MAX_LEN];
	strcpy(temp,message.c_str());
	for(int i = 0; i < clients.size(); i++)
	{
		if(clients[i].id != sender_id)
		{
			send(clients[i].socket,temp,sizeof(temp),0);
		}
	}		
	return 0;	
}

// multicast a number to all clients except the sender
int multicast_message(string message, int sender_id, string sender_rm)
{
	char temp[MAX_LEN];
	strcpy(temp,message.c_str());
	for(int i = 0; i < clients.size(); i++)
	{
		if(clients[i].id != sender_id && clients[i].chatroom.compare(sender_rm) == 0)
		{
			send(clients[i].socket,temp,sizeof(temp),0);
		}
	}
	return 0;		
}

// multicast a number to all clients except the sender
int multicast_message(int num, int sender_id, string sender_rm)
{
	for(int i = 0; i < clients.size(); i++)
	{
		if(clients[i].id != sender_id && clients[i].chatroom.compare(sender_rm) == 0)
		{
			send(clients[i].socket,&num,sizeof(num),0);
		}
	}	
	return 0;		
}

// Broadcast a number to all clients except the sender
int broadcast_message(int num, int sender_id)
{
	for(int i = 0; i<clients.size(); i++)
	{
		if(clients[i].id != sender_id)
		{
			send(clients[i].socket,&num,sizeof(num),0);
		}
	}	
	return 0;		
}

void end_connection(int id)
{
	for(int i = 0; i<clients.size(); i++)
	{
		if(clients[i].id == id)	
		{
			lock_guard<mutex> guard(clients_mtx);
			clients[i].th.detach();
			clients.erase(clients.begin()+i);
			close(clients[i].socket);
			break;
		}
	}				
}

void handle_client(int client_socket, int id)
{
	char name[MAX_LEN],str[MAX_LEN], chatroom[MAX_LEN];
	recv(client_socket,name,sizeof(name),0);
	set_name(id,name);	

	recv(client_socket,chatroom,sizeof(chatroom),0);
	set_room(id,chatroom);

	// Display welcome message
	string welcome_message=string(name)+string(" has joined ")+string(chatroom);
	multicast_message("#NULL",id,chatroom);	
	multicast_message(id,id,chatroom);								
	multicast_message(welcome_message,id,chatroom);	
	shared_print(color(id)+welcome_message+def_col);
	
	while(1)
	{
		int bytes_received=recv(client_socket,str,sizeof(str),0);
		if(bytes_received<=0)
			return;
		if(strcmp(str,"LEAVE")==0)
		{
			// Display leaving message
			string message=string(name)+string(" has left");		
			multicast_message("#NULL",id,chatroom);			
			multicast_message(id,id,chatroom);						
			multicast_message(message,id,chatroom);
			shared_print(color(id)+message+def_col);
			end_connection(id);							
			return;
		}
		multicast_message(string(name), id, chatroom);					
		multicast_message(id, id, chatroom);		
		multicast_message(string(str), id, chatroom);
		shared_print(color(id) + name + " : " + def_col + str);		
	}	
}
