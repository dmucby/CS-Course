//
// Created by cby on 22-8-1.
//

#include <iostream>
#include <fstream>
#include <sstream>

using std::cout;using std::endl;
using std::string;

string judgementCall(int age, string name, bool lovesCpp){
    std::ostringstream  formatter;
    formatter << name << ",age " << age;
    if(lovesCpp) formatter << ", rocks.";
    else formatter << " could be better";
    return formatter.str();
}

string reverseJudgementCall(string judgment){
    std::istringstream converter;
    string fluff; int age; bool lovesCpp; string name;
    converter >> name;
    converter >> fluff;
    converter >> age;
    converter >> fluff;
    string cool;
    return cool;
}


int main(){
    // 输出文件流
    std::ofstream out("out.txt", std::ofstream::out);
    out << 5 << endl;

    // 输入文件流
    std::ifstream in("out.txt");

    // 字符串输入流
    // 用作类型转换
//    std::istringstream formatter;
//    std::ostringstream outString;

    std::string str = judgementCall(18, "ss", true);
    cout << str << endl;
}