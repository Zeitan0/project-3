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

class hashTable{
private:
    unordered_map<string, bool> words_map;
public:
    void insert_word(string word);
    bool search(string word);
    vector<string> same_prefix(string prefix);
};
