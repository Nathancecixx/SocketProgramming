#include "Server.h"

bool Server::Init(int BindPort, int MaxConnections, int MaxPacketSize) {
    if (!this->CreateSocket()) return false;
    this->bindPort = BindPort;
    this->maxConnections = MaxConnections;
    this->maxPacketSize = MaxPacketSize;
    this->postHandler.loadPosts();
    this->authHandler.loadAuth();
    return true;
}

bool Server::Start() {
    if (!this->BindnListen()) return false;

    int clientSocket;
    while((clientSocket = accept(serverSocket, NULL, NULL))){
        std::cout << "Client " << clientSocket << " connected" << std::endl;
        std::thread thread_obj(&Server::handleClient, this, clientSocket);
        thread_obj.detach();
    }
    return true;
}

bool Server::Close() {
    this->postHandler.savePosts();
    this->authHandler.saveAuth();
    if (serverSocket) close(serverSocket);
    return true;
}



bool Server::CreateSocket(){
    this->serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(this->serverSocket == -1){
        std::cout << "ERROR: Failed to create socket" << std::endl;
        close(this->serverSocket);
        return false;
    }
    return true;
}

bool Server::BindnListen(){
    struct sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_addr.s_addr = INADDR_ANY;
    SvrAddr.sin_port = htons(this->bindPort);
    if (bind(serverSocket, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr)) == -1)
    {
        close(serverSocket);
        printf("ERROR: Failed to bind ServerSocket\n");
        return false;
    }

    if(listen(serverSocket, maxConnections) == -1)
    {
        close(serverSocket);
        printf("ERROR: Failed to listen\n");
        return false;
    }

    return true;
}


void Server::handleClient(int clientSocket) {
    std::cout << "Client " << clientSocket << " is being handled" << std::endl;

    char clientBuffer[this->maxPacketSize];
    int bytesReceived;

    while ((bytesReceived = recv(clientSocket, clientBuffer, sizeof(clientBuffer) - 1, 0)) > 0) {
        clientBuffer[bytesReceived] = '\0';

        std::cout << clientBuffer << std::endl;

        // Wrap the received message in a string stream
        std::istringstream ss(clientBuffer);
        std::string token;

        // Extract the MessageTypeFlag
        std::getline(ss, token, ' ');
        char messageTypeFlag = token[0];

        if (messageTypeFlag == '0') {
            // Posting
            std::getline(ss, token, ' ');
            int authorLength = std::stoi(token);

            std::getline(ss, token, ' ');
            int topicLength = std::stoi(token);

            std::getline(ss, token, ' ');
            int messageLength = std::stoi(token);

            std::getline(ss, token, ' '); // Skip ||||

            std::getline(ss, token, ' ');
            std::string author = token.substr(0, authorLength);

            std::getline(ss, token, ' ');
            std::string topic = token.substr(0, topicLength);


            std::string message(messageLength, '\0');
            ss.read(&message[0], messageLength);


            // Add post
            std::string currentTime = this->GetCurrentTime();
            Post newPost = {author, topic, message, currentTime};
            postHandler.AddPost(newPost);
            postHandler.savePosts();

            std::string response = "0";
            send(clientSocket, response.c_str(), response.size(), 0);
        } else if (messageTypeFlag == '1') {
            // Viewing
            std::getline(ss, token, ' ');
            char filterFlag = token[0];

            std::getline(ss, token, ' ');
            int filterLength = std::stoi(token);

            std::getline(ss, token, ' '); // Skip ||||

            std::getline(ss, token, ' ');
            std::string filter = token.substr(0, filterLength);

            std::vector<Post> results;
            if (filterFlag == '0') {
                // Show all posts
                results = postHandler.GetAllPosts();
            } else if (filterFlag == '1') {
                // Filter by author
                results = postHandler.GetPostsByAuthor(filter);
            } else if (filterFlag == '2') {
                // Filter by topic
                results = postHandler.GetPostsByTopic(filter);
            }

            std::string response;
            if (results.size() > 0) {
                for (const auto& post : results) {
                    response += "|" + post.timestamp + "|\n" + post.author + "|" + post.topic + "|" + post.content + "\n\n";
                }
            } else {
                response = "No results found.";
            }

            response[response.length()] = '\0';
            std::cout << response << std::endl;
            send(clientSocket, response.c_str(), response.size(), 0);

        } else if (messageTypeFlag == '2') {
            // Authentication
            std::getline(ss, token, ' ');
            int usernameLength = std::stoi(token);

            std::getline(ss, token, ' ');
            int passwordLength = std::stoi(token);

            std::getline(ss, token, ' '); // Skip ||||

            std::getline(ss, token, ' ');
            std::string username = token.substr(0, usernameLength);

            std::getline(ss, token, ' ');
            std::string password = token.substr(0, passwordLength);

            //TODO:
            // edit based on client
            std::string response;
            if (this->authHandler.verifyUser(username, password)) response = "0";
            else response = "1";
            send(clientSocket, response.c_str(), response.size(), 0);
        } else {
            // Unknown type
            std::string response = "Invalid message type.";
            send(clientSocket, response.c_str(), response.size(), 0);
        }
    }

    if (bytesReceived == 0) {
        std::cout << "Client " << clientSocket << " disconnected." << std::endl;
    } else if (bytesReceived < 0) {
        std::cerr << "Error receiving data from client " << clientSocket << "." << std::endl;
    }

    close(clientSocket);
}

std::string Server::GetCurrentTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_c);

    // Format the time as "YYYY-MM-DD HH:MM:SS"
    std::stringstream timeStream;
    timeStream << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S");
    std::string currentTime = timeStream.str();
    return currentTime;
}


