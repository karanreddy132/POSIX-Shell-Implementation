#include "header.h"
#include<bits/stdc++.h>
#include<dirent.h>
#include<sys/stat.h>
#include <sys/types.h>
#include <pwd.h>   
#include <grp.h>        
#include <ctime>
#include <unistd.h>

using namespace std;


inline bool directory_exists(const string& path) {
    struct stat file_stat;

    if (stat(path.c_str(), &file_stat) != 0) {
        return false; // stat failed, file does not exist
    }

    return (file_stat.st_mode & S_IFDIR) != 0; // Check if it's a directory
}

bool file_exists(const string& path) {
    struct stat file_stat;

    if (stat(path.c_str(), &file_stat) != 0) {
        return false; // stat failed, file does not exist
    }

    return (S_ISREG(file_stat.st_mode)); // Check if it's a file
}

void ls_func(string pth,string commd,string initial_path){

    vector<string> op = string_tokenizer(commd," ");

    int op_size = op.size();

    bool all_files = false, l_files = false;

    vector<string> directories_arr;

    if(op.size()==1) directories_arr.push_back(pth);

    for(int i=1;i<op_size;i++){
        if(op[i][0]=='/') directories_arr.push_back(op[i]);
        else if(op[i]==".") directories_arr.push_back(pth);
        else if(op[i]==".."){

            string tmp = pth;

            int n = pth.size();

            int j = n-1;

            while(j>=0 && tmp[j]!='/'){
                tmp.pop_back();
                j = tmp.size()-1;
            }

            if(j>=0) tmp.pop_back();

            directories_arr.push_back(tmp);
        }
        else if(op[i]=="~") directories_arr.push_back(initial_path);
        else if(op[i][0]=='-'){
            for(int j=1;j<op[i].size();j++){
                if(op[i][j]=='a') all_files = true;
                else if(op[i][j]=='l') l_files = true;
            }
        }
        else{
            if(op[i]!="") directories_arr.push_back(pth+"/"+op[i]);
            else directories_arr.push_back(pth);
        }
    }

    if(directories_arr.size()==0 && (all_files || l_files)) directories_arr.push_back(pth);

    for(int i=0;i<directories_arr.size();i++){

        string dir_path = directories_arr[i];

        if(file_exists(dir_path)){
            string filePath = dir_path;
            string permissions_str = "";

            if(!l_files) cout << filePath.substr(pth.size()+1) << '\n';
            else if(l_files && access(filePath.c_str(), F_OK)==0){
                
                struct stat file_stat;

                stat(filePath.c_str(), &file_stat);


                // Checks for files and directories
                if (S_ISREG(file_stat.st_mode)) permissions_str+="-";
                else if (S_ISDIR(file_stat.st_mode)) permissions_str+="d";
                else {
                    cout << filePath << " is neither a regular file nor a directory.\n";
                }

                // Checks permissions for users

                int permission_bits[] = {0400, 0200, 0100, 0040, 0020, 0010, 0004, 0002, 0001}; 

                char permission_chars[] = {'r', 'w', 'x'};  

                for (int i = 0; i < 9; i++) {
                    permissions_str += (file_stat.st_mode & permission_bits[i]) ? permission_chars[i % 3] : '-';
                }

                // user
                struct passwd* pw = getpwuid(file_stat.st_uid);
                struct group* gr = getgrgid(file_stat.st_gid);

                struct tm* timeinfo = localtime(&file_stat.st_mtime);
                char time_str[30];
                strftime(time_str, sizeof(time_str), "%b %d %H:%M", timeinfo);

                cout << permissions_str << " " << file_stat.st_nlink << " " << pw->pw_name << " " << gr->gr_name << " " << file_stat.st_size << "\t  " << time_str << " " << filePath.substr(pth.size()) << '\n';
            }
        }
        else if(directory_exists(dir_path)){
            DIR *dir;

            if(dir_path=="") dir = opendir(".");
            else dir = opendir(dir_path.c_str());

            if(dir == nullptr){
                cerr << "Error opening the directory " << dir_path << '\n';
                return;
            }

            struct dirent* it;

            int total_blocks = 0;

            while((it = readdir(dir))!=nullptr){
                if (!all_files && (string(it->d_name) == "." || string(it->d_name) == ".." || string(it->d_name)[0]=='.')) {
                    continue;
                }

                struct stat file_stat;

                string filePath = dir_path + "/" + it->d_name;

                if (stat(filePath.c_str(), &file_stat) == 0) {
                    total_blocks += file_stat.st_blocks;
                } else {
                    cerr << "Failed to get stats for " << filePath << "\n";
                }

            }

            if(dir_path.size()>=initial_path.size()){
                if(directories_arr.size()>1) cout << dir_path.substr(pth.size()+1) << '\n';
            }
            else if(directories_arr.size()>1) cout << dir_path << '\n';


            if(l_files) cout << "total " << total_blocks/2 << '\n';
            
            DIR *dir2;

            if(dir_path=="") dir2 = opendir(".");
            else dir2 = opendir(dir_path.c_str());


            if(dir2 == nullptr){
                cerr << "Error opening the directory " << dir_path << '\n';
                return;
            }

            struct dirent* it2;

            while((it2 = readdir(dir2))!=nullptr){
                if (!all_files && (string(it2->d_name) == "." || string(it2->d_name) == ".." || string(it2->d_name)[0]=='.')) {
                    continue;
                }

                string filePath = dir_path + "/" + it2->d_name;

                string permissions_str = "";

                if(!l_files) cout << it2->d_name << '\n';
                else if(l_files && access(filePath.c_str(), F_OK)==0){
                    
                    struct stat file_stat;

                    stat(filePath.c_str(), &file_stat);


                    // Checks for files and directories
                    if (S_ISREG(file_stat.st_mode)) permissions_str+="-";
                    else if (S_ISDIR(file_stat.st_mode)) permissions_str+="d";
                    else {
                        cout << filePath << " is neither a regular file nor a directory.\n";
                    }

                    // Checks permissions for users

                    int permission_bits[] = {0400, 0200, 0100, 0040, 0020, 0010, 0004, 0002, 0001}; 

                    char permission_chars[] = {'r', 'w', 'x'};  

                    for (int i = 0; i < 9; i++) {
                        permissions_str += (file_stat.st_mode & permission_bits[i]) ? permission_chars[i % 3] : '-';
                    }

                    // user
                    struct passwd* pw = getpwuid(file_stat.st_uid);
                    struct group* gr = getgrgid(file_stat.st_gid);

                    struct tm* timeinfo = localtime(&file_stat.st_mtime);
                    char time_str[30];
                    strftime(time_str, sizeof(time_str), "%b %d %H:%M", timeinfo);

                    cout << permissions_str << " " << file_stat.st_nlink << " " << pw->pw_name << " " << gr->gr_name << " " << file_stat.st_size << "\t  " << time_str << " " << it2->d_name << '\n';
                }
                else{
                    cerr << "Unable acces the file\n";
                }
            
            }

        }
        else{
            cerr << "ls: cannot access " << directories_arr[i] << ": No such file or directory\n";
        }
    }
    
}