#ifndef NETWORKINGASN3_SERVER_H
#define NETWORKINGASN3_SERVER_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>

#include "PostHandler.h"
#include "AuthHandler.h"


class Server {
public:
    Server()
    : postHandler("posts.txt"),
      authHandler("auth.txt"){}

    bool Init(int BindPort, int MaxConnections, int MaxPacketSize);
    bool Start();
    bool Close();
    
private:
    PostHandler postHandler;
    AuthHandler authHandler;
    int serverSocket;
    int bindPort;
    int maxConnections;
    int maxPacketSize;

    bool CreateSocket();
    bool BindnListen();
    void handleClient(int clientSocket);
    std::string GetCurrentTime();
};

#endif
