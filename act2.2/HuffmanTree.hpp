#pragma once
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <fstream>
#include <cstdint>
#include <utility>

struct Node {
    char c;
    float prob;
    Node* l;
    Node* r;

    Node(char c, float prob);
    Node(Node* nodeL, Node* nodeR);
};

struct Comparador {
    bool operator()(const Node* a, const Node* b) const;
};

class HuffmanTree {
private:
    Node* root;
    std::unordered_map<char, std::string> encoderMap;
    std::unordered_map<std::string, char> decoderMap;
    std::string text;

    void printTree(Node* root, int depth = 0);
    void collectLeafNodes(Node* node, std::vector<std::pair<char, float>>& leafNodes);

public:
    HuffmanTree(const std::string& text); // From text
    HuffmanTree(const std::unordered_map<char, int>& map); // From map [char:porb]
    /*
    Payload
    [HEADER SIZE: uint32_t][HEADER DATA: (char, float)[]][TEXT SIZE: uint32_t][ENCODED DATA: bit[]]
    */
    //HuffmanTree(const std::string& filename); // Ni modo no quiero hacer un args struct
    static HuffmanTree* loadFromBin(const std::string& filename);

    void saveToBin(const std::string& filename);

    std::string encode();
    std::string decode();
    std::string encode(const std::string& text);
    std::string decode(const std::string& text);
    void printEncoder();
    void printDecoder();
    void printTable();
    void printTree();
};