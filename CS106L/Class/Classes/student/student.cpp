#include "student.h"

std::string Student::getName(){
    return name;
}

void Student::setName(string name){
    this->name = name;
}


int Student::getAge(){
    return age;
}


void Student::setAge(int age){
    if( age > 1){
        this->age = age;
    }else{
        cout << "请输入正确的年龄" << endl;
        //error(); 教学中正确使用了error方法 此处有待考证
    }
}

int main(){
    Student *frankie = new Student("Frankie","MN",21);
    cout << frankie->getName() << "the age is:" << frankie->getAge() << endl;

    Student sathya("Sathya","New Jersey",20);
    Student liuchang;
    liuchang.setName("liuchang");
    liuchang.setAge(21);

    cout << sathya.getName() << " " << sathya.getAge() << endl;
    cout << liuchang.getAge() << " " << liuchang.getName() << endl;
}