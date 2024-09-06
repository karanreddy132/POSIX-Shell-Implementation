#include "header.h"
#include <unistd.h>
#include <csignal>
#include <sys/wait.h>
#include<bits/stdc++.h>

using namespace std;

void bg_fg_func(string comm) {

    pid_t pid = fork();

    if (pid == -1) {
        cerr << "Failed to create child process\n";
    } 
    else if (pid == 0) {
        pid_t pgid = getpid(); 

        if (setpgid(0, 0) == -1) {
            perror("Error in setting child process group ID to its own PID");
            exit(EXIT_FAILURE);
        }

        vector<string> comds = string_tokenizer(comm, " ");
        vector<char*> args;
        for (auto& cmd : comds) {
            args.push_back(const_cast<char*>(cmd.c_str()));
        }
        args.push_back(nullptr);

        execvp(args[0], args.data());
        cerr << "Failed to execute command: " << comm << '\n';
        exit(EXIT_FAILURE);
    } 
    else {
        cout << "Child process started with PID: " << pid << '\n';

        int status;
        waitpid(pid, &status, WNOHANG);
    }
}
