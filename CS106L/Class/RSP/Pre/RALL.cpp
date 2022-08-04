#include <iostream>

using std::cout;using std::endl;

class person{
    public:
        person(const std::string name ="", int age = 0) : 
        name_(name) , age_(age)  {
            cout << "Init a person!" << endl;
        }

        ~person(){
            cout << "destroy a person!" << endl;
        }

        const std::string& getname() const{
            return this->name_;
        }

        const int& getage() const{
            return this->age_;
        }

    private:
        int age_;
        const std::string name_;
};


int main(int argc, char const *argv[])
{
    person p;
    cout << "name_: " << p.getname() << endl;
    cout << "age_: " << p.getage() << endl;
    return 0;
}
