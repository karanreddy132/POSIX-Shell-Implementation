#include "header.h"
#include<iostream>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include <cstdlib> // to get environmental variables
#include<limits.h>
#include<bits/stdc++.h>
#include <sys/wait.h>
#include <termios.h>

const char* username = getenv("USER"); //get username
char systemname[HOST_NAME_MAX];   //get hostname
char cwd[PATH_MAX]; //get current directory

// Removes spaces from start and end of the commands
string trim(const string str) {
    auto start = str.find_first_not_of(' ');
    auto end = str.find_last_not_of(' ');
    return str.substr(start, end - start + 1);
}

vector<string> splitString(const string& str) {
    
    vector<string> tokens;
    istringstream iss(str);
    string token;
    
    // Split by spaces (skips any number of spaces)
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}

int fgPID=0;

void sigintHandler(int signum){
    if(fgPID!=0){
        cout << "control C -Terminated\n";
        kill(fgPID,SIGINT);
        fgPID=0;
    }
}

void sigstpHandler(int signum){

    if(fgPID!=0){
        cout << "control Z -Stopped\n";

        int pid = fork();

        if(pid<0){
            cerr << "Fork error\n";
            return;
        }
        else if(pid==0){
            pid_t pgid = getpid(); 

            if (setpgid(0, 0) == -1) {
                perror("Error in setting child process group ID to its own PID");
                exit(EXIT_FAILURE);
            }

        }
        fgPID=0;
    }

}

void setRawMode(bool enable) {
    static struct termios oldt, newt;
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldt);  // Get current terminal attributes
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);  // Apply new settings immediately
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  // Restore old settings
    }
}

void handleCtrlD(int signum) {
    setRawMode(false);  // Restore terminal settings
    exit(0);  // Exit the shell
}


string pth = "", prev_pth = "", initial_path = "";

