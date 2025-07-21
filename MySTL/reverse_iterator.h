#ifndef MYSTL_REVERSE_ITERATOR_H_
#define MYSTL_REVERSE_ITERATOR_H_

namespace mystl
{
    
template <class Iterator>
class reverse_iterator
{
private:
    // 对应的正向迭代器
    Iterator current;

public:
    // 反向迭代器的别名
    typedef typename iterator_traits<Iterator>::iterator_category    iterator_category;
    typedef typename iterator_traits<Iterator>::value_type           value_type;
    typedef typename iterator_traits<Iterator>::difference_type      difference_type;
    typedef typename iterator_traits<Iterator>::pointer              pointer;
    typedef typename iterator_traits<Iterator>::reference            reference;

    typedef Iterator                                                 iterator_type;
    typedef reverse_iterator<Iterator>                               self;

public:
    // 构造函数
    reverse_iterator() {}
    explicit reverse_iterator(iterator_type i) :current(i) {}
    reverse_iterator(const self& rhs) :current(rhs.current) {}
};

} // namespace mystl


#endif