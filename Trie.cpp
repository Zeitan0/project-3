#include "Trie.h"
    Trie::Trie() {
        root = new TrieNode();
    }

    Trie::~Trie()
    {
        deleteTrie(root);
    }

    void Trie::deleteTrie(TrieNode* node)
    {
        for(auto children:node->children)
        {
            deleteTrie(children.second);
        }
        delete node;
    }

    void Trie::insert(const string& word)
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

    bool Trie::search(const string& word)
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

    bool Trie::starts_with(const string& word)
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

    void Trie::find_words_with_prefix(const string& word)
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

        if(words.empty())
        {
            cout << "No words found with the prefix '" << word << "'." << endl;
        }
        else
        {
            int count=0;
            cout<<"Trie Prefix Words Search: "<<endl;
            for (const string& word : words) {
                cout << word<<" ";
                count++;
            }
            cout<<endl;
            cout<<"Word Count: "<<count<<endl;
            cout<<endl;
        }

    }



