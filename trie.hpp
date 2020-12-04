#pragma once
#include <vector>
#include <string>

class Trie{
    struct tNode{
        long lFreq;
        bool bIsEndpoint;
        struct tNode* children[26];
    };

    tNode* root;
    tNode* getNewNode();
    public:
    Trie();
    void addWord(std::string* word);
    float findWord(std::string* word);
};