#include "AuthHandler.h"

AuthHandler::AuthHandler(const std::string &filename) {
    this->dataFile = filename;
}

bool AuthHandler::loadAuth() {
    std::lock_guard<std::mutex> lock(accountMutex);
    std::ifstream infile(dataFile);

    if (!infile) {
        std::cout << "No existing accounts to load from " << dataFile << "." << std::endl;
        return false;
    }

    accounts.clear();

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        Account account;
        if (!std::getline(iss, account.username, '|')) continue;
        if (!std::getline(iss, account.password, '|')) continue;
        accounts.push_back(account);
    }

    std::cout << "Loaded " << accounts.size() << " accounts from file." << std::endl;
    return true;
}

bool AuthHandler::verifyUser(const std::string &username, const std::string &password) {
    std::lock_guard<std::mutex> lock(accountMutex);
    for (const auto& account : accounts) {
        if (account.username == username && account.password == password) return true;
    }
    return false;
}

void AuthHandler::addUser(const std::string &username, const std::string &password) {
    std::lock_guard<std::mutex> lock(accountMutex);
    Account account;
    account.username = username;
    account.password = password;
    accounts.push_back(account);
    this->saveAuth();
}


bool AuthHandler::saveAuth() {
    std::lock_guard<std::mutex> lock(accountMutex);
    std::ofstream outfile(dataFile, std::ofstream::trunc);
    if (!outfile) {
        std::cerr << "Error: Could not open file for saving: " << dataFile << std::endl;
        return false;
    }

    for (const auto& account : accounts) {
        outfile << account.username << "|"
                << account.password << "|\n";
    }
    outfile.close();
    std::cout << "Saved " << accounts.size() << " accounts to file." << std::endl;
    return true;
}

