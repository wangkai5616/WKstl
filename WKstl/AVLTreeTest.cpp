#include <iostream>
#include "Algorithm.h"
#include "Alloc.h"
#include "Allocator.h"
#include "Iterator.h"
#include "TypeTraits.h"
#include "Construct.h"
#include "ReverseIterator.h"
#include "Functional.h"
#include "AVLTree.h"
#include "AVLTree.impl.h"
#include <algorithm>
#include <cassert>
#include <random>
#include <string>
#include <deque>

using namespace WKstl;
/*
fun1()函数用于测试
1构造函数avl_tree():root_(0),size_(0){};
2insert(const T& val);
3height()const;
4size()const;
5empty()const;
6print_preorder(const std::string& delim = " ", std::ostream& os = std::cout)const;
7print_inorder(const std::string& delim = " ", std::ostream& os = std::cout)const;
8print_postorder(const std::string& delim = " ", std::ostream& os = std::cout)const;
9print_levelorder(const std::string& delim = " ", std::ostream& os = std::cout)const;
10find_min()、find_max()、find()、erase()不通过
*/
void fun1() {
	avl_tree<int> av1;
	av1.insert(1);
	av1.insert(10);
	av1.insert(3);
	av1.insert(5);
	av1.insert(9);
	if (av1.height() == 3)
		std::cout << 1 << std::endl;
	else
		std::cout << 0 << std::endl;
	if (av1.size() == 6)
		std::cout << 1 << std::endl;
	else
		std::cout << 0 << std::endl;
	bool b1 = av1.empty();
	std::cout << b1 << std::endl;
	/*
	if (*av1.find(5) == 5)
	std::cout << 1 << std::endl;
	else
	std::cout << 0 << std::endl;
	*/
	//av1.erase(1);
	av1.print_preorder();
	std::cout << std::endl;
	av1.print_inorder();
	std::cout << std::endl;
	av1.print_levelorder();
	std::cout << std::endl;
	av1.print_postorder();
	std::cout << std::endl;
}
int main()
{
	fun1();
	system("pause");
	return 0;
}