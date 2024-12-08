// This client side is done by Mohammad Aljabrery and Cecil Foubert


/*to compiile into a single file:
navigate into this file's directory using terminal
do:
g++ clientAssignment3.cpp postMessage.cpp viewMessage.cpp -o CA3
then to run it, do:
./CA3
*/

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "postMessage.h"
#include "viewMessage.h"
#define MAX_SIZE 10000
#define MAX_USERNAAME_LEN 75
#define MAX_PASSWORD_LEN 75
//i set it to 10,000 just in case the user wants to see all the messages, then it would be a lot
using namespace std;



int main() {
    int clientSocket;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == -1) {
    cout << "ERROR: Failed to create ServerSocket" << endl;
    return 0;
    }


    sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_port = htons(27500);
    SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if ((connect(clientSocket, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr))) == -1) {
    close(clientSocket);
    cout << "ERROR: Connection attempted failed" << endl;
    return 0;
    }

    cout << "Connected to server!" << endl;

    // Menu
    //authentication loop
    bool isAuthenticated = false;

    while (true) {
    // Authentication loop
        char username[MAX_USERNAAME_LEN];
        char password[MAX_PASSWORD_LEN];
        while (!isAuthenticated) {
            string authMessage;
            cout<< "Please enter your Username: ";
            cin >> username;
            cout << "Please enter your Password: ";
            cin >> password;
            authMessage = "2 75 75 |||| ";
            authMessage.append(username);
            authMessage.append(" ");
            authMessage.append(password);
            send(clientSocket, authMessage.c_str(), authMessage.length(), 0);
            //wait for return.
            char buffer[3];//so they can send yes or no OR 0 1 -- Assumption

            recv(clientSocket, buffer, sizeof(buffer), 0);
            if (buffer == "yes" or atoi(buffer) == 0 or buffer == "YES"){
                isAuthenticated = true;
                cout << "Login Successful!\n";
            }else{
                cout << "Sorry, but that combination of username and password aren't in our system, please try again.\n";
            }
        }

        // main menu
        cout << "\n=== MENU ===\n1. Post a Message\n2. View Messages\n3. Exit\nEnter your choice number: ";
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice <1 || choice >3){
            cout << "Invalid choice. Please try again." << endl;
            continue;
        }

        if (choice == 1) { // posting
            string author, topic;

            //author input
            cout << "Would you like to post anonymously? (y/n): ";
            char anonymousStatus;
            cin >> anonymousStatus;
            cin.ignore();

            if (anonymousStatus == 'y' || anonymousStatus == 'Y') {
            author = "anonymous";
            } else if (anonymousStatus == 'n' || anonymousStatus == 'N') {
            cout << "Okay then, enter an author name: ";
            getline(cin, author);
            }
            else{
            cout << "Invalid choice. Please try again.";
            continue;
            }

            //topic input
            cout << "Now, would you like to post about a spicific topic? (y/n): ";
            char topicStatus;
            cin >> topicStatus;
            cin.ignore();

            if (topicStatus == 'y' || topicStatus == 'Y') {
            cout << "Enter topic name: ";
            getline(cin, topic);
            } else if (topicStatus == 'n' || topicStatus == 'N') {
            topic = "general";
            } else {
            cout << "Invalid choice. Please try again.";
            continue;
            }

            //message input
            cout << "Now, write your message (write 'POST' on a new line when finished): ";
            string message = MessageInput();

            //view the message before sending
            cout << "\nYour message is:\n"
                 << "Author: " << author << "\n"
                 << "Topic: " << topic << "\n"
                 << "Message:\n" << message << endl;

            cout << "confirm posting the message (y/n): ";
            char messageConfirmation;
            cin >> messageConfirmation;
            cin.ignore();

            //confirm the message before sending
            if (messageConfirmation == 'Y' || messageConfirmation == 'y') {
                //string postingMessage = constructPostMessage(author, topic, message);
                string postingMessage = constructPostMessage(author, topic, message);
                send(clientSocket, postingMessage.c_str(), postingMessage.length(), 0);


                //Are we gonna have a post confirmation from the server?    0 good, 1 bad -- Assumption
                //silly us forgot to talk about this in the application protocol
                char postConfirmationBuffer[1];
                recv(clientSocket, postConfirmationBuffer, sizeof(postConfirmationBuffer), 0);
                if (postConfirmationBuffer[0] == '0'){
                    cout << "Your Message was posted successfully" << endl;
                }
                else{
                    cout << "Failed to post your message" << endl;
                }
            } else {
                cout << "Posting cancelled.\n";
            }




        // Viewing
        } else if (choice == 2) {
            cout << "How would you like to filter by (0: No Fliter, 1: Author, 2: Topic): ";
            int filterFlag;
            cin >> filterFlag;
            cin.ignore();

            string authorView, topicView, viewingMessage;
            if (filterFlag == 1) {
                cout << "write the author you wish to filter for: ";
                getline(cin, authorView);
                viewingMessage = constructViewMessage("1", authorView);
            } else if (filterFlag == 2) {
                cout << "Enter topic you wish to filter for: ";
                getline(cin, topicView);
                viewingMessage = constructViewMessage("2", topicView);
            } else if (filterFlag == 0){
                string NoFilter = "NO_FILTER";
                viewingMessage = constructViewMessage("0", NoFilter);
            } else{
                cout << "Invalid choice, please try again.";
                continue;
            }
            send(clientSocket, viewingMessage.c_str(), viewingMessage.length(), 0);
            char buffer[MAX_SIZE];
            int byteRecieved = recv(clientSocket, buffer, sizeof(buffer), 0);
            if(byteRecieved > 0){
              buffer[byteRecieved] = '\0';
                cout << "Server response: \n" << buffer << endl; //show the posts
            }



        } else if (choice == 3) { // Exit
            cout << "\n====Logging Off, Goodbye! :D  ====\n";
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    //TODO: close socket
    close(clientSocket);

    return 0;
}