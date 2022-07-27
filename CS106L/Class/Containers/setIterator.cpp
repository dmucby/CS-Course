#include <iostream>
#include <set>
// 不存在特殊find
#include <algorithm> 

using std::set;
using std::cout; using std::endl;

const int ELEM_TO_FIND = 5;

int main(int argc, char const *argv[])
{
    set<int> elems = {1,2,5,3,4,8,9,1,1,1,1,6};
    cout << elems.size() <<endl;

    std::set<int>::iterator find_elem;
    find_elem = std::find(elems.begin() , elems.end(), ELEM_TO_FIND);

    if( find_elem == elems.end()){
        cout << "Error!" << endl;
    }else {
        // 不要忘记迭代器前的*
        // 取地址中的内容
        cout << *find_elem << endl;
    }

    // 设置上下界
    std::set<int>::iterator iter_begin = elems.lower_bound(3); // >= key
    std::set<int>::iterator iter_end = elems.upper_bound(7); // < key
    
    for(; iter_begin != iter_end ; iter_begin++){
        cout << *iter_begin << " ";
    }
    cout << endl;

    // 求某个数的个数 
    cout << elems.count(ELEM_TO_FIND) << endl;


    

    return 0;
}
