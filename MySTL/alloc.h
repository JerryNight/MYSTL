#ifndef MYSTL_ALLOC_H_
#define MYSTL_ALLOC_H_

#include <cstdlib> // size_t/std::exit
#include <iostream> // std::cerr

namespace mystl
{
// 共用体：一个指针，一个数据
// 用链表的方式管理内存碎片：若用户申请小内存块，用FreeList分配内存。相当于一个缓冲池
// 维护16个自由链表：8,16,24,32,40,48,56,64,72,80,88,96,104,112,120,128 bytes
union FreeList
{
    union FreeList* next;  // 指向下一个区块
    char data[1];          // 存储本块内存的首地址
};

static const int EAlign = 8;
static const size_t EMaxBytes = 128;
static const int ENFreeLists = 16;

class alloc
{
public:
    static void* allocate(size_t n);
    static void  deallocate(void* p, size_t n);
    static void* reallocate(void* p, size_t oldz_size, size_t new_size);

private:
    static char*  start_free;
    static char*  end_free;
    static size_t heap_size;
    static FreeList* free_list[ENFreeLists];

private:
    static size_t round_up(size_t bytes);
    static size_t freelist_index(size_t bytes);
    static void*  refill(size_t n);
    static char*  chunk_alloc(size_t size, size_t &nblock);
};

// 静态成员初始化
char* alloc::start_free = nullptr;
char* alloc::end_free = nullptr;
size_t alloc::heap_size = 0;       // 已经从系统分配的总内存
FreeList* alloc::free_list[ENFreeLists] = 
    { nullptr, nullptr, nullptr, nullptr,
      nullptr, nullptr, nullptr, nullptr,
      nullptr, nullptr, nullptr, nullptr,
      nullptr, nullptr, nullptr, nullptr };

// allocata:分配空间
void* alloc::allocate(size_t n)
{
    if (n > EMaxBytes)
        return std::malloc(n);
    FreeList* my_free_list = free_list[freelist_index(n)];
    FreeList* result = my_free_list;
    if (result == nullptr)
    {
        void* re = refill(round_up(n));
        return re;
    }
    my_free_list = my_free_list->next;
    return result;
}

// 释放指针p
void alloc::deallocate(void* p, size_t n)
{
    if (n > EMaxBytes)
    {
        std::free(p);
        return;
    }
    // 把要释放的对象p,转成可用内存空间
    FreeList* free_p = static_cast<FreeList*>(p);
    // 获取对应大小的自由链表
    FreeList* my_free_list = free_list[freelist_index(n)];
    // 头插法，将空间p回收至内存池
    free_p->next = my_free_list;
    my_free_list = free_p;
}

// 重新分配空间
void* alloc::reallocate(void* p, size_t oldz_size, size_t new_size)
{
    deallocate(p, oldz_size);
    p = allocate(new_size);
    return p;
}

// 将bytes上调到8的倍数
size_t alloc::round_up(size_t bytes)
{
    // bytes向上取整，&~(EAlign - 1)清除最后3比特，快速对齐
    return (bytes + EAlign - 1) & ~(EAlign - 1);
}

// 求对应的数组下标
size_t alloc::freelist_index(size_t bytes)
{
    return (bytes + EAlign - 1) / EAlign - 1;
}

// 重新填充freelist: 申请20个新内存块，返回一个，剩下的挂在链表中
void* alloc::refill(size_t n)
{
    // 默认向freelist填充20个新内存块
    size_t nblock = 20;
    // c 是连续的大内存段
    char* c = chunk_alloc(n, nblock);
    FreeList* & my_free_list = free_list[freelist_index(n)];
    FreeList* result, *cur, *next;
    // nblock == 1: chunk_alloc只申请到1个块
    if (nblock == 1)
        return c;
    // 申请了多个块，多余的块，作为free_list的新节点
    result = (FreeList*)(c); // 用static_cast报错
    my_free_list = next = (FreeList*)(c + n);
    for (size_t i = 1; i < nblock - 1; i++)
    {
        cur = next;
        next = (FreeList*)((char*)next + n);
        cur->next = next;
    }
    next->next = nullptr;
    return result;
}

// 从内存池中获取空间
char* alloc::chunk_alloc(size_t size, size_t &nblock)
{
    char* result;
    size_t need_bytes = size * nblock;
    size_t pool_bytes = end_free - start_free;
    // 当前内存足够分配nblock个对象
    if (pool_bytes >= need_bytes)
    {
        result = start_free;
        start_free += need_bytes;
        return result;
    }
    // 当前内存只够分配部分对象
    if (pool_bytes >= size)
    {
        nblock = pool_bytes / size;
        need_bytes = size * nblock;
        result = start_free;
        start_free += need_bytes;
        return result;
    }
    // 当前内存连一个对象都无法分配了，把这点内存挂到链表上
    if (pool_bytes > 0)
    {
        FreeList* my_free_list = free_list[freelist_index(pool_bytes)];
        ((FreeList*)start_free)->next = my_free_list;
        my_free_list = ((FreeList*)start_free)->next;
    }
    // 重新申请空间
    size_t get_bytes = 2 * need_bytes + round_up(heap_size >> 4);
    start_free = static_cast<char*>(std::malloc(get_bytes));
    // 如果申请失败
    if (!start_free)
    {
        FreeList* my_free_list, *p;
        // 到更大的链表里找空间空间，
        for (size_t i = size; i < EMaxBytes; i+=EAlign)
        {
            my_free_list = free_list[freelist_index(i)];
            p = my_free_list;
            if (p)
            {
                my_free_list = p->next;
                start_free = (char*)my_free_list;
                end_free = start_free + i;
                return chunk_alloc(size, nblock);
            }
            // 如果所有链表都没有空间了，表示已经尽力了，没有内存可用了
            end_free = nullptr;
            std::cerr << "out of memory" << std::endl;
            std::exit(1);
        }
    }
    end_free = start_free + get_bytes;
    heap_size += get_bytes;
    // 申请空间完毕，重新调用chunk_alloc，分配空间
    return chunk_alloc(size, nblock);
}

} // namespace mystl



#endif