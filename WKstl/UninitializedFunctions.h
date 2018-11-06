#ifndef _UNINITIALIZED_FUNCTIONS_H_
#define _UNINITIALIZED_FUNCTIONS_H_

#include "Algorithm.h"
#include "Construct.h"
#include "Iterator.h"
#include "TypeTraits.h"

/*
STL定义有5个全局函数，作用于未初始化空间上。这样的功能对于容器的实现很有帮助
construct() destroy() uninitialized_copy()  uninitialized_fill()  uninitialized_fill_n()
*/
namespace WKstl {

	/***************************************************************************/
	/*
	uninitialized_copy()使我们能够将内存的配置与对象的构造行为分离开，作为输出目的地
	[result,result+(last-first))范围内的每一个迭代器都指向未初始化区域，则
	uninitialized_copy()会使用copy constructor，给[first,last)范围内的每一个对象产生一份
	复制品，放入输出范围中
	*/
	template<class InputIterator, class ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
		ForwardIterator result, _true_type);
	template<class InputIterator, class ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
		ForwardIterator result, _false_type);

	template<class InputIterator, class ForwardIterator>
	ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
		typedef typename _type_traits<iterator_traits<InputIterator>::value_type>::is_POD_type isPODType;
		return _uninitialized_copy_aux(first, last, result, isPODType());
	}
	template<class InputIterator, class ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
		ForwardIterator result, _true_type) {
		/*
		原型：void*memcpy(void*dest, const void*src,unsigned int count);
		功能：由src所指内存区域复制count个字节到dest所指内存区域。
		*/
		memcpy(result, first, (last - first) * sizeof(*first));
		return result + (last - first);
	}
	template<class InputIterator, class ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
		ForwardIterator result, _false_type) {
		int i = 0;
		for (; first != last; ++first, ++i) {
			construct((result + i), *first);
		}
		return (result + i);
	}

	/***************************************************************************/
	/*
	uninitialized_fill()能够使内存配置和对象的构造行为分开，如果[first,last)范围内
	的每个迭代器都指向未初始化的内存，那么uninitialized_fill()会在该范围内产生x
	的复制品
	也就是说[first,lasst)之间的内存配置好了，这个函数往里面构造对象
	*/
	template<class ForwardIterator, class T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
		const T& value, _true_type);
	template<class ForwardIterator, class T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
		const T& value, _false_type);

	template<class ForwardIterator, class T>
	void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value) {
		typedef typename _type_traits<T>::is_POD_type isPODType;
		_uninitialized_fill_aux(first, last, value, isPODType());
	}
	template<class ForwardIterator, class T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
		const T& value, _true_type) {
		fill(first, last, value);
	}
	template<class ForwardIterator, class T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,
		const T& value, _false_type) {
		for (; first != last; ++first) {
			construct(first, value);
		}
	}

	/***************************************************************************/
	/*
	uninitialized_fill_n()能够使内存配置和对象的构造行为分开，会为指定范围
	[first,first+n)内的所有元素设定相同的值
	也就是说[first,first+n)之间的内存配置好了，这个函数往里面构造对象
	*/
	template<class ForwardIterator, class Size, class T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first,
		Size n, const T& x, _true_type);
	template<class ForwardIterator, class Size, class T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first,
		Size n, const T& x, _false_type);

	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator uninitialized_fill_n(ForwardIterator first,
		Size n, const T& x) {
		typedef typename _type_traits<T>::is_POD_type isPODType;
		return _uninitialized_fill_n_aux(first, n, x, isPODType());
	}
	template<class ForwardIterator, class Size, class T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first,
		Size n, const T& x, _true_type) {
		return fill_n(first, n, x);
	}
	template<class ForwardIterator, class Size, class T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first,
		Size n, const T& x, _false_type) {
		int i = 0;
		for (; i != n; ++i) {
			construct((T*)(first + i), x);
		}
		return (first + i);
	}
}

#endif
