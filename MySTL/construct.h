#ifndef MYSTL_CONSTRUCT_H_
#define MYSTL_CONSTRUCT_H_

// 该头文件包含两个全局函数：construct 构造对象, destroy 析构对象

#include "iterator.h"
#include "type_traits.h"

namespace mystl
{
    
// construct版本1：接受一个指针
template <class T>
inline void construct(T* p)
{
    new (static_cast<void*>p) T();
}

// construct版本2：接受一个指针和初始化值
template <class T1, class T2>
inline void construct(T1* p,const T2& value)
{
    new (static_cast<void*>p) T(value);
}

// destroy版本1: 接收一个指针。显式调用对象的析构函数
template <class T>
inline void destroy(T* p)
{
    p->~T();
}

// destroy版本2: 适用容器。接收两个迭代器
template <class ForwardIterator>
inline void destroy(ForwardIterator first,ForwardIterator last)
{
    __destroy(first, last, value_type(first));
}

template <class ForwardIterator, class T>
inline void destroy(ForwardIterator first,ForwardIterator last, T*)
{
    typedef typename __type_traits<T>::has_trivial_destructor TrivalDestructor;
    return __destroy_aux(first, last, TrivalDestructor());
}

template <class ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type) {
    for (; first != last; ++first)
        destroy(&*first);
}

// 如果是对象有 trivial destructor，则什么也不做
template <class ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __true_type) {}

// 如果销毁的对象是指针，则什么也不做
inline void destroy(char*, char*) {}
inline void destroy(wchar_t*, wchar_t*) {}
inline void destroy(int*, int*) {}
inline void destroy(long*, long*) {}
inline void destroy(float*, float*) {}
inline void destroy(double*, double*) {}

} // namespace mystl



#endif