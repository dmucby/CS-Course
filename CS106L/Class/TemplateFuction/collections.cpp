#include <set>
#include <iostream>

using std::cout;using std::endl;

int main(int argc, char const *argv[])
{
    std::set<int> sets = {1,2,3,4,5};
    for(auto iter = sets.begin(); iter != sets.end(); iter++){
        // 注意不直接暴露
        auto &elem = *iter;
        cout << elem << " ";
    }
    cout << endl;
    return 0;
}
