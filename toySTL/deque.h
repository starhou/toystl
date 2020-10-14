#ifndef TOYSTL_DEQUE_H_
#define TOYSTL_DEQUE_H_

#include <memory>
#include "util.h"
#include "iterator.h"
#include "../testFiles/pytools.h"



namespace toystl
{

    // deque map 初始化的大小
    #ifndef DEQUE_MAP_INIT_SIZE
    #define DEQUE_MAP_INIT_SIZE 8
    #endif

    template <class T>
    struct deque_buf_size
    {
    static constexpr size_t value = sizeof(T) < 256 ? 512 / sizeof(T) : 1;
    };
    /*deque_iterator*/
    template <class T, class Ref, class Ptr>
    struct deque_iterator
    {   
        /*typical defination*/
        typedef 
        random_access_iterator_tag         iterator_category;
		typedef toystl::remove_cv_t<T>     value_type;
		typedef T*                         pointer;
		typedef T&                         reference;
		typedef const T                    const_value_type;
		typedef const T*                   const_pointer;
		typedef const T&                   const_reference;
		typedef std::ptrdiff_t             difference_type;
        typedef std::size_t				   size_type;
        /*Specialized defination*/
        typedef deque_iterator<T, T&, T*>              iterator;
        typedef deque_iterator<T, const T&, const T*>  const_iterator;
        typedef deque_iterator                         self;
        typedef T**                                    map_pointer;

        static const size_type buffer_size = deque_buf_size<T>::value;

        pointer       elements;     // 指向所在缓冲区的当前元素

        pointer       first;        // 指向所在缓冲区的头部
        pointer       last;         // 指向所在缓冲区的尾部
        map_pointer   node;         // 缓冲区所在节点
        
        /*构造函数*/
        deque_iterator() noexcept
        :elements(nullptr), first(nullptr), last(nullptr), node(nullptr) {}

        deque_iterator(pointer elements_, map_pointer node_)
        :elements(elements_), first(*node_), 
         last(*node_ + buffer_size), node(node_) {}

        deque_iterator(const iterator& iter_)
            : elements(iter_.elements), first(iter_.first), 
            last(iter_.last), node(iter_.node) {}


        /*更新结点*/
        void goto_next_node(map_pointer node_) {
            node = node_;
            first = *node_;
            last = first + difference_type(buffer_size);
        }
        /*重载操作符*/
        reference operator*() const {return *elements;} 

        difference_type operator-(const self& iter_) const {
            return difference_type(buffer_size) * (node - iter_.node - 1) +
            (elements - first) + (iter_.elements - iter_.elements);
        }

        self& operator++() {
            ++elements;  // 先移动下一个元素
            if (elements == last) { // 如果已到达所在缓冲区的尾端
                goto_next_node(node + 1); // 就切换到下一节点(下一个缓冲区)
                elements = first;   // 指向下一个缓冲区的第一个元素
            }
            return *this; 
        }

        self operator++(int)  {
            self __tmp = *this;
            ++*this;
            return __tmp;
        }

        self& operator--() {
            if (elements == first) {  // 如果已到达所在缓冲区的头端
            goto_next_node(node - 1); // 就切换到上一个节点(上一个缓冲区)
            elements = last;  // 指向上一个缓冲区的最后一个元素
            }
            --elements;  // 移动上一个元素
            return *this;
        }
        
        self operator--(int) {
            self __tmp = *this;
            --*this;
            return __tmp;
        }

        self& operator+=(difference_type __n)
        {
            difference_type __offset = __n + (elements - first);
            if (__offset >= 0 && __offset < difference_type(buffer_size))
            elements += __n;  // 目标位置在同一缓冲区内
            else {  // 标记的位置不在同一缓冲区内
            difference_type __node_offset =
                __offset > 0 ? __offset / difference_type(buffer_size)
                        : -difference_type((-__offset - 1) / buffer_size) - 1; //下取整
            goto_next_node(node + __node_offset);   // 切换到正确的节点(缓冲区) 
            elements = first +   // 切换到正确的元素
                (__offset - __node_offset * difference_type(buffer_size));
            }
            return *this;
        }
        self& operator-=(difference_type __n) 
        { return *this += -__n; }
        self operator-(difference_type __n) const {
            self __tmp = *this;
            return __tmp -= __n;
        }            
        self operator+(difference_type __n) const
        {
            self __tmp = *this;
            return __tmp += __n;
        }
        reference operator[](difference_type n) const 
        {return *(*this + n);}

