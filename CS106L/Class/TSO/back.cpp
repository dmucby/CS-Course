#include <iostream>
#include <vector>

using std::vector;

// template <typename valuetype>
// std::pair<bool, valuetype&> vector<valuetype>::back(){
//     if(empty()){
//         return {false, valuetype()};
//     }
//     return {true, *(begin() + size() -1)};
// }

template <typename valuetype>
std::optional<valuetype> vector<valuetype>::back(){
    if(empty()){
        return {false, valuetype()};
    }
    return {true, *(begin() + size() -1)};
}



int main(int argc, char const *argv[])
{
    std::optional<int> num1 = {}; // nothing
    num1 = 1;
    num1 = std::nullopt; // dosen't anymore
    return 0;
}
