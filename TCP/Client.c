/******************************************************************************
 * This file contains all the code for the client runnin on my host windows 
 * machine.
 *
 * Lab #3 
 * CSCN72020 - Fall 24 - Nathan Ceci - Client
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <ws2tcpip.h>

//Tell linker to link Ws2_32.lib file
//(Some build systems youll need to 
// specify manually as a dependency)
#pragma comment(lib, "Ws2_32.lib")

#define BUFF_SIZE   25

#define CONN_PORT    27500
#define CONN_IP     "127.0.0.1"

int main(void){
    //Initialize windows WSA
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
        printf("ERROR: Failed to start WSA\n");
        return 1;
    }
    
    //Create the client socket
    SOCKET ClientSocket;
    ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ClientSocket == INVALID_SOCKET) {
        WSACleanup();
        printf("ERROR: Failed to create ServerSocket\n");
        return 0;
    }

    //Set TCP rules
    struct sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_port = htons(CONN_PORT);
    SvrAddr.sin_addr.s_addr = inet_addr(CONN_IP);
    if ((connect(ClientSocket, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR) {
        closesocket(ClientSocket);
        WSACleanup();
        printf("ERROR: Connection attempted failed\n");
        return 1;
    }

    //Recieve over connection into buffer
    char buffer[BUFF_SIZE] = "Hello World!\n";
    int Bytes_Sent = send(ClientSocket, buffer, sizeof(buffer), 0);

    if(Bytes_Sent != 0)
        printf("Sent message to server!\n");
    else
        printf("ERROR: Failed to send message over connection\n");

    //Close the socket and de-init WSA
    closesocket(ClientSocket);
    WSACleanup();
    return 0;

}