#include "header.h"
#include<bits/stdc++.h>

using namespace std;

void history_func(deque<string> dq,bool print_history,int count=10){
    ofstream f;

    // Open the file in write mode
    f.open("history.txt");

    // Check if the file was opened successfully
    if (!f) {
        cerr << "Error: Unable to open file for writing!\n";
        return;
    }

    int i = 0, c = count;

    for(auto cmd:dq){
        f << cmd << '\n';
        if(20-i<=count && print_history) cout << '\n' << cmd;
        i++;
    }

    cout << '\n';

    f.close();
}