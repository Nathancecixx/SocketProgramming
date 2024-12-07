
#ifndef NETWORKINGASN3_AUTHHANDLER_H
#define NETWORKINGASN3_AUTHHANDLER_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>

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
    bool verifyUser(const std::string &username, const std::string &password);
    //U
    void addUser(const std::string &username, const std::string &password);
    //D
    bool saveAuth();

private:
    std::string dataFile;
    std::vector<Account> accounts;
    mutable std::mutex accountMutex;
};

#endif

