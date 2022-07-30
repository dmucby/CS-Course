#include <iostream>
#include <vector>
#include <string>

using std::cout;using std::endl;
using std::vector;


// 直接对引用进行对比
template <class T>
T myMin(const T a,const T b){
    return a < b ? a:b;
}

template <typename T=int>
T myMin2(const T a,const T b){
    return a < b ? a:b;
}

template <typename T, typename U>
auto smartMyMin(T a, U b){
    return a < b? a:b;
}

template <unsigned n>
struct Factorial
{
    enum {
        value = n * Factorial<n-1>::value
    };
};

template<>
struct Factorial<0>
{
    enum{value = 1};
};

template <typename T>
int count_occurrences(const vector<T> vec, T target){
    int count = 0;
    for (size_t i = 0; i < vec.size(); i++)
    {
        if(vec[i] == target) count++;
    }
    return count;
    
}

template <typename InputIt, typename DataType>
int count_occurrences(InputIt begin, InputIt end, DataType target){
    int count = 0;
    for (auto iter = begin; iter != end; iter++){
        const auto& elem = *iter;
        if(elem == target) count++;
    }
    return count;
}


int main(int argc, char const *argv[])
{
    // cout << myMin(7.5, 8.3) << endl;
    // auto name = myMin("liu","li");
    // cout << name << endl;

    // vector<int> a = {3};
    // vector<int> b = {1};

    // // 比较 vector
    // // cout << myMin(a, b) << endl;

    // cout << myMin<float>(4.555,3.37888) << endl;

    // cout << Factorial<10>::value << endl;
    
    // cout << count_occurrences<std::string>({"Xadia", "Drakewood", 
    // "Innean"}, "Xadia") << endl;

    vector<std::string> lands = {"Xadia", "Drakewood", "Innean"};
    cout << count_occurrences(lands.begin(), lands.end(), "Innean");

    std::string str = "Xadia";
    cout << count_occurrences(str.begin(), str.end(), 'a');

    return 0;
}
