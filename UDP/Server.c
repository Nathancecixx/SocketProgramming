/******************************************************************************
 * This file contains all the code for the UDP/IP socket server running on my
 * Ubunti Virtualbox vm. The virtual machines are connected on a virtualbox virtual
 * network, this code runs in ip "10.2.0.1"
 *
 * Lab #5 
 * CSCN72020 - Fall 24 - Nathan Ceci - Server
 *
 ******************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define BUFF_SIZE   25
#define BIND_PORT   27000

int main(void){

	//Create Socket configured to use UDP
	int ServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
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

    	//Create and send a buffer containing message over connection
	char buffer[BUFF_SIZE];
	//Create the clients socket address and its length
	struct sockaddr CliAddr;
	socklen_t CliAddrLength = sizeof(CliAddr);
	
	//Receive from client
	int Bytes_Recieved = recvfrom(ServerSocket, buffer, sizeof(buffer), 0, &CliAddr, &CliAddrLength);
	
	//Print out revieved message
	if(Bytes_Recieved == -1)
        	printf("ERROR: Failed to recieve message over connection\n");
	else
    		printf("Recieved Message: %s\n", buffer);
    	
    	
    	//Send over connection from buffer
    	strncpy(buffer, "Hello Client, Welcome!\0", sizeof(buffer));
	int Bytes_Sent = sendto(ServerSocket, buffer, sizeof(buffer), 0, (const struct sockaddr *) &CliAddr, sizeof(CliAddr));

	if(Bytes_Sent != -1)
		printf("Sent welcome message back to client!\n");
	else
		printf("ERROR: Failed to send message over connection\n");

    	//Close socket
	close(ServerSocket);
	return 0;
}
