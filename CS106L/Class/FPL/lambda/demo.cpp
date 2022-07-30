#include <iostream>
#include <vector>
using std::cout;using std::endl;


template <typename InputIt, typename UnPred>
int count_occurrences(InputIt begin, InputIt end, UnPred pred){
    int count = 0;
    for (auto iter = begin; iter != end; iter++){
        const auto& elem = *iter;
        if(pred(elem)) count++;
    }
    return count;
}


int main(int argc, char const *argv[])
{
    // auto printNum = [] (int n) -> void {
    //     cout << n << endl;
    // };

    // printNum(5);

    int limit = 5;
    auto isMoreThan = [limit] (int n) -> bool{
        return n > limit;
    };
    std::vector<int> nums = {3,5,6,7,9,13};
    cout << count_occurrences(nums.begin(), nums.end(),isMoreThan) << endl; 

    return 0;
}
