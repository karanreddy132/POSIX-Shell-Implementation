#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>

using namespace std;

void pinfo_func(int pid) {
    string pp = "/proc/" + to_string(pid);

    
    ifstream status_f(pp + "/stat");
    
    if (!status_f.is_open()) {
        cerr << "Failed to open " << pp << "/stat" << endl;
        return;
    }

    //Reading the status of the file
    string status_l;
    getline(status_f, status_l);
    status_f.close();

    // Tokenize stat file line
    stringstream ss(status_l);
    string token;
    string tokens[52];

    int i = 0;

    while (ss >> token && i < 52) {
        tokens[i++] = token;
    }

    string process_status = tokens[2]; 
    pid_t pgid = stoi(tokens[4]);     
    unsigned long vir_mem_size = stoul(tokens[22]); 

    // Foreground background status
    pid_t fgid = tcgetpgrp(STDIN_FILENO); // Get foreground process group ID of terminal
    bool is_fg = (pgid == fgid);

    // Append '+' to status if in foreground
    if (is_fg) {
        process_status += "+";
    }

    // Executable path
    char exe_path[PATH_MAX];
    ssize_t len = readlink((pp + "/exe").c_str(), exe_path, sizeof(exe_path) - 1);

    if (len != -1) {
        exe_path[len] = '\0';
    } else {
        strcpy(exe_path, "Unknown");
    }

    // Print the process information
    cout << "pid -- " << pid << endl;
    cout << "Process Status -- {" << process_status << "}" << endl;
    cout << "memory -- " << vir_mem_size << " {Virtual Memory}" << endl;
    cout << "Executable Path -- " << exe_path << endl;
}