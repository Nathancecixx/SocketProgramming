/******************************************************************************
 * This file contains all the code for the server running on a VirtualBox 
 * Ubuntu machine that port forwards port 27000 on windows host to 
 * 10.5.5.5:27500.
 *
 * Lab #3 
 * CSCN72020 - Fall 24 - Nathan Ceci - Server
 *
 ******************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFF_SIZE   25
#define BIND_PORT   27000

int main(void){

	//Create Socket
	int ServerSocket;
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ServerSocket == -1) {
		printf("ERROR: Failed to create socket");
		return 1;
	}

	//Bind socket to port
	struct sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_addr.s_addr = INADDR_ANY;
	SvrAddr.sin_port = htons(BIND_PORT);
	if (bind(ServerSocket, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr)) == -1)
	{
		close(ServerSocket);
		printf("ERROR: Failed to bind ServerSocket\n");
		return 1;
	}

	//Listen for connections
	if (listen(ServerSocket, 1) == -1) {
		close(ServerSocket);
		printf("ERROR: listen failed to configure ServerSocket\n");
		return 1;
	}

	//Accept Connection
	int ConnectionSocket;
	ConnectionSocket = -1;
	if ((ConnectionSocket = accept(ServerSocket, NULL, NULL)) == -1) {
		close(ServerSocket);
        printf("ERROR: accept failed to create connection socket\n");
		return 1;
	}

    //Create and send a buffer containing message over connection
	char buffer[BUFF_SIZE];
	recv(ConnectionSocket, buffer, sizeof(buffer), 0);

    //Print out revieved message
    printf("Recieved Message: %s\n", buffer);

    //Close both sockets and exits
	close(ConnectionSocket);
	close(ServerSocket);
	return 0;
}
