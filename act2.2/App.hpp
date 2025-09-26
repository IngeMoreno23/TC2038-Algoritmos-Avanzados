#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "HuffmanTree.hpp"

class IMenuStrategy;

class App {
private:
    std::string inTXT;
    std::string outTXT;
    std::string inBIN;
    std::string outBIN;

    std::vector<IMenuStrategy*> strategies;
    HuffmanTree* currentTree;

    void createEmptyFileIfNotExist(const std::string& path, bool binary = false);

public:
    App();
    ~App();

    void setPathInTXT(const std::string& path);
    void setPathOutTXT(const std::string& path);
    void setPathInBIN(const std::string& path);
    void setPathOutBIN(const std::string& path);

    const std::string& getPathInTXT();
    const std::string& getPathOutTXT();
    const std::string& getPathInBIN();
    const std::string& getPathOutBIN();

    void createTree(std::string text);
    void setTree(HuffmanTree* tree);

    HuffmanTree* getCurrentTree();
    void run();
    void addStrategy(IMenuStrategy* strategy);  // Add strategy to vector
};