#include "../toySTL/util.h"
#include "../toySTL/vector.h"
#include "../toySTL/iterator.h"
#include "../toySTL/deque.h"
#include "pytools.h"
#include <iostream> 
using std::cout;
using std::endl;
#include <algorithm>
#include <string>
#include <type_traits>


toystl::deque<int> myfun()
{
    toystl::deque<int> q = {1,2,3,4,5,6};
    return q;
}


int main(int argc, char const *argv[])
{
    /* 构造函数 */
    /*0. 默认构造函数*/
    pytool::print("默认构造函数: deque<int> a; ");
    pytool::print('\n');
    toystl::deque<int> a;
    pytool::print(a);
    pytool::print('\n');

    /*1. 赋值的构造函数*/
    pytool::print("赋值的构造函数: deque<int> b(10,-1);");
    pytool::print('\n');
    toystl::deque<int> b(10,-1);
    pytool::print(b);
    pytool::print('\n'); 

    /*2. 拷贝构造函数*/
    pytool::print("拷贝构造函数: deque<int> c(b);");
    pytool::print('\n');
    toystl::deque<int> c(b);
    pytool::print(c);
    pytool::print('\n');

    /*3. 右值构造函数,初始化列表构造*/
    pytool::print("右值构造函数,初始化列表构造 ");
    pytool::print('\n');
    toystl::deque<int> d = myfun();
    pytool::print(d);
    pytool::print('\n');

    /*4. vector构造*/
    pytool::print("vector构造 ");
    pytool::print('\n');
    toystl::vector<int> v(10, -1);
    toystl::deque<int> e(v.begin(), v.end());
    pytool::print(e);
    pytool::print('\n');       
    return 0;
}