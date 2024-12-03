#include <iostream>
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include <queue>
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

class Trie
{
    TrieNode* root;
    unordered_map<string,bool> cache;

public:
    Trie() {
        root = new TrieNode();
    }

    ~Trie()
    {
        deleteTrie(root);
    }

    void deleteTrie(TrieNode* node)
    {
        for(auto children:node->children)
        {
            deleteTrie(children.second);
        }
        delete node;
    }

    void insert(const string& word)
    {
        TrieNode* currentNode = root;
        for(char character:word)
        {
            if(currentNode->children.find(character) == currentNode->children.end())
            {
                currentNode->children[character] = new TrieNode();
            }
            currentNode = currentNode -> children[character];
        }
        currentNode->end_of_word = true;
    }

    bool search(const string& word)
    {
        if(cache.find(word)!=cache.end())
        {
            return cache[word];
        }

        TrieNode* currentNode = root;
        for(char character:word)
        {
            if(currentNode -> children.find(character) == currentNode->children.end()) {
                cache[word] = false;
                return false;
            }
            currentNode = currentNode->children[character];
        }

        cache[word] = currentNode->end_of_word;
        return currentNode->end_of_word;
    }

    bool starts_with(const string& word)
    {
        TrieNode* currentNode = root;
        for(char character : word)
        {
            if(currentNode -> children.find(character) == currentNode->children.end())
            {
                return false;
            }
            currentNode = currentNode->children[character];
        }
        return true;
    }

    void find_words_with_prefix(const string& word)
    {
        TrieNode* currentNode = root;
        for(char character : word)
        {
            if(currentNode -> children.find(character) == currentNode ->children.end())
            {
                return;
            }
            currentNode = currentNode->children[character];
        }

        vector<string> words;
        queue<pair<TrieNode*, string>>q;
        q.push({currentNode,word});

        while(!q.empty())
        {
            auto[node,current_word] = q.front();
            q.pop();

            if (node->end_of_word) {
                words.push_back(current_word);
            }

            for (auto& pair : node->children) {
                q.push({pair.second, current_word + pair.first});
            }
        }

        for (const string& word : words) {
            cout << word << endl;
        }

        }

    };

void parseWikipediaDump(const string& filename, Trie& trie)
{
    //reference to pugixml library
    pugi::xml_document doc;
    pugi::xml_parse_result result= doc.load_file(filename.c_str());
    if (!result)
    {
        cout << "XML file could not be loaded!"<< endl;
        return;
    }

    //reference to https://pugixml.org/docs/quickstart.html
    for(pugi::xml_node page : doc.children("page"))
    {
        pugi::xml_node title_node = page.child("title");
    }


}

int main() {

return 0;
}