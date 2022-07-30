#include <iostream>
#include <vector>

using std::cout;using std::endl;
using std::vector;

bool isVowel(char c){
    std::string vowels = "iaiyou";
    return vowels.find(c) != std::string::npos;
}

template <typename InputIt, typename UnPred>
int count_occurrences(InputIt begin, InputIt end, UnPred pred){
    int count = 0;
    for (auto iter = begin; iter != end; iter++){
        const auto& elem = *iter;
        if(pred(elem)) count++;
    }
    return count;
}

int main(){
    std::string str = "Xadia";
    cout << count_occurrences(str.begin(), str.end(), isVowel);
}