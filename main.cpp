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

void parseWikipediaDumpIntoTrie(const string& filename, Trie& trie)
{
    //reference to pugixml library
    pugi::xml_document doc;
    pugi::xml_parse_result result= doc.load_file(filename.c_str());
    if (!result)
    {
        cout << "XML file could not be loaded!"<< endl;
        cout << "Error: " << result.description() << endl;
        return;
    }

    //reference to https://pugixml.org/docs/quickstart.html
    pugi::xml_node feedNode = doc.child("feed");
    if(feedNode)
    {
        for(pugi::xml_node docNode : feedNode.children("doc"))
        {
            pugi::xml_node titleNode = docNode.child("title");
            pugi::xml_node abstractNode = docNode.child("abstract");
            if(titleNode && abstractNode)
            {
                string text = abstractNode.text().as_string();
                stringstream ss(text);
                string word;

                while(ss>>word)
                {
                    for(int i = 0; i<word.size();++i)
                    {
                        if(!isalpha(word[i]))
                        {
                            word.erase(i,1);
                            --i;
                        }
                    }

                    if(!word.empty())
                    {
                        trie.insert(word);
                    }
                }
            }
        }
    }
}

void parseWikipediaDumpIntoHashMap(const string& filename,)
{
    //reference to pugixml library
    pugi::xml_document doc;
    pugi::xml_parse_result result= doc.load_file(filename.c_str());
    if (!result)
    {
        cout << "XML file could not be loaded!"<< endl;
        cout << "Error: " << result.description() << endl;
        return;
    }

    //reference to https://pugixml.org/docs/quickstart.html
    pugi::xml_node feedNode = doc.child("feed");
    if(feedNode)
    {
        for(pugi::xml_node docNode : feedNode.children("doc"))
        {
            pugi::xml_node titleNode = docNode.child("title");
            pugi::xml_node abstractNode = docNode.child("abstract");
            if(titleNode && abstractNode)
            {
                string text = abstractNode.text().as_string();
                stringstream ss(text);
                string word;

                while(ss>>word)
                {
                    for(int i = 0; i<word.size();++i)
                    {
                        if(!isalpha(word[i]))
                        {
                            word.erase(i,1);
                            --i;
                        }
                    }

                    if(!word.empty())
                    {
                        trie.insert(word);
                    }
                }
            }
        }
    }
}


class HashTableImplementation{
    unordered_map<string, bool> words_map;
public:
    //add the word into the hashtable and mark it true
    //true means the word exists in the table
    void insert_word(string word){
        words_map[word] = true;
    }

    //if the word exist, return 1 since the key is unique in unordered map
    // if doesnet then return 0
    bool search(string word){
        return words_map.count(word);
    }


    vector<string>same_prefix(string prefix){
        vector<string> words_contain_prefix;
        for(auto pair: words_map){
            //if the key starts with the prefix, add it to vector
            if(pair.first.find(prefix) == 0){
                words_contain_prefix.push_back(pair.first);
            }
        }
        return words_contain_prefix;
    }
};
int main() {
    Trie trie;
    HashTableImplementation hashTable;
    int option;
    string input;

    cout << "Welcome to the Hash Table and Trie Search Engine!" << endl;
    bool control = true;

    while (control) {
        cout << "\nChoose an option:" << endl;
        cout << "1. Insert a word into both the Trie and Hash Table" << endl;
        cout << "2. Search for a word in both the Trie and Hash Table" << endl;
        cout << "3. Find all words with a prefix in the Hash Table" << endl;
        cout << "4. Compare search times for Trie and Hash Table" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your option: ";
        cin >> option;

        if (option == 1) {
            cout << "Enter the word you want to insert: ";
            cin >> input;
            trie.insert(input);
            hashTable.insert_word(input);
            cout << input << " inserted successfully into both the Trie and Hash Table!" << endl;

        } else if (option == 2) {
            cout << "Enter the word you want to search for: ";
            cin >> input;


            auto start_trie = chrono::high_resolution_clock::now();
            bool trie_result = trie.search(input);
            auto end_trie = chrono::high_resolution_clock::now();
            auto duration_trie = chrono::duration_cast<chrono::nanoseconds>(end_trie - start_trie);


            auto start_hash = chrono::high_resolution_clock::now();
            bool hash_result = hashTable.search(input);
            auto end_hash = chrono::high_resolution_clock::now();
            auto duration_hash = chrono::duration_cast<chrono::nanoseconds>(end_hash - start_hash);


            cout << "\nSearch Results:" << endl;
            cout << "Trie: " << (trie_result ? "Found" : "Not Found") << " in " << duration_trie.count() << " nanoseconds." << endl;
            cout << "Hash Table: " << (hash_result ? "Found" : "Not Found") << " in " << duration_hash.count() << " nanoseconds." << endl;

        } else if (option == 3) {
            cout << "Enter the prefix of the words you want to get: ";
            cin >> input;
            cout << "Words starting with: " << input << endl;
            vector<string> results = hashTable.same_prefix(input);
            if (results.empty()) {
                cout << "No words found with the prefix '" << input << "'." << endl;
            } else {
                for (const string& word : results) {
                    cout << word << endl;
                }
            }

        } else if (option == 4) {
            cout << "Enter the word to compare search times: ";
            cin >> input;

            auto start_trie = chrono::high_resolution_clock::now();
            bool trie_result = trie.search(input);
            auto end_trie = chrono::high_resolution_clock::now();
            auto duration_trie = chrono::duration_cast<chrono::nanoseconds>(end_trie - start_trie);


            auto start_hash = chrono::high_resolution_clock::now();
            bool hash_result = hashTable.search(input);
            auto end_hash = chrono::high_resolution_clock::now();
            auto duration_hash = chrono::duration_cast<chrono::nanoseconds>(end_hash - start_hash);


            cout << "\nPerformance Comparison:" << endl;
            cout << "Trie: " << (trie_result ? "Found" : "Not Found") << " in " << duration_trie.count() << " nanoseconds." << endl;
            cout << "Hash Table: " << (hash_result ? "Found" : "Not Found") << " in " << duration_hash.count() << " nanoseconds." << endl;


            if (duration_trie.count() > 0) {
                double speedup = static_cast<double>(duration_hash.count()) / duration_trie.count();
                cout << "Speedup (Hash Table / Trie): " << speedup << "x" << endl;
            } else {
                cout << "Trie search time is too fast to measure speedup accurately." << endl;
            }

        } else if (option == 5) {
            control = false;

        } else {
            cout << "Invalid option, please provide a valid instruction." << endl;
        }
    }

    return 0;
}
