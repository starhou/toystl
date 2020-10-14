#ifndef PYROOLS_H_
#define PYROOLS_H_
#include<iostream>
using std::cout;
using std::endl;

namespace pytool{

    /*
        powerful debug tool
        // cout <<  __PRETTY_FUNCTION__ << '\n';


    EXPEXCTION:
       In:  print(T a)
       Out: a

       In:  print(Container<T> A)  N=3 
       Out: a a a

       In:  print(Container<Container<A>> A)  4*3
       Out: a a a a
            a a a a
            a a a a 
    

    2020.10.7 by starhou

    Ref:
        [1] https://stackoverflow.com/questions/31768769/c-correct-syntax-for-triple-nested-template-parameter
        [2] https://stackoverflow.com/questions/37298393/variadic-templates-example
        [3] https://eli.thegreenplace.net/2014/variadic-templates-in-c/
        [4] https://my.oschina.net/u/4283333/blog/3423803
    */


/************************************************************************
 *                       print基本的实现
 * *********************************************************************/

/*单个字符*/
template<typename T>
void print(const T &t, const char delemetar = '\0')
{
    // cout <<  __PRETTY_FUNCTION__ << '\n';
    if(delemetar == '\0') 
        std::cout<<t;
    else 
        std::cout<<delemetar<<t;
}

/*单层容器*/
template<template<typename> class Container, class T>
void print(Container<T>& l, const char delemetar = ' ')
{
    bool flag = false;
    for(auto& iter:l)
    {
        if(flag) print(delemetar);
        print(iter);
        flag = true;
    }
}

/*双层容器*/
template <template<typename> class Container1,
        template<typename> class Container2, class T>
inline void print(Container1<Container2<T>> l, const char delemetar = ' ') {
    /*new line*/
    print('[');
    bool flag = false;
    for(auto& line:l)
    {
        if(flag) print('\n');
        print('[');
        print(line, delemetar);
        print(']');            
        flag = true;
    }
    print(']');
}

/************************************************************************
 *                       一些拓展的思考
 * *********************************************************************/

    /*任意数量参数*/
    template <class... Types>
    void print (const Types&... args)
    {   
        // cout <<  __PRETTY_FUNCTION__ << '\n';
        // print('\n');
        using expander = int[];
        (void) expander { 0, (void(cout << args << ' '), 0) ...};
    }
}

namespace testools
{

    // template <template <typename, typename...> class ContainerType,
    //         typename ValueType, typename... Args>
    // void print_container(const ContainerType<ValueType, Args...>& c) {
    // for (const auto& v : c) {
    //     std::cout << v << ' ';
    // }
    // std::cout << '\n';
    // }


    // template <template <template <class> class> class Ptr,
    //       template<class> class Container, class T>
    //     inline void print_Container(Ptr<Container> l) {
    //             // ^^^ Not Ptr<Container<T>>
    // std::cout << '[';
    // for (auto it = l->begin(); it != l->end() - 1; ++it) {
    //     std::cout << *it << ", ";
    // }
    // std::cout << l->back() << ']' << std::endl;
    // }
}

#endif 