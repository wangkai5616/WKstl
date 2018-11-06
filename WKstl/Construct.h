#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_
#include <new>
#include "TypeTraits.h"
namespace WKstl {
	template <class T1,class T2>
	//负责对象的构造
	inline void construct(T1 *ptr1, const T2& value) {
		new (ptr1) T1(value); //placement new; 调用T::T(value), 在指针ptr所指的内
		                      //存空间创建一个类型为T1的对象,对象值为value
	}
	//负责对象的析构
	//destroy()的第一个版本，接受一个指针
	template <class T>
	inline void destroy(T *ptr) {
		ptr->~T();
	}
	/*
	destroy()的第二个版本，接受两个迭代器。此函数设法找出元素的数值型别，
	进而利用_type_traits<>求取最适当措施
	*/
	template <class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last) {
		_destroy(first, last, value_type(first));//回返型别，这里指的是迭代器所指对象的型别（找出来的是类型T *）
	}
	//trivial_destructor()用于判断元素的数值型别是trivial dtor还是non-trivial dtor
	template <class ForwardIterator,class T>
	inline void _destroy(ForwardIterator first, ForwardIterator last, T*) {
		typedef typename _type_traits<T>::has_trivial_destructor trivial_destructor
			__destroy(first, last, trivial_destructor());
	}
	//如果元素的数值型别具备trivial dtor(不重要的析构函数)，采用内存直接处理操作
	template <class ForwardIterator>
	inline void __destroy(ForwardIterator first, ForwardIterator last, _true_type) {}
	//如果型别的数值型别具备non-trivial dtor(重要的析构函数)
	template<class ForwardIterator>
	inline void __destroy(ForwardIterator first, ForwardIterator last, _false_type) {
		for (; first < last; ++first)
			destroy(&*first);//调用第一个版本的析构操作，为什么有这样形式的书写博客里有答案
	}
}
/*
知识点：
内联函数在编译的时候将不进行函数调用，编译器将内联函数的代码粘贴在调用（形式上调用）处，可以提高效率。
内联函数只能是代码很少很简单的函数，因为如果一个很大很复杂的函数即使设为内联，编译器也将自动设置该函数为非内联。
*/
#endif
