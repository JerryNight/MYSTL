#include <iostream>
using std::cout;
using std::endl;

template <class T>
class RAII{
public:
    // 构造函数
    RAII(T * data):_data(data){ cout << "RAII()" << endl; }
    ~RAII()
    {
        delete _data;
        _data = nullptr;
    }
    // 拷贝构造与赋值运算符
    RAII(const RAII & ) = delete;
    RAII & operator=(const RAII &) = delete;

    // 重载 -> ：raii调用->时，相当于一个指针调用->
    // raii.operator->()->print();
    T * operator->()
    {
        return _data;
    }
    // 重载 * 解引用
    // raii.operator*() 相当于 *raii, 得到指针指向的值
    T & operator*()
    {
        return *_data;
    }
    // 获取裸指针
    T * get() const
    {
        return _data;
    }
    // 重置
    void reset(T * data)
    {
        if (_data)
        {
            delete _data;
        }
        _data = data;
    }
    // 手动释放所有权
    T * release()
    {
        T * temp = _data;
        _data = nullptr;
        return temp;
    }

private:
    T * _data;
};