//
// Created by zengt on 12/3/2024.
//

#include "hashTable.h"
void hashTable::insert_word(string word)
{
    words_map[word] = true;
}

bool hashTable::search(string word)
{
    return words_map.count(word);
}


vector<string> hashTable::same_prefix(string prefix)
{
    vector<string> words_contain_prefix;
    for(auto pair: words_map)
    {
        //if the key starts with the prefix, add it to vector
        if(pair.first.find(prefix) == 0)
        {
            words_contain_prefix.push_back(pair.first);
        }
    }
        return words_contain_prefix;
}


