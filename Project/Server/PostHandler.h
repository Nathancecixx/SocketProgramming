#ifndef NETWORKINGASN3_POSTHANDLER_H
#define NETWORKINGASN3_POSTHANDLER_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>

struct Post {
    std::string author;
    std::string topic;
    std::string content;
    std::string timestamp; 
};

class PostHandler {
public:

    //C
    PostHandler(const std::string& dataFile);
    bool loadPosts();
    //R
    std::vector<Post> GetAllPosts() const;
    std::vector<Post> GetPostsByTopic(const std::string& topic) const;
    std::vector<Post> GetPostsByAuthor(const std::string& topic) const;
    //U
    bool AddPost(const Post& post);
    //D
    bool savePosts();

private:
    std::string dataFile;
    std::vector<Post> posts;
    mutable std::mutex postsMutex;
};

#endif // NETWORKINGASN3_POSTHANDLER_H

