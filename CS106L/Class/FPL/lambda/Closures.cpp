# include <iostream>

using std::cout;using std::endl;

class functor{
    public:
        int operator() (int arg) const{
            return num + arg;
        }
    private:
        int num;
};

int main(int argc, char const *argv[])
{
    int num = 0;
    auto lambda = [&num] (int arg) -> int{
        num += arg;
        return num;
    };
    cout << lambda(5) <<endl;

    return 0;
}


