#include <iostream>
#include <memory>

using std::cout; using std::endl;

class Rectangle{
    int length;
    int breadth;

    public:
        Rectangle(const int &l,const int &b) 
            : length(l), breadth(b){ 
                cout << "Initial SPTR" << endl;
            }

        Rectangle(){
            cout << "Initial SPTR" << endl;
        }
        
        ~Rectangle(){
            cout << "destroy the SPTR" << endl;
        }
        
        const int area(){
            return length * breadth;
        }
};

int main(int argc, char const *argv[])
{
    // std::unique_ptr<Rectangle> P1(new Rectangle(10,5));
    // cout << P1->area() << endl;

    // std::unique_ptr<Rectangle> P2;
    // // 转移而不是拷贝
    // P2 = std::move(P1);

    // // 对比P1 和 P2
    // cout << "P2: " << P2->area() << endl;
    // // 段错误直接退出程序
    // // cout << "P1: " <<P1->area() << endl;

    std::shared_ptr<Rectangle> P1(new Rectangle(10, 5));
    std::shared_ptr<Rectangle> P2 = P1;

    cout << "P1:" << P1->area() << endl;
    cout << "P2:" << P2->area() << endl;

    cout << "count:" << P1.use_count() << endl;

    return 0;
}
