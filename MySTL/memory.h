#ifndef MYSTL_MEMORY_H_
#define MYSTL_MEMORY_H_

#include "allocator.h"
#include "algobase.h"
#include "construct.h"
#include "uninitialized.h"
#include "tempbuf.h"


// 实现一个智能指针
namespace mystl
{
    
// 模板类 auto_ptr
template <class T>
class auto_ptr
{
public:
    // 构造函数
    explicit auto_ptr(T* p = nullptr):ptr(p){}
    // 拷贝构造
    auto_ptr(auto_ptr& rhs):ptr(rhs.release()){}
    // 析构函数
    ~auto_ptr(){ delete ptr; }
    // 赋值运算符
    auto_ptr& operator=(const auto_ptr& rhs)
    {
        if (this != &rhs)
        {
            delete ptr;
            ptr = rhs.release();
        }
        return *this;
    }
    // 赋值运算符：不同类型
    template <class U>
    auto_ptr& operator=(const auto_ptr<U>& rhs)
    {
        if (this->get() != rhs.ptr)
        {
            delete ptr;
            ptr = rhs.release();
        }
        return *this;
    }

public:
    // 重载->
    T* operator->(){ return ptr; }

    // 重载*
    T& operator*(){ return *ptr; }
    
    // get 获得裸指针
    T* get() { return ptr; }

    // reset 重置
    void reset(T* p)
    {
        delete ptr;
        ptr = p;
    }

    // release 释放
    T* release()
    {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

private:
    T* ptr;
};

} // namespace mystl



#endif // MYSTL_MEMORY_H_