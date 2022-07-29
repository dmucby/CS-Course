#include<iostream>
#include<fstream>
#include<sstream>

using namespace std;

struct Student {
	string name; // these are called fields
	//string state; // separate these by semicolons
	int age;
    string state;
};

string judgementCall(int age ,string name, bool lovesCpp){
    ostringstream formatter;
    formatter << name << ",age " << age;
    if(lovesCpp) formatter << ", rocks.";
    else formatter << " could be better";
    return formatter.str();
}

Student reverseJudgementCall(string judgement)
{ //input: “Frankie age 22, rocks”
    std::istringstream converter;
    string fluff; int age; bool lovesCpp; string name;
    converter >> name;
    converter >> fluff;
    converter >> age;
    converter >> fluff;
    string cool;
    converter >> cool;
    if(cool == "rocks") return Student{name, age, "bliss"};
    else return Student{name, age, "misery"};
}// returns: {“Frankie”, 22, “bliss”}

int main(int argc, char const *argv[])
{
    // /**
    //  * output streams
    // **/ 
    // Student s = {"Frankie" , "MN" , 21};
    // cout << s.name << s.age << endl;

    // // cout << s << endl;  不接受结构体
    
    // ofstream out("out.txt");
    // out << 5 << endl;

    /**
     * Input streams
    **/ 
    // int x;
    // string str;

    // cin >> x >> str;
    // cout << x << " " << str << endl;

    // string line;
    // getline(cin,line);
    // cout << line << endl;

    // ifstream in("out.txt");
    // string str;
    // in >> str;
    // cout << str << endl;

    string str = judgementCall(1,"age",true);
    cout << str << endl;

    return 0;
}
