#include "header.h"
#include<bits/stdc++.h>
#include<sys/stat.h>
#include<dirent.h>
#include<sys/types.h>
#include<unistd.h>
#include <cstring>

using namespace std;

bool search_func(string fileName,string pth){
    DIR* dir = opendir(pth.c_str());

    if(dir==nullptr){
        perror("opendir");
        return false;
    }

    struct dirent* entry;

    while((entry=readdir(dir))!=nullptr){
        string fn = entry->d_name;
        string full_path = pth + "/" + fn;

        if(fn=="." || fn=="..") continue;

        // If file name matches return true
        if(fn==fileName) return true;

        struct stat file_stat;
        if (stat(full_path.c_str(), &file_stat) == -1) {
            perror("stat");
            continue;
        }

        // If it is a directory iterate the directory
        if(S_ISDIR(file_stat.st_mode)){
            string new_path = pth + "/" + fn;

            if(search_func(fileName,new_path)) return true;
        }
    }

    closedir(dir);

    return false;
}