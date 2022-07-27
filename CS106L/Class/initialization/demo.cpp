#include<iostream>
#include<vector>

using std::pair;
using std::string;
using std::cout;
using std::endl;


// void changeX(int &x){
//     x = 0;
//     return;
// }

// void keepX(int x){
//     x = 0;
//     return;
// }

void shift(vector<std::pair<int, int>>& nums) {
    for (size_t i = 0; i < nums.size(); ++i) {
        auto& [num1, num2] = nums[i];
        num1++;
        num2++;
    }
    return;
}

int main(int argc, char const *argv[])
{
    // make_pair initialize
    // std::pair <int,string> demo = std::make_pair(3,"rd");

    // std::cout << demo.first << " " << demo.second << std::endl;

    // int a = 100;
    // int b = 100;

    // changeX(a);
    // keepX(b);
 
    // cout << a << " :" << &a << endl;

    // cout << b << " :" << &b << endl;

    auto my_nums = {{1,1}};

    shift(my_nums);

    return 0;
}
