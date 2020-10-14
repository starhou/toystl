#include "util.h"
#include "vector.h"
#include "iterator.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <type_traits>







int main()
{

    toystl::vector<int> v(10,-1);
    toystl::vector<toystl::vector<double>> v2(10, toystl::vector<double>(10));

    v.push_back(666);
    v.push_back(666);
    v.push_back(666);
    v[5] = 1314;

    std::for_each(v.begin(), v.end(), [](auto s){std::cout<<s<<" ";});
    
    std::for_each(v2.begin(), v2.end(), [](auto s)
    {for(auto& iter:s) std::cout<<iter<<" "; std::cout<<std::endl;});

    
    return 0;
}


// 