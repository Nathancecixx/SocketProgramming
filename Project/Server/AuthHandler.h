
#ifndef NETWORKINGASN3_AUTHHANDLER_H
#define NETWORKINGASN3_AUTHHANDLER_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

struct Account {
    std::string username;
    std::string password;
};

class AuthHandler {
public:

    //C
    AuthHandler(const std::string &filename);
    bool loadAuth();
    //R
    //U
    //D

private:
    std::string dataFile;
    std::vector<Account> accounts;
};

#endif

