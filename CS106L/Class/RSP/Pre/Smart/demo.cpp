#include <iostream>

using std::cout; using std::endl;

template <class T>
class SmartPtr{
    T* ptr;
    public:
        explicit SmartPtr(T* p = NULL) : ptr(p){cout << "Initial ptr" <<endl; }
        ~SmartPtr(){ cout << "delete the ptr" << endl;delete (ptr);}
        T& operator*(){ return *ptr;}
        T* operator->() {return ptr;}
};   


int main(int argc, char const *argv[])
{
    SmartPtr<int> ptr(new int());
    *ptr = 20;
    cout << "*ptr: "<< *ptr << endl;
    return 0;
}
