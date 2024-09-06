#include "header.h"
#include<bits/stdc++.h>

using namespace std;

vector<string> string_tokenizer(string str, const char *delim){
    vector<string> comm;

    //Convert string to character array
    char* commands = new char[str.length() + 1];

    strcpy(commands, str.c_str());

    // Tokenize the string
    char* token = strtok(commands, delim);

    while (token != nullptr) {
        
        comm.push_back(token);
        
        token = strtok(nullptr, delim);
    }

    return comm;
}