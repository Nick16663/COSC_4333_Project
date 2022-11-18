========================== Compilation ==============================

First ensure both chatroomclient.cpp and chatroomserver.cpp are in the same folder or directory.


To compile chatroomserver.cpp, run in terminal:

	g++ -o <program_name> chatroomserver.cpp

For example,

	g++ -o chatServer chatroomserver.cpp



To compile chatroomclient.cpp, run in terminal:

	g++ -o <program_name> chatroomclient.cpp

For example,

	g++ -o chatClient chatroomclient.cpp


========================== Execution ==============================

Following compilation, type the syntax

	./<program_name> <port>

to run the compiled programs.



It would be preferred to run the server program first.

The below example will run the chat server and bind it to port 55555.

	./chatServer 55555


To exit or leave the chat server, type 

	LEAVE

or use keybind

	CTRL + C
