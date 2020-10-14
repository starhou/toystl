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




int main(int argc, char const *argv[])
{
    /* 构造函数 */
        /*0. 默认构造函数*/
        toystl::vector<int> v;
        pytool::print("construct with 0 arg: "); pytool::print('\n');
        pytool::print(v); pytool::print('\n');
        /*1. 初始化列表构造函数*/
        toystl::vector<int> v1 = {1,2,3,5,6,8,7,9}; pytool::print('\n');
        pytool::print("construct with initial list: "); pytool::print('\n');
        pytool::print(v1);
        /*2. 拷贝构造函数*/
        toystl::vector<int> v2(v1);
        pytool::print("construct with copy: "); pytool::print('\n');
        pytool::print(v2); pytool::print('\n');
        /*3. 计数构造函数*/
        toystl::vector<int> v3(10, -9);
        pytool::print("construct with cnt: "); pytool::print('\n');
        pytool::print(v3); pytool::print('\n');
        /*4. 赋值运算符*/
        pytool::print("=: "); 
        v2 = v3;
        pytool::print(v3); pytool::print('\n');
        /*5. empty()*/
        pytool::print("v.empty(), v1.empty() ");  pytool::print('\n');
        pytool::print(v.empty()); pytool::print('\n');
        pytool::print(v1.empty()); pytool::print('\n');
        /*6. 多维度构造函数*/
        toystl::vector<toystl::vector<toystl::vector<int>>> b(1, toystl::vector<toystl::vector<int>>(2,toystl::vector<int>(3,666)));
        pytool::print(b);
    return 0;
}