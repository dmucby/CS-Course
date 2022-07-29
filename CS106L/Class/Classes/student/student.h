#include <iostream>

using std::string;
using std::cout;using std::endl;

class Student
{
private:
    std::string name;
    std::string state;
    int age;
    
public:
    std::string getName();
    void setName(std::string name);
    int getAge();
    void setAge(int age);
    Student(std::string name, std::string state, int age);
    Student();
    ~Student();
};

Student::Student() {

}

Student::Student(std::string name, std::string state, int age)
{
    this->age = age;
    this->state = state;
    this->name = name;
}

Student::~Student()
{
}
