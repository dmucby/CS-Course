#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstring>

using std::cout;    
using std::endl;
using std::string; 
using std::cin;
using std::pair;



//read numbers from a file
void readNumbers() {
    // Create our ifstream and make it open the file
    std::ifstream in("res/numbers.txt");

    // Something to store values we get from the stream
    int num;
    
    // Extract next number from input
    while(true){
        in >> num;

        // If input is in a fail state, either a value couldn't
        // if(in.fail()){
        //     cout << "fail" << endl;
        //     break;
        // }else 
        if(in.eof()){
            cout << "eof" << endl;
            break;
        }else{
            // be converted, or we are at the end of the file.
            cout << "num value:" << num << endl;
            //print whatever we read
        }
    }    
}

//read a single word at a time from a file
void readHaikuWord() {
    // Create our ifstream and make it open the file
    std::ifstream in("res/haiku.txt");

     // Something to store values we get from the stream
    string Word;
        // Extract next word from input
    while(true){
        // If input is in a fail state, either a value couldn't
        in >> Word;
        // be converted, or we are at the end of the file.
        if(in.eof()){
            break;
        }else{
            cout << "word value:" << Word << endl;
        }
        //print whatever we read
    }
}

void readHaikuLine() {
    // Create our ifstream and make it open the file
    std::ifstream in("res/haiku.txt");

    string str;
    
    // Something to store the lines as we get them form the stream
    pair<string,string> lines;
    int id;
    char tmp_str[50];
    
    while(true){
        getline(in,lines.second);
        // If input is in a fail state, either a value couldn't
        if(in.fail()){
            cout << "fail" << endl;
            break;
        }else if(in.eof()){
            sprintf(tmp_str, "the number %d sentence:", id);
            id++;
            // be converted, or we are at the end of the file.
            lines.first = (string(tmp_str));
            //print whatever we read
            cout << lines.first << lines.second <<endl;
            break;
        }else{
            //std::ifstream div(id);
            //div >> str;
            sprintf(tmp_str, "the number %d sentence:", id);
            id++;
            // be converted, or we are at the end of the file.
            lines.first = (string(tmp_str));
            //print whatever we read
            cout << lines.first << lines.second <<endl;
        }    
    }
}



//function for us to play around with cin
void playground(){
    string line;
    std::ifstream in("res/haiku.txt");
    while(std::getline(in, line)){
        if(in.eof()){
        cout << "eof" << endl;
         // break;
        }
        else if (in.fail()) {
            cout << "fail" << endl;
            // break;
        }
        else cout << line << endl;
        cout << in.eof() << in.fail() << endl;
    }
    cout << in.eof() << in.fail() << endl;
}

int main() {

    readHaikuLine();

    //readHaikuWord();

    //readNumbers();

    //playground();

    return 0;
}