        /*重载比较操作符*/
        bool operator==(const self& rhs) const 
        { return elements == rhs.elements; }
        bool operator< (const self& rhs) const
        { return node == rhs.node ? (elements < rhs.elements) : (node < rhs.node); }
        bool operator!=(const self& rhs) const 
        { return !(*this == rhs); }
        bool operator> (const self& rhs) const 
        { return rhs < *this; }
        bool operator<=(const self& rhs) const 
        { return !(rhs < *this); }
        bool operator>=(const self& rhs) const 
        { return !(*this < rhs);}       

    };


	/*deque*/
	template <typename T>
    class deque
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
        /*Specialized defination*/
        typedef deque_iterator<T, T&, T*>              iterator;
        typedef deque_iterator<T, const T&, const T*>  const_iterator;
        typedef T**                                    map_pointer;
        typedef const T**                              const_map_pointer;


        static  const  size_type buffer_size = deque_buf_size<T>::value;

    private:
        /*使用了std的分配子*/
        static   std::allocator<T>                     value_alloc; 
        static   std::allocator<T*>                    pointer_alloc;

        // 用以下四个数据来表现一个 deque
        iterator       begin_;     // 指向第一个结点
        iterator       end_;       // 指向最后一个结点
        map_pointer    map_;       // 指向一块 map
        size_type      map_size_;  // map 内缓存区的数目
        /*内存分配函数*/
        // create node / destroy node
        map_pointer create_map(size_type size);
        void        create_buffer(map_pointer nstart, map_pointer nfinish);
        void        destroy_buffer(map_pointer nstart, map_pointer nfinish);
        void        clear();
        // initialize
        void        map_init(size_type nelem);
        void        fill_init(size_type n, const value_type& value);

        template <class IIter>
        void     copy_init(IIter, IIter, input_iterator_tag);
        


    public:


        /*构造函数*/
        deque()
        { fill_init(0, value_type()); }

        deque(value_type n)
        { fill_init(n, value_type()); }

        deque(value_type n, const value_type& value)
        { fill_init(n, value);}
        
        template <class IIter>
        deque(IIter first, IIter last)
        {copy_init(first, last, iterator_category(first)); }
        /*初始化列表构造函数*/
        deque(std::initializer_list<value_type> ilist)
        {
          copy_init(ilist.begin(), ilist.end(), input_iterator_tag());
        }
        /*拷贝构造函数*/
        deque(const deque& rhs)
        {
            copy_init(rhs.begin(), rhs.end(), input_iterator_tag());
        }
        /*右值构造函数*/
        deque(deque&& rhs) noexcept
        :begin_(toystl::move(rhs.begin_)),
            end_(toystl::move(rhs.end_)),
            map_(toystl::move(rhs.map_)),
            map_size_(toystl::move(rhs.map_size_))
            {
                // rhs.map_ = nullptr;
                // rhs.map_size_ = 0;
            }
        ~deque(){
            clear();
            pointer_alloc.deallocate(map_, map_size_);
            map_ = nullptr;
        }
        

