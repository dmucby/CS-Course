//
// Created by cby on 22-8-1.
//

#include <iostream>
#include <vector>

using std::vector;

using std::cout;using std::endl;

void shift(vector<std::pair<int ,int>>& nums){
    for (size_t i = 0; i < nums.size(); ++i){
        auto [num1, num2] = nums[i];
        num1++;
        num2++;
    }
}

void shift2(vector<std::pair<int, int>>& nums) {
    for (auto& [num1, num2]: nums) {
        // num1 num2 指的是什么？
        num1++;
        num2++;
        cout << "num1=" << num1 << ", num2=" << num2 << endl;
    }
}

int main(){
    // shift({{1, 1}}); 
    // // {{1, 1}} is an rvalue, it can’t be referenced
    vector<std::pair<int, int>> my_nums = {{1,1}};
    shift2(my_nums);
}