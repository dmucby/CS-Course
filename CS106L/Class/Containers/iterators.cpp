/*************************************************************************
	> File Name: iterators.cpp
	> Author: Boyu-DMU
	> Mail: cby@dlmu.edu.cn
	> Created Time: 2022年07月17日 星期日 10时44分10秒
 ************************************************************************/

#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;


int numOccurreces(std::vector<int>& cont, int elemToCount){
	int counter = 0;
	// 可随意改变迭代器指针的类型 来遍历任意
	std::vector<int>::iterator iter;
	for(iter = cont.begin() ;iter != cont.end() ; ++iter){
		// iter 是一个地址 
		if(*iter == elemToCount){
			counter++;
		}
	}
	return counter;
}

void make_pair_s(){
	// make_pair is not know the type
	std::make_pair("1" , 1);

	std::pair<string , int> p {"1",1}; 

	// auto 创建 ignore the type
	auto p = std::make_pair(1,45);

}

/**
 * @brief 遍历Map
 * 
 */
void map_iterators(){

	map<int , int> m;

	map<int , int>::iterator iter;
	map<int , int>::iterator end;
	while (i != end){
		// 注意括号 保证优先级
		cout << (*i).first << (*i).second << endl;
		// cout << key << val << endl;
		++iter;
	}

	// basic loop
	// elem = [key , val] ;
	for(auto elem : m){
		cout << elem.first << "," << elem.second << endl;
	}

}



// g++ 编译
// gcc 编译

int main(){
	std::vector<int> cont = {1,1,1,1};

	int res  = numOccurreces(cont, 1);

	std::cout << res << std::endl;
	return 0;

}

