#ifndef CD_FUNCTIONS_H
#define CD_FUNCTIONS_H

#include<bits/stdc++.h>
using namespace std;


void cd_func(string &pth,string operation,string &prev_pth,string initial_path);  // Function declaration
void cwd_func(char cwd[]);
void echo_func(string text);
vector<string> string_tokenizer(string str, const char *delim);
void ls_func(string pth,string commd,string initial_path);
void bg_fg_func(string comm);
void history_func(deque<string> dq,bool flag,int count);
void io_piping(string comm);
bool search_func(string fileName,string pth);
void pinfo_func(int pid);
void signal_func(string command);
void display_func();
void tab_func(string comm, bool space, bool double_tab);

#endif
