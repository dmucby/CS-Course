#include<iostream>


a = "test";
b = 3.2 * 5 - 1;
c = 5 / 2;

int d(int foo){
	return foo / 2;
}

double e(double foo){
	return foo / 2;
}

int f(double foo){
	return int(foo / 2);
}

void g(double c){
	std::cout << c << std::endl;
}

int main(int argc, char const *argv[])
{
	int val = 5; //32 bits
	char ch = 'F'; //8 bits (usually)
	float decimalVal1 = 5.0; //32 bits (usually)
	double decimalVal2 = 5.0; //64 bits (usually)
	bool bVal = true; //1 bit
	std::string str = "Frankie";
	return 0;

	
}