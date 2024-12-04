#ifndef PROJECT_3_TRIE_H
#define PROJECT_3_TRIE_H

#include <iostream>
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include <queue>
#include <sstream>
#include <chrono>
#include "pugixml-master/src/pugixml.hpp"
using namespace std;

class TrieNode{
public:
    unordered_map<char, TrieNode*> children;
    bool end_of_word;

    TrieNode()
    {
        end_of_word=false;
    }
};

class Trie {
private:
    TrieNode* root;
    unordered_map<string,bool> cache;

public:
    Trie();
    ~Trie();
    void deleteTrie(TrieNode* node);
    void insert(const string& word);
    bool search(const string& word);
    bool starts_with(const string& word);
    void find_words_with_prefix(const string& word);
};


#endif //PROJECT_3_TRIE_H
