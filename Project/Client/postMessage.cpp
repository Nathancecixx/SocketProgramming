#include "postMessage.h"
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include "postMessage.h"
#define AUTHOR_LENGTH 50
#define TOPIC_LENGTH 50
#define MESSAGE_LENGTH 1000
using namespace std;

string constructPostMessage(const string author, const string topic, const string message){
    int authorLength = author.length();
    int topicLength = topic.length();
    int messageLength = message.length();

    return (
           "0 "
         + to_string(authorLength)
         + " "
         + to_string(topicLength)
         + " "
         + to_string(messageLength)
         + " |||| "
         + author
         + " "
         + topic
         + " "
         + message
    );
}

string MessageInput(){
    string message, line;
    while (1) {
        getline(cin, line);

        if (line == "POST" || line == "post" || line == "p") {
            break;
        }

        if (!message.empty()) {
            message += "\n";
        }
        message += line;
    }

    return message;
}