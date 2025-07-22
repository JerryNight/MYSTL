#ifndef MYSTL_VECTOR_H_
#define MYSTL_VECTOR_H_

#include "iterator.h"
#include "memory.h"



namespace mystl
{

// 模板类 vector
template <class T, class Alloc = mystl::alloc>
class vector
{
public:
    // vector嵌套类型定义
    typedef T                                         value_type;
    typedef Alloc                                     allocator_type;
    typedef value_type*                               pointer;
    typedef const value_type*                         const_pointer;
    typedef value_type&                               reference;
    typedef const value_type&                         const_reference;
    typedef size_t                                    size_type;
    typedef ptrdiff_t                                 difference_type;

    typedef value_type*                               iterator;
    typedef const value_type*                         const_iterator;
    typedef mystl::reverse_iterator<iterator>         reverse_iterator;
    typedef mystl::reverse_iterator<const_iterator>   const_reverse_iterator;

    typedef mystl::allocator<T, Alloc>                data_allocator;
    
    allocator_type get_allocator(){return allocator_type();}

public:
    // 构造
    vector():start_(nullptr), finish_(nullptr), end_of_storage_(nullptr) {}
    explicit vector(size_type n)                 { __fill_initialize(n, value_type()); }
    vector(size_type n, const value_type& value) { __fill_initialize(n, value); }
    // 构造函数 - 参数为迭代器
    template <class InputIterator>
    vector(InputIterator first, InputIterator last);
    // 拷贝构造
    vector(const vector& rhs);
    vector(vector&& rhs);
    // 赋值运算符
    vector& operator=(const vector& rhs);
    vector& operator=(vector&& rhs);
    // 析构
    ~vector(__destroy_and_deallocate();)

public:
    // 迭代器相关操作
    iterator begin() {return start_;}
    const_iterator begin()const {return start_;}
    iterator end() {return end_;}
    const_iterator end()const {return end_;}
    reverse_iterator rbegin() {return reverse_iterator(end());}
    const_reverse_iterator rbegin()const {return const_reverse_iterator(end());}
    reverse_iterator rend() {return reverse_iterator(begin());}
    const_reverse_iterator rend()const {return const_reverse_iterator(begin());}
    // 容量相关操作
    bool empty()const {return begin() == end();}
    size_type size()const {return static_cast<size_type>(end() - begin());}
    size_type max_size()const {return static_cast<size_type>(-1) / sizeof(T);}
    size_type capacity()const {return static_cast<size_type>(end_of_storage_ - start_);}
    void shrink_to_fit();
    void reserve(size_type n); 



private:
    // 如果是数值类型，用此函数初始化
    template <class Integer>
    void __vector_initialize(Integer n, Integer value, __true_type);
    // 如果不是数值类型，用此函数初始化
    template <class InputIterator>
    void __vector_initialize(InputIterator first,InputIterator last, __false_type);
    // 先destroy析构对象，再deallocate回收内存空间
    void __destroy_and_deallocate();
    // 先allocate给对象分配内存，再uninitialized_fill_n 初始化
    void __fill_initialize(size_type n, const value_type& value);
    // 先allocate给对象分配内存，再uninitialized_copy 把first~last的数据复制到新对象
    template <class InputIterator>
    void __range_initialize(InputIterator first,InputIterator last);
    //
    template <class Integer>
    void __assign_dispatch(Integer n, Integer value, __true_type);
    // 
    template <class InputIterator>
    void __assign_dispatch(InputIterator first, InputIterator last, __false_type);

private:
    iterator start_;          // vector里已用空间头结点
    iterator finish_;         // vector里已用空间尾结点
    iterator end_of_storage_; // vector里整体空间尾结点

};

template <class T, class Alloc>
template <class InputIterator>
vector<T, Alloc>::vector(InputIterator first, InputIterator last)
{
    
}

// 拷贝构造
template <class T, class Alloc>
vector<T, Alloc>::vector(const vector& rhs){}

template <class T, class Alloc>
vector<T, Alloc>::vector(vector&& rhs){}

// 赋值运算符
template <class T, class Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(const vector& rhs){}

template <class T, class Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(vector&& rhs){}


} // namespace mystl



#endif