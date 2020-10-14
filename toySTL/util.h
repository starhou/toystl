#ifndef TOYSTL_UTIL_H_
#define TOYSTL_UTIL_H_
#include <type_traits>
#include <vector>

namespace toystl
{

/*
  move, forward, 普通传参  特点汇总


  输入引用   	类别		  输出引用       
			
		    普通传参：         左值
  右值       move:            右值
		    forward:          右值

			普通传参           左值
  左值       move		      右值
            forward:          左值
*/


/**
 * 
 * 去除顶层const和顶层volatile
 * ref: https://zh.cppreference.com/w/cpp/types/remove_cv
*/
template< class T > struct remove_cv                   { typedef T type; };
template< class T > struct remove_cv<const T>          { typedef T type; };
template< class T > struct remove_cv<volatile T>       { typedef T type; };
template< class T > struct remove_cv<const volatile T> { typedef T type; };


template< class T >
using remove_cv_t = typename remove_cv<T>::type;
/*
*  remove_reference: 返回原类型
*  ref: https://en.cppreference.com/w/cpp/types/remove_reference 
* 
*/ 
template< class T > struct remove_reference      {typedef T type;};
template< class T > struct remove_reference<T&>  {typedef T type;};
template< class T > struct remove_reference<T&&> {typedef T type;};

template< class T >
using remove_reference_t = typename remove_reference<T>::type;

/*
*  move: 返回右值类型 
*  		 参数是右值： 参数去引用->转化为右值返回
*  ref: https://en.cppreference.com/w/cpp/utility/move
* 
*/ 
template< class T >
constexpr remove_reference_t<T>&& move( T&& t ) noexcept
{
	return static_cast<remove_reference_t<T>&&>(t);
}

/*
*  forward: 左值返回左值，右值返回右值
*  ref: https://en.cppreference.com/w/cpp/utility/forward 
*/ 

template <class T>
T&& forward(typename std::remove_reference<T>::type& arg) noexcept
{
  return static_cast<T&&>(arg);
}

template <class T>
T&& forward(typename std::remove_reference<T>::type&& arg) noexcept
{
  static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
  return static_cast<T&&>(arg);
}






/*
*  pair: 乞版pair 包含make_pair函数
*  ref: https://en.cppreference.com/w/cpp/utility/pair
*/
	template<class T1,class T2> struct pair;

	template<class T1,class T2> 
	constexpr pair<T1,T2> make_pair (T1&& t, T2&& u);

	template< class T1, class T2 >
	constexpr pair<T1,T2> make_pair( T1& t, T2& u );


	template<class T1,class T2> 
	struct pair
	{
	public:
		/*默认构造*/
		constexpr pair(const T1& a, const T2& b)
		: first(a), second(b){}

		// constexpr pair(std::initializer_list<T1> il)
		// : first(*il.begin()), second(*il.end()){}

		/*友元*/
		template< class T3, class T4 >
		friend constexpr pair<T3,T4> make_pair(T3&& t, T4&& u);
		template< class T3, class T4 >
		friend constexpr pair<T3,T4> make_pair(T3& t, T4& u);

		/*移动构造函数*/
		pair(pair&& rhs):first(toystl::move(rhs.first)), second(toystl::move(rhs.second)){}
		/*拷贝构造函数*/
		pair(const pair& p) = default;
		/*析构函数*/
		~pair() = default;
		/*拷贝赋值函数*/
		pair& operator=(const pair& rhs);
		/*移动赋值函数*/
		pair& operator=(pair&& rhs);

		T1 first;  
		T2 second; 
	}; 


	template<class T1,class T2> 
	pair<T1, T2>& pair<T1, T2>::operator=(const pair& rhs)
	{
		if (this != &rhs)
		{
		first = rhs.first;
		second = rhs.second;
		}
		return *this;
	}

	template<class T1,class T2> 
	pair<T1, T2>& pair<T1, T2>::operator=(pair&& rhs)
	{
		if (this != &rhs)
		{
			first = toystl::move(rhs.first);
			second = toystl::move(rhs.second);
		}
		return *this;
	}

	/*make_pair是以友元方式存在*/
	template< class T1, class T2 >
	constexpr pair<T1,T2> make_pair( T1&& t, T2&& u )
	{
		return pair<T1, T2>(forward<T1>(t), forward<T2>(u));
	}

	template< class T1, class T2 >
	constexpr pair<T1,T2> make_pair( T1& t, T2& u )
	{
		return pair<T1, T2>(forward<T1>(t), forward<T2>(u));
	}



  /**/

}
#endif 