void display_func(){
    deque<string> dq;

    signal(SIGINT,sigintHandler);
    signal(SIGTSTP,sigstpHandler);

    ifstream infile("history.txt");  // Open the file

    if (infile){
        string line;
    
    // Read the file line by line
        while (getline(infile, line)) {
            dq.push_back(line);  
        }

        // Close the file
        infile.close();
    }

    if (gethostname(systemname, sizeof(systemname)) != 0) {
        cerr << "Unable to get the system name (hostname)." << endl;
    } 

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        cerr << "Unable to get the current working directory." << endl;
    }

    initial_path = cwd;
    pth = initial_path;

    while(true){
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            cerr << "Unable to get the current working directory." << endl;
        }

        string temp;

        if(initial_path.size()>pth.size()) temp = pth;
        else{
            temp = "~" + pth.substr(initial_path.size());
        }

        cout << '\n';

        ostringstream oss;

        oss << "\033[34m" << username << "@" << systemname << ":" << temp << "_>" << "\033[0m";
        

    // Store the result in a string
        string prompt = oss.str();

        cout << prompt;
        cout.flush();

        string input_command;

        int ch;
        int prev_ch = 0;   

        bool flag = false, prev_key = true, double_tab = false;

        deque<string>::reverse_iterator rit = dq.rbegin();
        deque<string>::iterator it = dq.begin();

        bool space = false;

        setRawMode(true);  

        while (true) {

            ch = getchar();

            if(ch == 3){
                raise(SIGINT);
            }

            if(ch=='\n') break;
            else if(ch == 26){  // Control Z
                raise(SIGTSTP);
            }
            else if (ch == 4) {
                cout << '\n';
                handleCtrlD(0);  // Handle Ctrl+D (EOF)
            } else if (ch == 127) {  
                if (!input_command.empty()) {  
                    input_command.pop_back();
                    cout << "\b \b";  
                    cout.flush(); 
                }
            } 
            else if (input_command!="" && ch == '\t') { //Condtion for tab
                flag = true;
                if(prev_ch=='\t'){ 
                    string s;
                    if(space){
                        vector<string> com_arr = string_tokenizer(input_command," ");

                        s = com_arr[1];

                        int n = s.size();
                        while(s[n-1]!='.'){
                            s.pop_back();
                            n = s.size();
                        }
                    }
                    
                    tab_func(s,space,true);
                    break; 
                }
                else{ 
                    FILE* file = freopen("tab_output.txt", "w", stdout);

                    if (file == nullptr) {
                        cerr << "Error opening file for writing!" << endl;
                        return;
                    }
                    tab_func(input_command,space,false);

                    // Pushing stdout to a file and then reding it
                    freopen("/dev/tty", "a", stdout);

                    ifstream inputFile("tab_output.txt");

                    vector<string> com_arr;

                    if(space){
                        com_arr = string_tokenizer(input_command," ");
                    }

                    string line;
                    if(getline(inputFile, line)) {
                        line = trim(line);
                        if(space) input_command = com_arr[0] + " " + line;
                        else input_command = line;
                    }

                    cout << "\r\033[K" << prompt << input_command;
                    cout.flush();

                    inputFile.close();

                    remove("tab_output.txt");
                } 
            }
            else if (ch == 27) {  // Escape character
                ch = getchar();
                if (ch == '[') {
                    ch = getchar();
                    if (ch == 'A') {  // Up arrow
                        if (rit != dq.rend()) {
                            if(!prev_key){
                                rit++;
                                rit++;
                            }
                            input_command = *rit;
                            rit++;
                            
                            cout << "\r\033[K" << prompt << input_command;
                            cout.flush();
                            prev_key = true;
                        }
                    }
                    else if (ch == 'B') {  // Down arrow
                        if (rit!= dq.rend()) {
                            if(prev_key){
                                rit--;
                                rit--;
                                prev_key = false;
                            }
                            input_command = *rit;
                            rit--;
                            
                            cout << "\r\033[K" << prompt << input_command;
                            cout.flush();
                        }
                    }
                }
            }
            else{

                char c = static_cast<char>(ch); 

                if(input_command=="" || c!='\t'){
                    cout << static_cast<char>(ch);
                    cout.flush();  
                } 

                if(input_command=="" || c!='\t') input_command += c;
                if(c==' ') space = true;
            }
        
            
            prev_ch = ch;
        }

        setRawMode(false);

        if(flag) continue;

        if(dq.size()<20){
            if(dq.empty() || dq.back()!=input_command) dq.push_back(input_command);
        }
        else{
            if(dq.back()!=input_command){
                dq.pop_front();
                dq.push_back(input_command);
            }
        }

        history_func(dq,false,0);

        if(input_command=="exit"){
            cout << '\n';
            exit(0);
        }


        // Tokenize the input commands
        vector<string> comm = string_tokenizer(input_command, ";");

        int comm_len = comm.size();

        // Executing each command
        for(int i=0;i<comm_len;i++){

            comm[i] = trim(comm[i]);  //removing extra spaces
            
            // Removing extra spaces
            vector<string> comm_arr = string_tokenizer(comm[i]," ");

            int n = comm_arr.size();

            if(n == 0) break;
            else if(comm_arr[n-1]=="&"){
                bg_fg_func(comm_arr[i]);
            }
            else if(input_command.find("|")!=string::npos || input_command.find("<")!=string::npos || input_command.find(">")!=string::npos || input_command.find(">>")!=string::npos){
                // For io redirection and piping
                io_piping(input_command);
            }
            else if(comm_arr[0]=="cd"){
                cd_func(pth,comm[i],prev_pth,initial_path);
            }
            else if(comm_arr[0]=="pwd"){
                cwd_func(cwd);
            }
            else if(comm_arr[0]=="echo"){
                echo_func(comm[i]);
            }
            else if(comm_arr[0]=="ls"){
                ls_func(pth,comm[i],initial_path);
            }
            else if(comm_arr[0]=="history"){
                int count = 10;

                if(n==2){
                    stringstream ss(comm_arr[1]);

                    ss >> count;
                }
                history_func(dq,true,count);
            }
            else if(comm_arr[0]=="search"){
                if(n==2){
                    bool ans = search_func(comm_arr[1],pth);

                    if(ans) cout << "true\n";
                    else cout << "false\n";
                }
                else{
                    cerr << "Invalid command\n";
                }
            }
            else if(comm_arr[0]=="pinfo"){
                if(n==1){
                    int pid = getpid();
                    pinfo_func(pid);
                }
                if(n==2){
                    int pid = stoi(comm_arr[1]);

                    pinfo_func(pid);    
                }
            }
            else{
                vector<string> tokens = string_tokenizer(input_command, " ");
                char **args = new char*[tokens.size() + 1];

                for (size_t i = 0; i < tokens.size(); ++i) {
                    args[i] = strdup(tokens[i].c_str());
                }
                args[tokens.size()] = nullptr;

                pid_t pid = fork();
                fgPID=pid;

                if(pid<0){
                    cerr << "Fork Error\n";
                    return;
                }
                else if(pid == 0){
                    if (execvp(args[0], args) == -1) {
                        perror("execvp");
                        exit(EXIT_FAILURE);
                    }
                }
                else{
                    wait(nullptr);
                }
            }
        }
        
    }
}