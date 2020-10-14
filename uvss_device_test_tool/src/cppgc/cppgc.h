#ifndef CPP_GC_H
#define CPP_GC_H 1
/**
 * 本模块实现了一个可以自动析构的智能指针。
 * 举例说明：
 * 如果你用 char *s = (char *)malloc(10) 或者用 A *a = new A()，申请了一段内存，
 * 你必须时刻记住，要及时 free 或 delete 它们，这么做，非常繁琐且耗费精力，且容易出错。
 * 
 * 为了避免忘记忘记析构导致内存泄漏，我写了本模块。
 * 
 * 其中 AutoPtr 自带一个计数器，可以在生命周期结束时，即计数器降到 0 时，自动清空通过它申请的内存。
 * 如果把 AutoPtr 作为函数返回、或作为引用参数，也不用担心它自动清空。
 * 只有在程序彻底不需要它的时候，它才会自动清空。
 * 
 * 使用方法：
 * AutoPtr p(sizeof(char) * 10);
 * char *c = (char *)(p.Ptr());
 * // 存一些字符到c里
 * // 如果你要把申请的字符串返回，就这么使用：
 * return p;
 * // 不能直接 return c，不然 p 的生命周期到这里就结束了，它会在析构时，把之前申请的内存清空，即返回的 c 没有任何意义。
 * 
 * AutoPtr SendHttpRequest(string url) {
 * 	 ... // 发送 http 请求，得到 响应包的 body
 *   AutoPtr p(sizeof(body) + 1);
 *   memcpy(p.Ptr(), body, sizeof(body));
 *   return p;
 * }
 * 
 * // 外面的调用方法：
 * AutoPtr response = SendHttpRequest("https://www.baidu.com");
 * printf("%s\n", response.Ptr());
 * // 如果这时，外部函数结束，response 会自动析构它在 SendHttpRequest 函数内部 malloc 的内存。
 * // 其他使用方法，详见下面对借口的注释
 */
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "../hex/hex.h"
#include "../cjsonobject/CJsonObject.hpp"

// 自动析构的指针
class AutoPtr
{
public:
	// 得到一个空的 AutoPtr
	AutoPtr();
	// 得到一个长度为 size 的 AutoPtr，并且把申请的内存置 0。
	AutoPtr(size_t size);
	// 得到一个长度为 size+1 的内存，先置 0，然后把 p copy 到内存里。
	// size 是 p 的长度，不需要你多空 1 个字节，保护字符串。
	AutoPtr(void *p, size_t size);
	// 用 p 初始化，它们会共用一段内存和一个计数器，只要新创建的 AutoPtr 和 p 有一个还存在，内存就不会被清空。
	AutoPtr(const AutoPtr &p);

	// 赋值，会先把左值的计数器 - 1，如果计数器已是 0，则清空左值申请的内存，然后让左值和右值共享内存和计数器。
	void operator=(const AutoPtr& p);
	~AutoPtr();
	
	// 获取申请的内存
	void * Ptr();

	// 当前的记数器
	int Count();

	// 把申请的内存，转成 hex 字符串。
	// 有时，需要把一个结构体，通过 http，或函数调用，传给另一个程序，但传输接口不方便传送结构体，只方便传送 string，
	// 就把内存转成 hex，然后以字符串的格式传输，
	// 接收方则调用 FromString(std::string) 来恢复结构体。
    std::string ToString();
	// 从hex字符串恢复原来的内存，恢复的内存和原来的不再是同一个，初始计数器是 1。
    static AutoPtr FromString(std::string &s);

private:
	void Delete();
	void *_ptr;
	int *_count;
    int _size;

	// 禁止取地址，只能引用或复制。如果使用地址，AutoPtr 依然会在生命周期结束时析构。
	AutoPtr* operator &();
	// 禁止 new 和 delete。否则你依然必须记住应该在何时 delete 它。
	void* operator new(size_t size) throw();
	void operator delete(void *);
};

// 自动析构的指针列表
// 有时，一个结构体A当中会有一个成员变量 child ，是指向另一个结构体B的指针，那么，只传输A肯定是不够的，
// 这时，如果设置两个参数，比如同时传输 A 和 B，则会让函数参数变多。
// 所以增加一个自动析构的指针列表。
// AutoPtrList，列表中的第一个是A，第二个是B，可以这样使用：
/**
 * A *a = (A *)(auto_ptr_list.SetHead(sizeof(A)));
 * a->child = (B *)(auto_ptr_list.Add(sizeof(B)));
 * // 对 a，a->child 做一些其他的赋值。
 * 
 * 然后在返回时，返回 auto_ptr_list。
 * // 在另一个位置得到 a
 * A *a = (A *)(auto_ptr_list.Head());
 * // 得到 b
 * B *b = a->child;
 */
// 这样就不需要单独去维护每一个结构体。
class AutoPtrList
{
public:
	// 生成一个长度为 0 的智能指针列表
	AutoPtrList();
	~AutoPtrList();
	// 设置头指针，头指针可以通过 Head() 访问到，不需要是第一个设置。
	void *SetHead(AutoPtr &p);
	void *SetHead(size_t size);

	// 创建其他内存或指针。
	void *Add(AutoPtr &p);
	void *Add(size_t size);

	// 获取头指针
	void *Head();
    
	// 获取下标为 index 的指针，如果 index 超出范围，则返回 nullptr。
	void *Ptr(int index);

	// 从已存在的 AutoPtrList 初始化。
	AutoPtrList(const AutoPtrList &l);
	
	// 先清空自身列表中所有 AutoPtr，然后再从 l 初始化。
	void operator=(const AutoPtrList &l);

	// 把列表中的所有内存，转成 hex，生成一个 json 字符串。
    std::string ToString();

	// 从 json 恢复。
	// 注意，如果里面a有一个成员变量，指向另一段内存b，那么恢复的成员变量a2，依然指向之前的内存b，而不是恢复的内存b2。
	// 你必须自己把指针修改成新的地址，a2->child = b2。
    static AutoPtrList FromString(std::string &s);

private:
	std::vector<AutoPtr> _list;
	void *operator &();
	void *operator new(size_t size) throw();
	void operator delete(void *);
	void *_head;
};

#endif
