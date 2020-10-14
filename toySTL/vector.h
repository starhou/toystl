#ifndef TOYSTL_VECTOR_H_
#define TOYSTL_VECTOR_H_

#include <memory>
#include "util.h"
#include "iterator.h"

namespace toystl
{
	/*vector*/
	template <typename T>
	class vector
	{
	public:
		/*typical defination*/
		typedef toystl::remove_cv_t<T>     value_type;
		typedef T*                         pointer;
		typedef T&                         reference;
		typedef const T                    const_value_type;
		typedef const T*                   const_pointer;
		typedef const T&                   const_reference;
		typedef std::ptrdiff_t             difference_type;
		typedef std::size_t				   size_type;

		/*use native point as vector iterator*/
		typedef T* 						   iterator; 
		typedef const T*				   const_iterator;

		/*构造函数*/
		vector(): elements(nullptr), first_free(nullptr), cap(nullptr){};
		vector(std::initializer_list<T> il);
		vector(const size_type n);
		vector(const size_type n, const value_type& value);
		vector(const vector &);
		vector(vector &&) noexcept;

		/*赋值运算符*/
		vector &operator=(const vector&);
		vector &operator=(vector &&rhs) noexcept;

		/*析构函数*/
		~vector();

		/*初始化*/
		constexpr void assign( size_type count, const T& value )
		{
			alloc_n_fill(count, value);
		};

		/*常用接口*/	
		void push_back(const T&);
		void push_back(T&&);
		void pop_back();
		
		difference_type size() 		const {return first_free-elements;}
		difference_type capacity()  const {return cap-elements;}
		iterator begin() 			const {return elements;}
		iterator end() 	 			const {return first_free;}
		bool empty()				const {return elements == first_free;}

		/*重载操作符*/
		reference operator[]
			(const difference_type i)	{return *(begin() + i);}
		const_reference operator[]
			(const difference_type i)	const{return *(begin() + i);}
		
		

	private:
			/*使用了std的分配子*/
			static std::allocator<T> alloc;

			/*辅助内存分配函数*/
			void chk_n_alloc() {if(size() == capacity()) reallocate();}
			toystl::pair<T*, T*> alloc_n_fill
				(const size_type, const value_type&);
			toystl::pair<T*, T*> alloc_n_copy
				(const T*, const T*);
			void free();
			void reallocate();

			/*三个成员迭代器*/
			iterator elements;
			iterator first_free;
			iterator cap;
	};


/***************************************************************************
 * 
 *				函数实现  c++ primer中的strVec的扩充版 
 * 				
 *  			Date 2020.10.06 by starhou
***************************************************************************/

/*静态变量运行，分配内存分配器*/
template <typename T>
std::allocator<T> vector<T>::alloc;

/*
  	分配并填充内存	

	返回{first, second}
		first 分配内存起始位置
		second 分配内存结束后第一个位置

*/
template <typename T>
toystl::pair<T*,T*> 
	vector<T>::alloc_n_copy
		(const T* b, const T* e)
{
	auto data = alloc.allocate(e-b);
	return {data, std::uninitialized_copy(b, e, data)};
}

/*
  	分配并填充内存	

	返回{first, second}
		first 分配内存起始位置
		second 分配内存结束后第一个位置
*/
template <typename T>
toystl::pair<T*,T*> 
	vector<T>::alloc_n_fill
		(const size_type n, const value_type& value)
{
	auto data = alloc.allocate(n);
	return {data, std::uninitialized_fill_n(data, n, value)};
}

/*释放内存*/
template <typename T>
void vector<T>::free()
{
	if (elements)
	{
		for(auto p = first_free; p!=elements;)
			alloc.destroy(--p);
		alloc.deallocate(elements, cap-elements);
	}
}

/*内存倍增扩容*/
template <typename T>
void vector<T>::reallocate()
{
	auto newcapacity = size()? 2*size() : 1;
	auto newdata = alloc.allocate(newcapacity);

	auto dest = newdata;
	auto elem = elements;

	for (size_t i = 0; i != size(); ++i)
	{
		alloc.construct(dest++,toystl::move(*elem++));
	}
	free();
	elements = newdata;
	first_free = dest;
	cap = elements + newcapacity;
}

/*初始化列表构造函数*/
template <typename T>
vector<T>::vector(std::initializer_list<T> il)
{
	auto newdata = alloc_n_copy(il.begin(), il.end());
	elements = newdata.first;
	first_free = cap = newdata.second;
}

/*移动构造函数*/
template <typename T>
vector<T>::vector(vector<T> &&s) noexcept
:elements(s.elements), first_free(s.first_free), cap(s.cap)
{
	s.elements = s.first_free = s.cap = nullptr;
}

/*拷贝构造函数*/
template <typename T>
vector<T>::vector(const vector<T> &s)
{
	auto newdata = alloc_n_copy(s.begin(), s.end());
	elements = newdata.first;
	first_free = cap = newdata.second;
}

/*计数构造函数*/
template <typename T>
vector<T>::vector(const size_type n)
{
	auto newdata = alloc_n_fill(n, value_type());
	elements = newdata.first;
	first_free = cap = newdata.second;
}
template <typename T>
vector<T>::vector(const size_type n, const value_type& value)
{
	auto newdata = alloc_n_fill(n, value);
	elements = newdata.first;
	first_free = cap = newdata.second;
}

/*析构函数*/
template <typename T>
vector<T>::~vector(){free();}

/*赋值运算符*/
template <typename T>
vector<T> &vector<T>::operator = (const vector<T> &rhs)
{
	auto data = alloc_n_copy(rhs.begin(), rhs.end());
	free();
	elements = data.first;
	first_free = cap = data.second;
	return *this;
}

/*右值赋值运算符*/
template <typename T>
vector<T> &vector<T>::operator=(vector<T> &&rhs) noexcept
{
	if(this!= &rhs)
	{
		free();
		elements = rhs.elements;
		first_free = rhs.first_free;
		cap = rhs.cap;
		rhs.elements = rhs.first_free = rhs.cap = nullptr;
	}
	return *this;
}

/*加入元素*/
template <typename T>
void vector<T>::push_back(const T& s)
{
	chk_n_alloc();
	alloc.construct(first_free++, s);
}
template <typename T>
void vector<T>::push_back(T&& s)
{
	chk_n_alloc();
	alloc.construct(first_free++, toystl::move(s));
}
/*删除元素*/
template<class T>
void vector<T>::pop_back(){
	--first_free;
	alloc.destroy(first_free);
}

}
#endif