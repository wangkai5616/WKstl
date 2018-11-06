#ifndef _ALLCATOR_H_
#define _ALLCATOR_H_
#include "Alloc.h"
#include "Construct.h"
#include <cassert>
#include <new>
/*
#ifndef _ALLCATOR_H_
#define _ALLCATOR_H_
#endif
可以防止这种重定义错误
*
#include <new>
/*
对于#include <new>的解释
在new的头文件中只包含6个重载的new和delete，和一些new的声明。iostream是C++中特定的输入输出
流操作以及一些其他常用函数和操作符的声明。里面其实就是包含了new头文件的内容，一般只需要在
C++程序开头加上"#include<iostream>"和 using namespace std;这两句代码就可以了。不需要再另外加上new。
另外在C++中包含iostream头文件的做法有些微妙：带.h的和不带.h的iostream在使用时也是不一致的，
带.h的一般作用域属于全局的，可以直接引用，但是不带.h的作用域属于std名字空间。建议直接使用
不带.h的iostream并使用标准名字空间。
*/
/*
allocator封装了Alloc，另外增加了构造和析构函数，供Vector、list等类使用
*/
namespace WKstl
{
	template <class T>
	class allocator {
	public:
		typedef T	value_type;
		typedef T*	pointer;
		typedef const T*	const_pointer;
		typedef T&	reference;
		typedef const T&	const_reference;
		typedef size_t	size_type;
		//ptrdiff_t是C/C++标准库中定义的一个与机器相关的数据类型。ptrdiff_t类型变量通常用来保存两个指针减法操作的结果。
		typedef ptrdiff_t	difference_type;
	public:
		static T *allocate();
		static T *allocate(size_t n);
		static void deallocate(T *ptr);
		static void deallocate(T *ptr, size_t n);

		static void construct(T *ptr);
		static void construct(T *ptr, const T& value);
		static void destroy(T *ptr);
		static void destroy(T *first, T *last);
	};

	//下面这些分配内存和收回内存的函数其实就是做了一下封装
	template<class T>
	T *allocator<T>::allocate() {
		return static_cast<T *>(alloc::allocate(sizeof(T)));
	}
	template<class T>
	T *allocator<T>::allocate(size_t n) {
		if (n == 0)
			return 0;
		return static_cast<T *>(alloc::allocate(sizeof(T)*n));
	}
	template<class T>
	void allocator<T>::deallocate(T *ptr) {
		alloc::deallocate(static_cast<void *>(ptr), sizeof(T));
	}
	template<class T>
	void allocator<T>::deallocate(T *ptr,size_t n) {
		if (n == 0)
			return;
		alloc::deallocate(static_cast<void *>(ptr), sizeof(T)*n);
	}

	//注意,static函数类外定义不能写static，只在声明的时候写
	template<class T>
	void allocator<T>::construct(T *ptr) {
		new(ptr)T();//placement new;调用T::T(),在指针ptr所指的内存空间创建一个类型为T的对象
	}
	template<class T>
	void allocator<T>::construct(T *ptr, const T& value) {
		new (ptr)T(value);
	}
	template<class T>
	void allocator<T>::destroy(T *ptr) {
		ptr->~T();
	}
	template<class T>
	void allocator<T>::destroy(T *first, T *last) {
		for (; first != last; ++first) {
			first->~T();
		}
	}
}
/*
静态函数：
在函数返回类型前加关键字static，函数就定义成静态函数。函数的定义和生命在默认情况
下都是extern的，但静态函数只是在声明他的文件当中可见，不能被其他文件所用。
*/
#endif