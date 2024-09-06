#include "header.h"
#include<bits/stdc++.h>
#include <cstdlib>
#include <sys/wait.h>
#include <sys/types.h>  
#include <unistd.h> 
#include <fcntl.h>
#include<sys/stat.h>

using namespace std;

bool fileExists(const string& filename) {
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}

inline vector<string> split_string_multiple_del(const string& str, const vector<string>& delimiters) {
    vector<string> tokens;
    size_t start = 0;
    size_t end = 0;

    while (start < str.length()) {
        end = string::npos;
        
        for (const string& delim : delimiters) {
            size_t pos = str.find(delim, start);
            if (pos != string::npos && (end == string::npos || pos < end)) {
                end = pos;  
            }
        }

        if (end == string::npos) {
            tokens.push_back(str.substr(start));
            break;
        }

        if (end != start) {  
            tokens.push_back(str.substr(start, end - start));
        }

        for (const string& delim : delimiters) {
            if (str.compare(end, delim.size(), delim) == 0) {
                start = end + delim.size();
                break;
            }
        }
    }

    return tokens;
}

inline string trim(const string str) {
    auto start = str.find_first_not_of(' ');
    auto end = str.find_last_not_of(' ');
    return str.substr(start, end - start + 1);
}

void io_redirection(string com,int input_fd, int output_fd) {
    com = trim(com);

    vector<string> delimiters = {"<", ">", ">>"};
    vector<string> comm_arr = split_string_multiple_del(com, delimiters);

    int n = com.size();
    vector<string> delim;

    for (int i = 0; i < n; i++) {
        if (i + 1 < n && com[i] == '>' && com[i + 1] == '>') {
            delim.push_back(">>");
            i++;
        } else if (com[i] == '<') {
            delim.push_back("<");
        } else if (com[i] == '>') {
            delim.push_back(">");
        }
    }

    for(int j=0;j<delim.size();j++){
        if(delim[j]==">" || delim[j]==">>"){
            if (!fileExists(comm_arr[j+1])) {
                ofstream outfile(comm_arr[j+1]);
                if (outfile.is_open()) {
                    outfile.close();
                } else {
                    cerr << "Failed to create the file: " << comm_arr[j+1] << endl;
                }
            } else {
                cout << "File already exists: " << comm_arr[j+1] << endl;
            }
        }
    }

    string inputFile, outputFile;

    string write_or_app;

    for (int i = 0; i < delim.size(); i++) {
        if (delim[i] == "<") {
            inputFile = trim(comm_arr[i + 1]);
        } else {
            write_or_app = delim[i];
            outputFile = trim(comm_arr[i + 1]);
        }
    }

    int src_fd = input_fd, dest_fd = output_fd;

    if (!inputFile.empty()) {
        src_fd = open(inputFile.c_str(), O_RDONLY);
        if (src_fd < 0) {
            perror("open input file");
            return;
        }
        
    }

    if(!outputFile.empty()){
        if (write_or_app == ">") {
            dest_fd = open(outputFile.c_str(), O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
        } else if (write_or_app == ">>") {
            dest_fd = open(outputFile.c_str(), O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
        }
        if (dest_fd < 0) {
            perror("open output file");
            return;
        }
    }

    //Tokenization
    vector<string> tokens = string_tokenizer(comm_arr[0], " ");
    char **args = new char*[tokens.size() + 1];

    for (size_t i = 0; i < tokens.size(); ++i) {
        args[i] = strdup(tokens[i].c_str());
    }
    args[tokens.size()] = nullptr;

    pid_t pid = fork();
    int status;

    if (pid < 0) {
        cerr << "Fork error\n";
        exit(EXIT_FAILURE);
    } else if (pid == 0) {

        if (dup2(src_fd, STDIN_FILENO) < 0) {
            perror("dup2 input file");
            close(src_fd);
            return;
        }

        if (dup2(dest_fd, STDOUT_FILENO) < 0) {
            perror("dup2 output file");
            close(dest_fd);
            return;
        }


        if (execvp(args[0], args) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else {
        pid_t wpid;
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    for (size_t i = 0; i < tokens.size(); ++i) {
        free(args[i]);
    }
    delete[] args;
}


void io_piping(string comm){
    string com = trim(comm);

    vector<string> comm_arr = string_tokenizer(com,"|");

    int comm_len = comm_arr.size();

    vector<string> delimiters = {"<", ">", ">>"};

    string tmp = "";

    for(int x=0;x<comm_len;x++){
        if(x>0) tmp+=" | ";

        vector<string> del_arr = split_string_multiple_del(comm_arr[x], delimiters);

        int n = comm_arr[x].size();

        vector<string> delim;

        for (int i = 0; i < n; i++) {
            if (i + 1 < n && comm_arr[x][i] == '>' && comm_arr[x][i + 1] == '>') {
                delim.push_back(">>");
                i++;
            } else if (comm_arr[x][i] == '<') {
                delim.push_back("<");
            } else if (comm_arr[x][i] == '>') {
                delim.push_back(">");
            }
        }

        for(int i=0;i<del_arr.size();i++){
            if(i==0) tmp+=del_arr[i];
            else{
                tmp = tmp + " " + delim[i-1] + " ";
                vector<string> space_arr = string_tokenizer(del_arr[i]," ");

                for(int j=0;j<space_arr.size();j++){
                    if(j==0) tmp+=space_arr[j];
                    else{
                        tmp = tmp + " | " + space_arr[j];
                    }
                }
            }
        } 
    }

    vector<string> final_arr = string_tokenizer(tmp,"|");

    int final_len = final_arr.size();

    int **pipes = (int**)malloc((final_len - 1) * sizeof(int *));

    for (int i = 0; i < final_len - 1; i++) {
        pipes[i] = (int *)malloc(2 * sizeof(int)); 
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            return;
        }
    }

    for (int i = 0; i < final_len - 1; i++) {
        int input_fd = (i == 0) ? STDIN_FILENO : pipes[i-1][0];
        int output_fd = pipes[i][1];

        io_redirection(trim(final_arr[i]), input_fd, output_fd);

        close(output_fd); // We are done writing to this pipe
    }

    int input_fd = (final_len > 1) ? pipes[final_len-2][0] : STDIN_FILENO;
    int output_fd = STDOUT_FILENO;

    io_redirection(trim(final_arr[final_len-1]), input_fd, output_fd);

     for (int i = 0; i < final_len - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
        free(pipes[i]);
    }
    free(pipes);
}