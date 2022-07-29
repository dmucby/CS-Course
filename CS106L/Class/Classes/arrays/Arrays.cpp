//
// Created by cby on 22-7-27.
//
#include <iostream>

using std::cout;using std::endl;

int main(){
    int *my_int_array;

    // allocate the memory
    my_int_array = new int[10];
    int one_element = my_int_array[0];
    cout << one_element << " " << my_int_array[0] << endl;

    // deallocate that memory
    delete [] my_int_array;
}