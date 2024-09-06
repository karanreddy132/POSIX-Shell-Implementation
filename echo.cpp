#include "header.h"
#include<bits/stdc++.h>

using namespace std;


void echo_func(string text){

    int t = text.size();

    int i = 4;

    bool space = false;

    while(i<t && text[i]==' '){
        i++;
    }

    while(i<t){
        
        while(i<t && text[i]!=' '){
            if(space){
                cout << " " << text[i];
                space = false;
            }
            else cout << text[i]; 
            i++;
        }

        while(i<t && text[i]==' '){
            i++;
            space = true;
        }
    }

    cout << '\n';
    
}