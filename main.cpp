#include<unistd.h>
#include<stdlib.h>
#include <cstdlib> // to get environmental variables
#include<bits/stdc++.h>
#include "header.h"
#include <sys/wait.h>

using namespace std;

int pid;

// void handleSIGTSTP(int signum) {
//     // Custom operation on Ctrl+Z

//     kill(pid, signum);

//     cout << '\n';
    
//     display_func();
//     kill(pid, SIGTSTP);
// }

int main(int argc, char *argv[]){

    // struct sigaction sa_tstp;
    // memset(&sa_tstp, 0, sizeof(sa_tstp));
    // sa_tstp.sa_handler = handleSIGTSTP;
    // signal(SIGTSTP, handleSIGTSTP);


    display_func();

    return 0;
}
