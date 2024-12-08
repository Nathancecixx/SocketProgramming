#include "PostHandler.h"

#include <cstring>

PostHandler::PostHandler(const std::string& dataFile)
    : dataFile(dataFile) {}

bool PostHandler::loadPosts() {
    std::lock_guard<std::mutex> lock(postsMutex);
    std::ifstream infile(dataFile);
    if (!infile) {
        std::cout << "No existing posts to load from " << dataFile << "." << std::endl;
        return false;
    }

    posts.clear();

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        Post post;
        std::string tmp;
        if (!std::getline(iss, tmp, '|')) continue;
        if (!std::getline(iss, post.timestamp, '|')) continue;
        if (!std::getline(iss, post.author, ' ')) continue;
        if (!std::getline(iss, post.topic, ' ')) continue;
        if (!std::getline(iss, post.content, '\n')) continue;

        posts.push_back(post);
    }

    infile.close();
    std::cout << "Loaded " << posts.size() << " posts from file." << std::endl;
    return true;
}

bool PostHandler::savePosts() {
    std::lock_guard<std::mutex> lock(postsMutex);
    std::cout << "Start of saving post" << std::endl;
    std::ofstream outfile(dataFile, std::ofstream::trunc);
    if (!outfile) {
        std::cerr << "Error: Could not open file for saving: " << dataFile << std::endl;
        return false;
    }

    for (const auto& post : posts) {
        outfile << "|" << post.timestamp << "|"
                << post.author << " "
                << post.topic << " "
                << post.content << "\n";
    }

    std::cout << "Made it to post writing" << std::endl;

    outfile.close();
    std::cout << "Saved " << posts.size() << " posts to file." << std::endl;
    return true;
}

std::vector<Post> PostHandler::GetAllPosts() const {
    std::lock_guard<std::mutex> lock(postsMutex);
    std::vector<Post> result;
    for (const auto& p : posts) {
        result.push_back(p);
    }
    return result;
}

std::vector<Post> PostHandler::GetPostsByTopic(const std::string& topic) const {
    std::lock_guard<std::mutex> lock(postsMutex);
    std::vector<Post> result;
    for (const auto& p : posts) {
        if (p.topic == topic) {
            result.push_back(p);
        }
    }
    return result;
}

std::vector<Post> PostHandler::GetPostsByAuthor(const std::string& author) const {
    std::lock_guard<std::mutex> lock(postsMutex);
    std::vector<Post> result;
    for (const auto& p : posts) {
        if (p.author == author) {
            result.push_back(p);
        }
    }
    return result;
}

bool PostHandler::AddPost(const Post& post) {
    std::lock_guard<std::mutex> lock(postsMutex);
    posts.push_back(post);
    return true;
}
