#include "header.h"
#include<bits/stdc++.h>
#include<sys/stat.h>
#include <sys/types.h>

using namespace std;


bool directory_exists(const string& path) {
    struct stat file_stat;

    if (stat(path.c_str(), &file_stat) != 0) {
        return false; 
    }
    return (file_stat.st_mode & S_IFDIR) != 0; 
}

void cd_func(string &pth,string operation,string &prev_pth,string initial_path){

    vector<string> cmds = string_tokenizer(operation," "); 

    int op_size = cmds.size();

    string tmp = pth;

    // if cd has 2 or more argumrnts then it throws error
    if(op_size>2){
        cerr << "bash: cd: too many argumants\n";
        return;
    }
    else if(op_size==1){
        // Redirect to home directory if no argument is passes
        pth = initial_path;
        prev_pth = tmp;
    }
    else if (op_size==2){
        if(cmds[1]=="~"){
            // redirect to home directory 
            pth = initial_path;
            prev_pth = tmp;
        }
        else if(cmds[1]=="/"){
            pth = "/";
            prev_pth = tmp;
        }
        else if(cmds[1]=="." || cmds[1]=="./"){
            prev_pth = tmp;
        }
        else if(cmds[1]==".."){
            if(pth==""){
                prev_pth = tmp;
            }
            else{
                // Redirect to parent directory if .. is passed
                int n = pth.size();

                int i = n-1;

                while(i>=0 && pth[i]!='/'){
                    pth.pop_back();
                    i = pth.size()-1;
                }

                if(i>=0) pth.pop_back();
                prev_pth = tmp;
            }
        }
        else if(cmds[1]=="-"){
            pth = prev_pth;
            prev_pth = tmp;
            cout << pth << '\n';
        }
        else{
            string s = "";
            
            if(pth!="") s = pth;
            if(cmds[1]!="") s = s + "/" + cmds[1];
            if(directory_exists(s)){
                pth = pth + "/" + cmds[1];
                prev_pth = tmp;
            }
            else{
                cerr << "bash: cd: " << cmds[1] << " : No such file or directory";
                return;
            }
        }
    }  

    if(pth==""){
        const char* home_dir = getenv("HOME");
        if (chdir(home_dir) != 0) cerr << "Error in changing directory\n";
    }
    else if(chdir(pth.c_str())<0){
        cerr << "Error in changing directory\n";
    }
}