// system and timing
#include <iostream>
#include <chrono>

// data structure imports
#include <deque>
#include <vector>
#include <iterator>
#include <utility> // pair

using std::cout;    using std::endl;

const char target = 'a';

// count_occurrences
template <typename InPuit, typename UnPred>
int count_occurrences(InPuit begin, InPuit end, UnPred pred){
    int count = 0;
    for (auto iter = begin; iter != end; iter++){
        auto const& elem = *iter;
        if(pred(elem)) count++;
    }
    return count;
}


int main() {

    // vector of strings
    std::vector<std::string> lands = {"Xadia", "Drakewood", "Innean"};
    // how can we count the number of times "Xadia" appears in this?
    
    // string itself (array of chars)
    std::string str = "Xadia";
    // how can we count the number of times 'a' appears in this?
    auto pred = [] (char s) -> bool{
        return s == target;
    };
    cout << count_occurrences(str.begin(), str.end(), pred) << endl;
    return 0;
}

