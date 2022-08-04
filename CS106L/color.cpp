#include <iostream>

#define RED     "\033[31m" 
#define RESET   "\033[0m"
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */


using std::cout;using std::endl;

int main(int argc, char const *argv[])
{
    cout << RED << "1" << RESET << endl;
    cout << BLUE << "2" << RESET << endl;
    cout << MAGENTA << "3" << RESET << endl;
    return 0;
}
