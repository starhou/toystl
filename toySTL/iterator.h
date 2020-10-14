#ifndef TOYSTL_ITERATOR_H_
#define TOYSTL_ITERATOR_H_

#include "util.h"

namespace toystl
{
    /*五种迭代器 Category*/
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

    /*迭代器模板*/
    template <class Category, class T>
    struct iterator
        {
            public:
            typedef Category                   iterator_category;
            typedef toystl::remove_cv_t<T>     value_type;
            typedef T*                         pointer;
            typedef T&                         reference;
            typedef const T                    const_value_type;
            typedef const T*                   const_pointer;
            typedef const T&                   const_reference;
            typedef std::ptrdiff_t             difference_type;
        };

        /*萃取某个迭代器的 category*/
        template <class Category>
        struct iterator_traits
        {
            typedef Category iterator_category;
        };
          // 利用重载技术统一指针，默认指针为random_access_iterator_tag
        template <class T>
        struct iterator_traits<T*>
        {
          
            typedef random_access_iterator_tag iterator_category;
        };
        /*这个函数作用是返回一个符合标签的对象*/
        template <class Iterator>
        typename iterator_traits<Iterator>::iterator_category
        iterator_category(const Iterator&)
        {
          typedef typename iterator_traits<Iterator>::iterator_category Category;
          return Category();
        } 
}
#endif