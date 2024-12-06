#include "PostHandler.h"

PostHandler::PostHandler(const std::string& dataFile)
    : dataFile(dataFile) {}

bool PostHandler::loadPosts() {
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
        if (!std::getline(iss, post.author, '|')) continue;
        if (!std::getline(iss, post.topic, '|')) continue;
        if (!std::getline(iss, post.content, '|')) continue;
        if (!std::getline(iss, post.timestamp, '|')) continue;

        posts.push_back(post);
    }

    infile.close();
    std::cout << "Loaded " << posts.size() << " posts from file." << std::endl;
    return true;
}

bool PostHandler::savePosts() {
    std::ofstream outfile(dataFile, std::ofstream::trunc);
    if (!outfile) {
        std::cerr << "Error: Could not open file for saving: " << dataFile << std::endl;
        return false;
    }

    for (const auto& post : posts) {
        outfile << post.author << "|"
                << post.topic << "|"
                << post.content << "|"
                << post.timestamp << "|\n";
    }

    outfile.close();
    std::cout << "Saved " << posts.size() << " posts to file." << std::endl;
    return true;
}

std::vector<Post> PostHandler::GetAllPosts() const {
    std::vector<Post> result;
    for (const auto& p : posts) {
        result.push_back(p);
    }
    return result;
}

std::vector<Post> PostHandler::GetPostsByTopic(const std::string& topic) const {
    std::vector<Post> result;
    for (const auto& p : posts) {
        if (p.topic == topic) {
            result.push_back(p);
        }
    }
    return result;
}

std::vector<Post> PostHandler::GetPostsByAuthor(const std::string& author) const {
    std::vector<Post> result;
    for (const auto& p : posts) {
        if (p.author == author) {
            result.push_back(p);
        }
    }
    return result;
}

bool PostHandler::AddPost(const Post& post) {
    posts.push_back(post);
    // Immediately save to persist changes
    return savePosts();
}