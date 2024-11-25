/******************************************************************************
 * This file contains all the code for the client running on my second Virtualbox 
 * Ubuntu virtual machine. The virtual machines are connected on a virtualbox
 * virtual network, this code runs in ip "10.2.0.2"
 *
 * Lab #5 
 * CSCN72020 - Fall 24 - Nathan Ceci - Client
 *
 ******************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFF_SIZE   25

#define CONN_PORT    27000
#define CONN_IP     "10.2.0.1"

int main(void){
    //Create the client socket configured to use UDP
    int ClientSocket;
    ClientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (ClientSocket == -1) {
        printf("ERROR: Failed to create ClientSocket\n");
        return 0;
    }

    //Configure server address
    struct sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_port = htons(CONN_PORT);
    SvrAddr.sin_addr.s_addr = inet_addr(CONN_IP);

    //Send over connection from buffer
    char buffer[BUFF_SIZE] = "Hello World!\n";
    int Bytes_Sent = sendto(ClientSocket, buffer, sizeof(buffer), 0, (const struct sockaddr *) &SvrAddr, sizeof(SvrAddr));

    if(Bytes_Sent != -1)
        printf("Sent message to server!\n");
    else
        printf("ERROR: Failed to send message over connection\n");

    //Socklen_t needed when recieving from
    socklen_t SvrAddrLen = sizeof(SvrAddr);
    
    //Recieve over connection into buffer
    int Bytes_Recieved = recvfrom(ClientSocket, buffer, sizeof(buffer), 0, (struct sockaddr *) &SvrAddr, &SvrAddrLen);
    
    if(Bytes_Recieved == -1)
        printf("ERROR: Failed to recieve message over connection\n");
    else
    	printf("Recieved Message: %s\n", buffer);


    //Close the socket
    close(ClientSocket);
    return 0;

}