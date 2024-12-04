#include <iostream>
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include <queue>
#include <sstream>
#include <chrono>
#include "pugixml-master/src/pugixml.hpp"
#include "Trie.h"
#include "hashTable.h"
using namespace std;

bool parseWikipediaDumpIntoTrie(const string& filename, Trie& trie)
{
    //reference to pugixml library
    pugi::xml_document doc;
    pugi::xml_parse_result result= doc.load_file(filename.c_str());
    if (!result)
    {
        cout << "XML file could not be loaded!"<< endl;
        cout << "Error: " << result.description() << endl;
        return false;
    }
    else
    {
        cout<<"inserted successfully into the Trie!"<<endl;
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
        return true;
    }
}

bool parseWikipediaDumpIntoHashMap(const string& filename,hashTable& hashtable)
{
    //reference to pugixml library
    pugi::xml_document doc;
    pugi::xml_parse_result result= doc.load_file(filename.c_str());
    if (!result)
    {
        cout << "XML file could not be loaded!"<< endl;
        cout << "Error: " << result.description() << endl;
        return false;
    }
    else
    {
        cout << "inserted successfully into the Hash Table!" << endl;
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
                        hashtable.insert_word(word);
                    }
                }
            }
        }
        return true;
    }
}

int main() {
    hashTable hashTable;
    Trie trie;

    int option;

    cout << "Welcome to the Hash Table and Trie Search Engine!" << endl;
    bool control = true;


        cout << "Insert a XML document into both the Trie and Hash Table and compare the insert time" << endl;
        string input;
        cout <<"Enter the path of the XML document you want to insert: ";
        cin >> input;

        auto start_trie_insert = chrono::high_resolution_clock::now();
        bool isParseTrie = parseWikipediaDumpIntoTrie(input,trie);
        auto end_trie_insert = chrono::high_resolution_clock::now();
        auto duration_trie = chrono::duration_cast<chrono::nanoseconds>(end_trie_insert - start_trie_insert);

        auto start_hash_insert = chrono::high_resolution_clock::now();
        bool isParseHashmap = parseWikipediaDumpIntoHashMap(input,hashTable);
        auto end_hash_insert = chrono::high_resolution_clock::now();
        auto duration_hash = chrono::duration_cast<chrono::nanoseconds>(end_hash_insert - start_hash_insert);

        if (isParseTrie)
        {
            cout << "XML insert to Trie: " << "in " << duration_trie.count() << " nanoseconds." << endl;
        }
        else
        {
            exit(0);
        }

        if (isParseHashmap)
        {
            cout << "XML insert to HashTable: " << "in " << duration_hash.count() << " nanoseconds." << endl;
        }
        else
        {
            exit(0);
        }

        if (isParseHashmap&&isParseTrie) {
            if(duration_trie.count()>duration_hash.count())
            {
                double speedup = static_cast<double>(duration_trie.count()) / duration_hash.count();
                cout << "Speedup insert from HashTable to Trie(Trie / HashTable): " << speedup << "x" << endl;
            }
            if(duration_trie.count()<duration_hash.count())
            {
                double speedup = static_cast<double>(duration_hash.count()) / duration_trie.count();
                cout << "Speedup insert from Trie to HashTable(Hash Table / Trie): " << speedup << "x" << endl;
            }
        }

    while (control) {
        cout << "\nChoose an option:" << endl;
        cout << "1. Search for a word in both the Trie and Hash Table and compare the exact search time" << endl;
        cout << "2. Find all words with a prefix in both the Trie and Hash Table and compare the prefix search time" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your option: ";
        cin >> option;
           if (option == 1) {
            string input;
            cout << "Enter the word you want to search for:";
            cin >> input;
            cout<<input<<endl;

            auto start_trie = chrono::high_resolution_clock::now();
            bool trie_result = trie.search(input);
            auto end_trie = chrono::high_resolution_clock::now();
            auto duration_trie = chrono::duration_cast<chrono::nanoseconds>(end_trie - start_trie);


            auto start_hash = chrono::high_resolution_clock::now();
            bool hash_result = hashTable.search(input);
            auto end_hash = chrono::high_resolution_clock::now();
            auto duration_hash = chrono::duration_cast<chrono::nanoseconds>(end_hash - start_hash);


            cout << "\nSearch Results:" << endl;

            if (trie_result)
            {
                cout << "Trie: " << "Found "<< "in " << duration_trie.count() << " nanoseconds." << endl;
            }
            else
            {
                cout << "Trie: " << "Not Found "<< endl;
            }

            if (hash_result)
            {
                cout << "HashTable: " << "Found "<< "in " << duration_trie.count() << " nanoseconds." << endl;
            }
            else
            {
                cout << "HashTable: " << "Not Found "<< endl;
            }

            if(hash_result&&trie_result) {
                if (duration_trie.count() > 0) {
                    if (duration_trie.count() < duration_hash.count()) {
                        double speedup = static_cast<double>(duration_hash.count()) / duration_trie.count();
                        cout << "Speedup Exact Search from Trie to HashTable(Hash Table / Trie): " << speedup << "x"
                             << endl;
                    }
                } else if (duration_hash.count() > 0) {
                    if (duration_trie.count() > duration_hash.count()) {
                        double speedup = static_cast<double>(duration_trie.count()) / duration_hash.count();
                        cout << "Speedup Exact Search from HashTable to Trie(Trie / HashTable): " << speedup << "x"
                             << endl;
                    }
                } else {

                    cout << "Trie search time is too fast to measure speedup accurately." << endl;
                }
            }

        } else if (option == 2) {
            string input;
            cout << "Enter the prefix of the words you want to get: ";
            cin >> input;
            cout << "Words starting with: " << input << endl;

            auto start_trie = chrono::high_resolution_clock::now();
            trie.find_words_with_prefix(input);
            auto end_trie = chrono::high_resolution_clock::now();
            auto duration_trie = chrono::duration_cast<chrono::nanoseconds>(end_trie - start_trie);

            auto start_hash = chrono::high_resolution_clock::now();
            vector<string> results = hashTable.same_prefix(input);
            if (results.empty()) {
                cout << "No words found with the prefix '" << input << "'." << endl;
            } else {
                cout<<"Hash Table Prefix Words Search:"<<endl;
                int count=0;
                for (const string& word : results) {
                    cout << word<<" ";
                    count++;
                }
                cout<<endl;
                cout<<"Word Count: "<<count<<endl;
                cout<<endl;
            }
            auto end_hash = chrono::high_resolution_clock::now();
            auto duration_hash = chrono::duration_cast<chrono::nanoseconds>(end_hash - start_hash);

            if(!results.empty())
            {
                cout << "HashTable Searching all the words in " <<duration_hash.count() << " nanoseconds." << endl;
                cout << "Trie Searching all the words in " <<duration_trie.count() << " nanoseconds." << endl;
            }


               if (duration_trie.count() > 0) {
                   if(duration_trie.count()<duration_hash.count())
                   {
                       double speedup = static_cast<double>(duration_hash.count()) / duration_trie.count();
                       cout << "Speedup Exact Search from Trie to HashTable(Hash Table / Trie): " << speedup << "x" << endl;
                   }
               }
               else if(duration_hash.count() > 0)
               {
                   if(duration_trie.count()>duration_hash.count())
                   {
                       double speedup = static_cast<double>(duration_trie.count()) / duration_hash.count();
                       cout << "Speedup Exact Search from HashTable to Trie(Trie / HashTable): " << speedup << "x" << endl;
                   }
               }
               else
               {
                   cout << "Trie search time is too fast to measure speedup accurately or there is no result" << endl;
               }



        } else if (option == 3) {
            control = false;
        } else {
            cout << "Invalid option, please provide a valid instruction." << endl;
            control=false;
        }
    }

    return 0;
}
