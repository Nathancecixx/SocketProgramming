#include <csignal> // Used to call functions on interrupts
#include <iostream>

#include "PostHandler.h"
#include "AuthHandler.h"
#include "Server.h"

#define MAX_CONNECTIONS 5
#define BIND_PORT   1720

// Global variable
Server server;

void signalHandler(int signum);

int main() {

    //Call function when user interrupt given
    signal(SIGINT, signalHandler);

    server.Init(BIND_PORT, MAX_CONNECTIONS);
    //Blocking
    server.Start();

}

// Signal handler for graceful shutdown
void signalHandler(int signum) {
    std::cout << "\nInterrupt signal (" << signum << ") received.\n";
    server.Close();

    exit(signum);
}