        iterator               begin()         noexcept
        { return begin_; }
        const_iterator         begin()   const noexcept
        { return begin_; }
        iterator               end()           noexcept
        { return end_; }
        const_iterator         end()     const noexcept
        { return end_; }
    };
    

    /*静态变量运行，分配内存分配器*/
    template <typename T>
    std::allocator<T> deque<T>::value_alloc;
    template <typename T>
    std::allocator<T*> deque<T>::pointer_alloc;

    /*一个缓存区的内存 返回头指针*/
    template <class T>
    typename deque<T>::map_pointer
    deque<T>::create_map(size_type size)
    {
        map_pointer mp = nullptr;
        mp = pointer_alloc.allocate(size);
        for (size_type i = 0; i < size; ++i)
            *(mp + i) = nullptr;
        return mp;
    }

    // create_buffer 函数
    template <class T>
    void deque<T>::
    create_buffer(map_pointer nstart, map_pointer nfinish)
    {
        map_pointer cur;
        try
        {
            for (cur = nstart; cur <= nfinish; ++cur)
            {
                *cur = value_alloc.allocate(buffer_size);
            }
        }
        catch (...)
        {
            while (cur != nstart)
            {
                --cur;
                value_alloc.deallocate(*cur, buffer_size);
                *cur = nullptr;
            }
            throw;
        }
    }

    // destroy_buffer 函数
    template <class T>
    void deque<T>::
    destroy_buffer(map_pointer nstart, map_pointer nfinish)
    {
        for (map_pointer n = nstart; n <= nfinish; ++n)
        {
            value_alloc.deallocate(*n, buffer_size);
            *n = nullptr;
        }
    }

    // map_init 函数
    template <class T>
    void deque<T>::
    map_init(size_type nElem)
    {
    const size_type nNode = nElem / buffer_size + 1;  // 需要分配的缓冲区个数
    map_size_ = std::max((size_type) DEQUE_MAP_INIT_SIZE, nNode + 2);
    try
    {
        map_ = create_map(map_size_);
    }
    catch (...)
    {
        map_ = nullptr;
        map_size_ = 0;
        throw;
    }

    // 让 nstart 和 nfinish 都指向 map_ 最中央的区域，方便向头尾扩充
    map_pointer nstart = map_ + (map_size_ - nNode)/2;
    map_pointer nfinish = nstart + nNode - 1;
    try
    {
        create_buffer(nstart, nfinish);
    }
    catch (...)
    {
        pointer_alloc.deallocate(map_, map_size_);
        map_ = nullptr;
        map_size_ = 0;
        throw;
    }
    begin_.goto_next_node(nstart);
    end_.goto_next_node(nfinish);
    begin_.elements = begin_.first;
    end_.elements = end_.first + (nElem % buffer_size);
    }

    // fill_init 函数
    template <class T>
    void deque<T>::
    fill_init(size_type n, const value_type& value)
    {
        map_init(n);
        if (n != 0)
        {
            for (auto cur = begin_.node; cur < end_.node; ++cur)
            {
            std::uninitialized_fill(*cur, *cur + buffer_size, value);
            }
            std::uninitialized_fill(end_.first, end_.elements, value);
        }
    }

    // copy_init 函数
    template <class T>
    template <class IIter>
    void deque<T>::
    copy_init(IIter first, IIter last, input_iterator_tag)
    {        
        size_type n = 0;
        /*advace功能*/
        auto cnt = first;
        while (cnt != last)
        {
            ++cnt; ++n;
        }
        map_init(n);
        for (iterator iter = begin_; first != last; ++first,++iter)
            {*iter.elements = *first;}
    }


    // 清空 deque
    template <class T>
    void deque<T>::clear()
    {
        for (map_pointer cur = begin_.node + 1; cur < end_.node; ++cur)
        {
            auto iter = *cur;
            auto cnt = buffer_size;
            while (cnt--)
            {
                value_alloc.destroy(*cur);
                ++iter;
            }
            
            value_alloc.deallocate(*cur, buffer_size);
        }

        if (begin_.node != end_.node)
        { // 有两个以上的缓冲区
            for (auto cur = begin_.first; cur != begin_.last; ++cur)
            {
                value_alloc.destroy(&cur); 
            }
            value_alloc.deallocate(begin_.first, buffer_size);

            for (auto cur = end_.first; cur != end_.elements; ++cur)
            {
                value_alloc.destroy(&cur); 
            }
            value_alloc.deallocate(end_.first, buffer_size);
        }
        else
        {
            for (auto cur = begin_.first; cur!= end_.elements; ++cur)
            {
                value_alloc.destroy(&cur); 
            }
            value_alloc.deallocate(begin_.first, buffer_size);
        }
        begin_ = end_;
    }  
    
}

#endif