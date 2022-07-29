#include<iostream>

using namespace std;

struct Student {
	string name; // these are called fields
	string state; // separate these by semicolons
	int age;
};

Student s;
s.name = "Frankie";
s.state = "MN";
s.age = 21; // use . to access fields

void printStudentInfo(Student student) {
	cout << s.name << " from " << s.state;
	cout << " (" << s.age ")" << endl;
}

Student randomStudentFrom(std::string state) {
	Student s;
	s.name = "Frankie";//random = always Frankie
	s.state = state;
	s.age = std::randint(0, 100);
	return s;
}

 Student foundStudent = randomStudentFrom("MN");
 cout << foundStudent.name << endl; // Frankie

std::pair<bool, Student> lookupStudent(string name) {
	Student blank;
	if (found(name)) return std::make_pair(false, blank);
	Student result = getStudentWithName(name);
	return std::make_pair(true, result);
}

int main(int argc, char const *argv[])
{
	std::pair<bool, Student> output = lookupStudent(“Keith”);
	std::cout << output.first << std::endl;
	std::cout << output.second << std.second;
	return 0;
}

