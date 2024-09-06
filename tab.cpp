#include "header.h"
#include <iostream>
#include <sys/wait.h>
#include <sys/stat.h>
#include<bits/stdc++.h>

using namespace std;

inline string trim(const string str) {
    auto start = str.find_first_not_of(" \t");
    if (start == string::npos) return "";

    auto end = str.find_last_not_of(" \t");

    return str.substr(start, end - start + 1);
}

void tab_func(string com, bool space, bool double_tab) {
    cout << '\n';

    string comd = trim(com);


    vector<string> comm_arr;
    if (space) {
        comm_arr = string_tokenizer(comd, " ");
    }

    int n = comm_arr.size();

    string comm;

    if(space){
        if(double_tab) comm = "ls | grep ^" + comm_arr[n-1];
        else comm = "ls | grep ^" + comm_arr[n-1];
    }
    else{
        if(double_tab) comm = "ls /usr/bin | grep ^" + comd;
        else comm = "ls /usr/bin | grep ^" + comd;
    }



    pid_t pid;
    if ((pid = fork()) == -1) {
        cerr << "Failed to fork process: " << strerror(errno) << endl;
        return;
    }

    if (pid == 0) {
        // Child process
        io_piping(comm);
        exit(0);
    } else {
        // Parent process
        int status;
        if (waitpid(pid, &status, 2) == -1) {
            cerr << "Error waiting for child process: " << strerror(errno) << endl;
        }
    }
}