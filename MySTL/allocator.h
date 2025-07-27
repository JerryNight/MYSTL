#ifndef MYSTL_ALLOCATOR_H_
#define MYSTL_ALLOCATOR_H_

#include "alloc.h"

namespace mystl
{
// allocator 模版类：用于对象内存的分配与释放
// 参数一：对象的类型，参数二：空间配置器的类型
template <class T, class Alloc>
class allocator
{
public:
    typedef T            value_type;
    typedef T*           pointer;
    typedef const T*     const_pointer;
    typedef T&           reference;
    typedef const T&     const_reference;
    typedef size_t       size_type;
    typedef ptrdiff_t    difference_type;

public:
    static T*   allocate();
    static T*   allocate(size_type n);
    static void deallocate(T* ptr);
    static void deallocate(T* ptr, size_type n);
};

// allocator 版本一：无参数，之间根据对象的大小返回分配的内存
template <class T, class Alloc>
T* allocator<T, Alloc>::allocate()
{
    return static_cast<T*>(Alloc::allocate(sizeof(T)));
}

// allocator 版本二：参数为对象的个数
template <class T, class Alloc>
T* allocator<T, Alloc>::allocate(size_type n)
{
    return static_cast<T*>(Alloc::allocate(n * sizeof(T)));
}

// deallocator 版本一：无参数，之间根据对象的大小返回分配的内存
template <class T, class Alloc>
void allocator<T, Alloc>::deallocate(T* ptr)
{   
    if (!ptr) return;
    Alloc::deallocate(ptr, sizeof(T));
}

// allocator 版本二：参数为对象的个数
template <class T, class Alloc>
void allocator<T, Alloc>::deallocate(T* ptr, size_type n)
{
    if (!ptr) return;
    Alloc::deallocate(ptr, n * sizeof(T));
}

} // namespace mystl



#endif