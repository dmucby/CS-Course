#include<string>
#include<iostream>
#include<cmath>

using std::string;
using std::cout;
using std::cin;
using std::pow;
using std::sqrt;
using std::endl;

std::pair<bool,std::pair<double,double>> quadratic(double a,double b,double c){

    double radical = pow(b, 2) - (4*a*c);

    if(radical < 0){
        return {false,{-1,-1}};
    }else{
        double root1 = ( -1*b + sqrt(radical) ) / (2*a);
        double root2 = ( -1*b - sqrt(radical) ) / (2*a);

        return {true,{root1,root2}};
    }
}

int main(int argc, char const *argv[])
{
    double a,b,c;
    cout << "Input three numbers:" << endl;
    cin >> a >> b >> c;

    auto res = quadratic(a,b,c);

    bool exists = res.first;
    auto roots = res.second;

    if (exists){
        double root1 = roots.first;
        double root2 = roots.second;

        cout << "Solutions are:" << root1 << " " << root2 << endl;
    }else{
        cout << "No solution exist!" <<endl;
    }
    return 0;
}

