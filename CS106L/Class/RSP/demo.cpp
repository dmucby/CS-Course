#include <iostream>
// smart pointer 
#include <memory>

using std::cout;using std::endl;

int main(int argc, char const *argv[])
{
    std::shared_ptr<int> p1(new int);
    {
        // 局部变量
        std::shared_ptr<int> p2 = p1;
        cout << p1.use_count() << endl;
    }
    std::shared_ptr<int> p2 = p1;
    cout << *p1.get() << endl; // deleted
    // cout << *p2.get() << endl;
    cout << p1.use_count() << endl;

    // cout << *p1.get() << endl;
    // cout << *p2.get() << endl; 
    // cout << p2.use_count() << endl; 

    // make_unique
    std::shared_ptr<int> p3 = std::make_unique<int>(3);
    cout << "before move " << *p3.get() << endl;
    p3 = std::move(p2);
    cout << "after move " << *p3.get() << endl;

    return 0;
}
