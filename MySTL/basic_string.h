#ifndef MYSTL_BASIC_STRING_H_
#define MYSTL_BASIC_STRING_H_

#include "iterator.h"
#include "memory.h"
#include "type_traits.h"

namespace mystl
{

// 模板类 basic_string
// 参数一：字符类型
// 参数二：萃取字符类型的方式，默认使用 __char_type
// 参数三：空间配置器，默认使用 alloc
template <class CharType, class CharTraits = mystl::__char_type<CharType>, class Alloc = mystl::alloc>
class basic_string
{
// 定义类型别名
public:
    typedef CharType                                   value_type;
    typedef Alloc                                      allocator_type;
    typedef value_type*                                pointer;
    typedef const value_type*                          const_pointer;
    typedef value_type&                                reference;
    typedef const value_type&                          const_reference;
    typedef size_t                                     size_type;
    typedef ptrdiff_t                                  difference_type;

    typedef value_type*                                iterator;
    typedef const value_type*                          const_iterator;
    typedef mystl::reverse_iterator<iterator>          reverse_iterator;
    typedef mystl::reverse_iterator<const_iterator>    const_reverse_iterator;

    typedef mystl::allocator<value_type, Alloc>        data_allocator;
    allocator_type get_allocator() { return allocator_type(); }

    // 末尾位置
    static constexpr size_type end_pos = static_cast<size_type>(-1);

private:
    iterator buffer_; // 字符串起始位置
    iterator finish_; // 字符串结束位置
    iterator end_;    // 存储空间结束位置

public:
    // 构造、复制、移动、析构函数
    basic_string()                                   { __initialize_string(0, value_type()); }
    explicit basic_string(size_type n)               { __initialize_string(n, value_type()); }
    basic_string(size_type n, value_type ch)         { __initialize_string(n, ch); }

    basic_string(const_pointer str)                  { __copy_from(str, 0, __get_strlen(str)); }
    basic_string(const_pointer str, size_type count) { __copy_from(str, 0, count); }

    template<class InputIterator>
    basic_string(InputIterator first, InputIterator last);

    basic_string(const basic_string& other, size_type count);
    basic_string(const basic_string& other, size_type index, size_type count);

    basic_string(const basic_string& rhs) :buffer_(nullptr), finish_(nullptr), end_(nullptr) {
        *this = rhs;
    }
    basic_string(basic_string&& rhs) :buffer_(nullptr), finish_(nullptr), end_(nullptr) {
        *this = std::move(rhs);
    }

    basic_string& operator=(const basic_string& rhs);
    basic_string& operator=(basic_string&& rhs);
    basic_string& operator=(const_pointer str) {
        basic_string tmp(str);
        *this = std::move(tmp);
        return *this;
    }
    basic_string& operator=(value_type ch) {
        basic_string tmp(1, ch);
        *this = std::move(tmp);
        return *this;
    }

    ~basic_string() { __destroy_buffer(); }

public:
    // 迭代器相关操作
    iterator               begin()        { return buffer_; }
    const_iterator         begin()  const { return buffer_; }
    iterator               end()          { return finish_; }
    const_iterator         end()    const { return finish_; }
    reverse_iterator       rbegin()       { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return reverse_iterator(end()); }
    reverse_iterator       rend()         { return reverse_iterator(begin()); }
    const_reverse_iterator rend()   const { return reverse_iterator(begin()); }

    // 容量相关操作
    bool      empty()    const { return buffer_ == finish_; }
    size_type size()     const { return finish_ - buffer_; }
    size_type length()   const { return finish_ - buffer_; }
    size_type capacity() const { return end_ - buffer_; }
    size_type max_size() const { return static_cast<size_type>(-1) / sizeof(value_type); }
    void      shrink_to_fit();

