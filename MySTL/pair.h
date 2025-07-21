#ifndef MYSTL_PAIR_H_
#define MYSTL_PAIR_H_


namespace mystl
{

template <class T1, class T2>
struct pair
{
    typedef T1   first_type;
    typedef T2   second_type;

    T1 first;
    T2 second;

    // 构造函数
    pair():first(T1()), second(T2()) {}
    pair(const T1& a, const T2& b):first(a),second(b) {}

    template <class U1, class U2>
    pair(pair<U1, U2>& p):first(p.first), second(p.second) {}
};

// 重载运算符
template <class T1, class T2>
inline bool operator==(pair<T1, T2>& p1, pair<T1, T2>& p2)
{
    return p1.first == p2.first && p1.second == p2.second;
}

template <class T1, class T2>
inline bool operator!=(pair<T1, T2>& p1, pair<T1, T2>& p2)
{
    return p1.first != p2.first || p1.second != p2.second;
}

template <class T1, class T2>
inline bool operator>(pair<T1, T2>& p1, pair<T1, T2>& p2)
{
    return p1.first > p2.first || (!(p1.first > p2.first) && p1.second > p2.second); //first/second有一个满足条件即可
}

template <class T1, class T2>
inline bool operator<(pair<T1, T2>& p1, pair<T1, T2>& p2)
{
    return p2 < p1;
}

template <class T1, class T2>
inline bool operator>=(pair<T1, T2>& p1, pair<T1, T2>& p2)
{
    return !(p1 < p2);
}

template <class T1, class T2>
inline bool operator<=(pair<T1, T2>& p1, pair<T1, T2>& p2)
{
    return !(p1 > p2);
}

// 让两个数据成为一个pair
template <class T1, class T2>
inline pair<T1, T2> make_pair(const T1& a, const T2& b)
{
    return pair<T1, T2>(a,b);
}


} // namespace mystl



#endif