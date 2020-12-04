#include "trie.hpp"

Trie::Trie(){
    tNode* root = new tNode();
};

struct tNode* getNode(void){
    tNode* ret = new tNode;
    ret->bIsEndpoint = false;

    for(int i = 0; i < 26; i++){
        ret->children[i] = NULL;
    }

    return ret;
}

void Trie::addWord(std::string* word){
    tNode* temp = root; // todo sketchy

    for(int i = 0; i < word->length(); i++){
        if(root->children[word->at(i)] != nullptr){
            root = root->children[word->at(i)];
        } else {
            tNode* newChild = new tNode();
            root->children[word->at(i)] = (tNode *)newChild;
        }
    }
};

float Trie::findWord(std::string* word){

};