    // 访问元素相关操作
    reference       operator[](size_type n)       { return *(buffer_ + n); }
    const_reference operator[](size_type n) const { return *(buffer_ + n); }
    reference       at(size_type n)               { return (*this)[n]; }
    const_reference at(size_type n)         const { return (*this)[n]; }
    reference       front()                       { return *begin(); }
    const_reference front()                 const { return *begin(); }
    reference       back()                        { return *(end() - 1); }
    const_reference back()                  const { return *(end() - 1); }
    pointer         data()                        { return __get_str(); }
    const_pointer   data()                  const { return __get_str(); }
    pointer         c_str()                       { return __get_str(); }
    const_pointer   c_str()                 const { return __get_str(); }

public:
    // 添加删除相关操作
    iterator insert(iterator pos, size_type count, value_type ch);
    iterator insert(iterator pos, value_type ch);
    iterator insert(iterator pos, const_pointer str) {
        insert(pos, str, str + __get_strlen(str));
        return pos + __get_strlen(str);
    }

    template <class InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last);

    iterator erase(iterator pos)                            { return erase(pos, 1); }
    iterator erase(iterator pos, size_type count);
    void erase(iterator first, iterator last)               { erase(first, last - first); }
    void clear();

    void add_back(value_type ch)                            { insert(end(), ch); }
    void add_back(value_type ch, size_type count)           { insert(end(), count, ch); }
    void add_back(const_pointer str)                        { add_back(str, __get_strlen(str)); }
    void add_back(const_pointer str, size_type count);
    template <class InputIterator>
    void add_back(InputIterator first, InputIterator last)  { insert(end(), first, last); }

    void add_front(value_type ch)                           { insert(begin(), ch); }
    void add_front(value_type ch, size_type count)          { insert(begin(), count, ch); }
    void add_front(const_pointer str)                       { add_front(str, __get_strlen(str)); }
    void add_front(const_pointer str, size_type count);
    template <class InputIterator>
    void add_front(InputIterator first, InputIterator last) { insert(begin(), first, last); }

    // basic_string 相关操作
    difference_type compare(const basic_string& other)        const;
    basic_string    substr(size_type index);
    basic_string    substr(size_type index, size_type count);
    void            remove(value_type ch);
    template <class UnaryPredicate>
    void            remove_if(UnaryPredicate up);
    void            replace(value_type ch, value_type rch);
    template <class UnaryPredicate>
    void            replace_if(UnaryPredicate up, value_type rch);
    size_type       find(value_type ch)                       const;
    size_type       find(value_type ch, size_type index)      const;
    size_type       find(const_pointer str)                   const;
    size_type       find(const_pointer str, size_type index)  const;
    size_type       find(const basic_string& str)             const;
    size_type       rfind(value_type ch, size_type index)     const;
    size_type       rfind(value_type ch)                      const;
    size_type       rfind(const_pointer str, size_type index) const;
    size_type       rfind(const_pointer str)                  const;
    size_type       rfind(const basic_string& str)            const;
    size_type       count(value_type ch)                      const;
    size_type       count(value_type ch, size_type index)     const;
    void            reverse() { mystl::reverse(begin(), end()); }
    void            swap(basic_string& rhs);

    // 重载 operator+= 
    basic_string& operator+=(const basic_string& str) {
        if(str.length())
            add_back(str.begin(), str.end());
        return *this;
    }
    basic_string& operator+=(value_type ch) {
        add_back(ch);
        return *this;
    }
    basic_string& operator+=(const_pointer str) {
        if(__get_strlen(str))
            add_back(str, str + __get_strlen(str));
        return *this;
    }

private:
    size_type     __init_size()                   { return static_cast<size_type>(16); }
    pointer       __get_buffer(size_type n)       { return data_allocator::allocate(n); }
    pointer       __get_buffer(size_type n) const { return data_allocator::allocate(n); }
    void          __put_buffer(pointer buf)       { data_allocator::deallocate(buf); }
    void          __destroy_buffer();
    pointer       __get_str();
    const_pointer __get_str() const;
    size_type     __get_strlen(const_pointer str) const;
    size_type     __get_strlen(const_pointer str, char_type)  const { return strlen(str); }
    size_type     __get_strlen(const_pointer str, wchar_type) const { return wcslen(str); }
    void          __initialize_string(size_type n, value_type ch);
    void          __copy_from(const_pointer src, size_type pos, size_type n);
    iterator      __reallocate_and_fill(iterator pos, size_type n, value_type ch);
    iterator      __reallocate_and_copy(iterator pos, const_iterator first, const_iterator last);


};




} // namespace mystl



#endif