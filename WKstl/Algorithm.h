#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include <cstring>
#include<utility>

#include "Allocator.h"
#include "Iterator.h"
#include "TypeTraits.h"
#include "Utility.h"
#include "Functional.h"
namespace WKstl {
	//*****************fill******************************
	template<class ForwardIterator,class T>
	void fill(ForwardIterator first, ForwardIterator last, const T& value) {
		for (; first != last; ++first)
			*first = value;
	}
	//*****************fill_n****************************
	template<class OutputIterator,class Size,class T>
	OutputIterator fill_n(OutputIterator first, Size n, const T& value) {
		for (; n > 0; --n, ++first)
			*first = value;
		return first;
	}
	//*****************max****************************
	//版本一：使用<操作符来比较大小
	template<class T>
	const T& max(const T& a, const T& b) {
		return a < b ? b : a;
	}
	//版本二：使用仿函数comp来判断大小
	template<class T,class Compare>
	const T& max(const T& a, const T& b,Compare comp) {
		return comp(a, b) ? b : a;
	}
	//*****************min******************************
	//版本一：使用<操作符来比较大小
	template<class T>
	const T& min(const T& a, const T& b) {
		return a < b ? a : b;
	}
	//版本二：使用仿函数comp来判断大小
	template<class T, class Compare>
	const T& min(const T& a, const T& b, Compare comp) {
		return comp(a, b) ? a : b;
	}
	//*****************equal****************************
	//版本一：使用!=操作符来判断是否相等
	template<class InputIterator1,class InputIterator2>
	bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator1 first2) {
		for (; first1 != last1; ++first1, ++first2)
			if (*first1 != *first2)
				return false;
		return true;
	}
	//版本二：使用仿函数pred来判断是否相等
	template<class InputIterator1, class InputIterator2,class BinaryPredicate>
	bool equal(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2,BinaryPredicate binary_pred) {
			for (; first1 != last1; ++first1, ++first2)
				if(!binary_pred(first1,first2))
					return false;
			return true;
	}
	/******************mismatch***************************
	功能描述：
	用来平行比较两个序列，指出两者之间的第一个不匹配点，返回一对迭代器，分别指向
	两序列中的不匹配点。注意：这里是以第一对迭代器为基准，所以如果第二序列的元素
	个数比第一序列多，多出来的元素忽略不计。相反，则会发生未可预期的行为。
	*/
	//版本一：使用!=操作符来判断是否相等
	template<class InputIterator1, class InputIterator2>
	pair<InputIterator1, InputIterator2> mismatch(
		InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2) {
			for (; first1 != last1; ++first1, ++first2) {
				if (*first != *last1)
					break;
			}
			return pair<InputIterator1, InputIterator2>(first1, first2);
	}
	//版本二：使用仿函数pred来判断是否相等
	template<class InputIterator1, class InputIterator2,class BinaryPredicate>
	pair<InputIterator1, InputIterator2> mismatch(
		InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2,BinaryPredicate pred) {
		for (; first1 != last1; ++first1, ++first2) {
			if(!pred(*first1,first2))
				break;
		}
		return pair<InputIterator1, InputIterator2>(first1, first2);
	}
	/*****************swawp****************************
	功能描述;
	用来交换两个对象的内容
	注意：在utility中已有定义了
	template<class T>
	void swap(T& a,T& b){
		T tmp = a;
		a = b;
		b = tmp;
	}
	*/
	/*****************copy****************************
	算法描述：
	copy算法可将输入区间[first,last)内的元素复制到输出区间[result,result+(last-first))
	内，返回一个迭代器result+(last-first)。copy()算法对其输入区间只需由InputIterators
	构成即可，输出区间只需由OutputIterator构成即可
	*/

	//下面的完全泛化版1，特殊版本1，特殊版本2构成了重载
	//完全泛化版1
	template<class InputIterator,class OutputIterator>
	OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
		//__copy_dispatch<InputIterator, OutputIterator>()这是匿名函数对象的形式
		//然后调用重载运算符()
		return __copy_dispatch<InputIterator, OutputIterator>()
			(first, last, result);
	}
	//特殊版本1，内存底层操作，速度极快
	char* copy(const char* first, const char* last, char* result) {
		memmove(result,first,sizeof(char)*(last-first));
		return result + (last - first);
	}
	//特殊版本2，内存底层操作，速度极快
	wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
		memmove(result, first, sizeof(wchar_t)*(last - first));
		return result + (last - first);
	}


	//完全泛化版本1.1（由完全泛化版本1得来）
	template<class InputIterator, class OutputIterator>
    struct __copy_dispatch
	{
		OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) {
			result __copy(first, last, result, iterator_category(first));
		}
	};
	//偏特化版本1.2（由完全泛化版本1得来），考虑了原生指针
	template<class T>
	struct __copy_dispatch<T*, T*>//<T*,T*>用来说明是偏特化
	{
		T* operator()(T* first, T* last, T* result) {
			typedef typename _type_traits<T>::has_trival_assignment_operator t;
			return __copy_t(first, last, result, t());
		}
	};
	//偏特化版本1.3（由完全泛化版本1得来），考虑了原生指针
	template<class T>
	struct __copy_dispatch<const T*, T*>//<T*,T*>用来说明是偏特化
	{
		T* operator()(const T* first, T* last, T* result) {
			typedef typename _type_traits<T>::has_trival_assignment_operator t;
			return __copy_t(first, last, result, t());
		}
	};


	//版本1.1.1（由完全泛化版本1.1得来）,针对迭代器是InputIterator
	template<class InputIterator,class OutputIterator>
	OutputIterator __copy(InputIterator first, InputIterator last,
		OutputIterator result, input_iterator_tag) {
		//以迭代器相同与否，决定循环是否继续，速度较慢
		for (; first != last; ++first, ++result)
			*result = *first;
		return result;
	}
	//版本1.1.2（由完全泛化版本1.1得来）,针对迭代器是RandomAccessIterator
	template<class RandomAccessIterator,class OutputIterator>
	OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last,
		OutputIterator result, random_access_iterator_tag) {
		return __copy_d(first, last, result, difference_type(first));
	}
	template<class RandomAccessIterator,class OutputIterator,class Distance>
	OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last,
		OutputIterator result, Distance*) {//?????????????????????????????????????为什么不是Distance
		for (Dostance n = last - first; n > 0; --n, ++first, ++result) {
			*result = *first;
		return result;
		}
	}

	//版本1.2.1、1.3.1，指针所指对象具备trival assignment operator*
	template<class T>
	T* __copy_t(const T* first, const T* last, T* result, _true_type) {
		memmove(result, first, sizeof(T)*(last - first));
		return result + (last - first);
	}
	//版本1.2.2、1.3.2，指针所指对象具备non-trival assignment operator*
	template<class T>
	T* __copy_t(const T* first, const T* last, T* result, _false_type) {
		return __copy_d(first,last,result,ptrdiff_t*)//书上是(ptrdiff_t*)0
	}
	///////////////////////////////////////////单纯的数据处理//////////////////////////////////////////////////////////////////////////
	//下面所列的算法，只是单纯的数据移动、线性查找、计数、循环遍历、逐一对元素施行指定运算等操作
	/*****************adjant_find****************************
	算法描述：
	找出一组满足条件的相邻元素。所谓的条件：在第一版本中是指“两元素相等”，而第二版本
	允许用户指定一个二元运算，两个操作数分别是相邻的第一元素和第二元素
	*/
	//版本一
	template <class ForwardIterator>
	ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last) {
		if (first == last) return last;
		ForwardIterator next = first;
		while (++next != last) {
			if (*first == *next)
				return first;
			first = next;
		}
		return last;
	}
	//版本二
	template <class ForwardIterator,class BinaryPredicate>
	ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last,
		BinaryPredicate binary_pred) {
		if (first == last) return last;
		ForwardIterator next = first;
		while (++next != last) {
			if (binary_pred(*first,*last))
				return first;
			first = next;
		}
		return last;
	}
	/*****************count****************************
	算法描述：
	运用==操作符，将[first,last)区间内的每一个元素拿来和指定值value比较，并返回和value
	相等的元素个数
	*/
	template<class InputIterator,class T>
	typename iterator_traits<InputIterator>::difference_type
	count(InputIterator first,InputIterator last,const T& value)
	{
		typename iterator_traits<InputIterator>::difference_type n = 0;
		for (; first != last; ++first) {
			++n;
		}
		return n;
	}
	/*****************count_if****************************
	算法描述：
	将指定操作（一个仿函数）pred实施与[first,last)区间内的每一个函数身上，并将“造成pred
	之计算结果为true”的所有元素的个数返回
	*/
	template<class InputIterator, class Predicate>
	typename iterator_traits<InputIterator>::difference_type
		count(InputIterator first, InputIterator last, Predicate pred)
	{
		typename iterator_traits<InputIterator>::difference_type n = 0;
		for (; first != last; ++first) {
			if (pred(*first))
				++n;
		}
		return n;
	}
	/*****************find****************************
	算法描述：
	根据equality操作符，循环查找[first,last)内的所有元素，找出第一个匹配“等同条件”者。如果
	找到，就返回一个InputIterator指向该元素，否则返回迭代器last
	*/
	template<class InputIterator,class Predicate>
	InputIterator find(InputIterator first, InputIterator last, Predicate pred) {
		for (; first != last; ++first) {
			if (pred(*first))
				++first;
		}
		return first;
	}
	/*****************find_if****************************
	算法描述：
	用来查找容器内的符合条件的元素
	*/
	template<class InputIteartor,class UnaryPredicate>
	InputIteartor find_if(InputIteartor first, InputIteartor last, UnaryPredicate pred) {
		for (; first != last; ++first) {
			if (pred(*first))
				break;
		}
		return first;
	}
	/*****************find_end****************************
	算法描述：
	在序列一[first,last1)所涵盖的区间中，查找序列二[first2,last2)的在最后一次出现点。如果序列一
	之内不存在“完全匹配序列二”的子序列，便返回迭代器last1。此算法有两个版本，版本一使用==操作符
	进行判断，版本二允许用户指定某个二元运算，作为判断元素相等与否的依据
	*/
	//版本一
	template<class ForwardIterator1,class ForwardIterator2>
	ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1, 
		ForwardIterator2 first2, ForwardIterator2 last2) {
		if (first2 == last2)
			return last1;
		ForwardIterator1 ret = last1;
		while (first1 != last1) {
			ForwardIterator1 it1 = first1;
			ForwardIterator2 it2 = first2;
			while (*it1 == *it2) {
				++it1, ++it2;
				if (it2 == last2) {
					ret = first1;
					break;
				}
				if (it1 == last1)
					return ret;
			}
			++first1;
		}
		return ret;
	}
	//版本二

	template<class ForwardIterator1, class ForwardIterator2,class BinaryPredicate>
	ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1, 
		ForwardIterator2 first2, ForwardIterator2 last2,
		BinaryPredicate pred) {
		if (first2 == last2)
			return last1;
		ForwardIterator1 ret = last1;
		while (first1 != last1) {
			ForwardIterator1 it1 = first1;
			ForwardIterator2 it2 = first2;
			while (pred(*it1,*it2)) {
				++it1, ++it2;
				if (it2 == last2) {
					ret = first1;
					break;
				}
				if (it1 == last1)
					return ret;
			}
			++first1;
		}
		return ret;
	}
	/***************find_first_of****************************
	算法描述：
	以区间[first2,last2)区间内的某些元素作为查找目标，寻找它们在[first1,last1)区间
	内的第一次出现点
	*/
	//版本一
	template <class InputIterator,class ForwardIterator>
	InputIterator find_first_of(InputIterator first1, InputIterator last1,
		ForwardIterator first2, ForwardIterator last2) {
		for (; first1 != last1; ++first1)
			for (ForwardIterator it = first2; it != last2; ++it) {
				if (*first1 == *it)
					return first1;
			}
		return last1;
	}
	//版本二
	template <class InputIterator, class ForwardIterator,class BinaryPredicate>
	InputIterator find_first_of(InputIterator first1, InputIterator last1,
		ForwardIterator first2, ForwardIterator last2, BinaryPredicate pred) {
		for (; first1 != last1; ++first1)
			for (ForwardIterator it = first2; it != last2; ++it) {
				if (pred(*first1,*it))
					return first1;
			}
		return last1;
	}
	/*****************for_each****************************
	算法描述：
	将仿函数fn施行于[first,last)区间内的每一个元素身上。fn不可以改变元素内容，因为
	first和last都是InputIterator,不接受赋值行为
	*/
	template<class InputIterator,class Function>
	
	Function for_each(InputIterator first, InputIterator last, Function fn) {
		for (; first != last; ++first)
			fn(*first);
		return fn;
	}
	/*****************generate****************************
	算法描述：
	将仿函数fn的运算结果填写的在[first,last)区间内的所有元素身上
	*/
	template<class OutputIterator,class Function>
	void generate(OutputIterator first, OutputIterator last, Function fn) {
		for (; first != last; ++first)
			*first = gen();
	}
	/*****************generate_n****************************
	算法描述：
	将仿函数fn的运算结果填写的在first开始的n个元素身上
	*/
	template<class OutputIterator,class Size, class Function>
	void generate(OutputIterator first, Size n, Function fn) {
		for (; n > 0; --n, ++first)
			*first = gen();
	}
	//***************distance******************************
	
	template<class InputIterator>
	typename iterator_traits<InputIterator>::difference_type
		_distance(InputIterator first, InputIterator last, input_iterator_tag) {
		typename iterator_traits<InputIterator>::difference_type dist = 0;
		while (first++ != last) {
			++dist;
		}
		return dist;
	}
	template<class RandomIterator>
	typename iterator_traits<RandomIterator>::difference_type
		_distance(RandomIterator first, RandomIterator last, random_access_iterator_tag) {
		auto dist = last - first;
		return dist;
	}
	template<class Iterator>
	typename iterator_traits<Iterator>::difference_type
		distance(Iterator first, Iterator last) {
		typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
		return _distance(first, last, iterator_category());
	}
	/*****************make_heap****************************
	算法描述：
	构建一个大顶堆
	*/
	template<class RandomAccessIterator,class Compare>
	//heap上溯算法
	//这里的first,last,head都是迭代器，head是树根的位置，也就i是vector中第一个元素的位置
	static void up(RandomAccessIterator first, RandomAccessIterator last,
		RandomAccessIterator head, Compare comp) {
		if (first != last) {
			//index代表的是距离，距离树根index远的节点元素与其父节点进行比较
			int index = last - head;
			//父节点距离树根的距离
			auto parentIndex = (index - 1) / 2;
			for (auto cur = last; parentIndex >= 0 && cur != head; parentIndex = (index - 1) / 2) {
				auto parent = head + parentIndex;
				if (comp(*parent, *cur))
					WKstl::swap(*parent, *cur);
				cur = parent;
				index = cur - head;
			}
		}
	}

	template<class RandomAccessIterator,class Compare>
	//heap下降算法
	static void down(RandomAccessIterator first, RandomAccessIterator last,
		RandomAccessIterator head, Compare comp) {
		if (first != last) {
			auto index = first - head;
			auto leftChildIndex = index * 2 + 1;
			for (auto cur = first; leftChildIndex < (last - head + 1) && cur < last; leftChildIndex = index * 2 + 1) {
				auto child = head + leftChildIndex;
				if ((child + 1) <= last && *(child + 1) > *child)
					child = child + 1;
				if (comp(*cur, *child))
					WKstl::swap(*cur, *child);
				cur = child;//实际的迭代器指向
				index = cur - head;//距离
			}
		}
	}
	template<class RandomAccessIterator>
	void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
		WKstl::make_heap(first, last,
			typename WKstl::less<WKstl::iterator_traits<RandomAccessIterator>::value_type>());
	}
	template<class RandomAccessIterator,class Compare>
	void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		const auto range = last - first;
		for (auto cur = first + range / 2 - 1; cur >= first; --cur) {
			WKstl::down(cur, last - 1, first, comp);
			if (cur == first) return;
		}
	} 
	/*****************push_heap****************************
	算法描述：
	向大顶堆中插入元素
	*/
	template<class RandomAccessIterator>
	void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
		WKstl::push_heap(first, last,
			WKstl::less<typename WKstl::iterator_traits<RandomAccessIterator>::value_type>());
	}
	template<class RandomAccessIterator,class Compare>
	void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		WKstl::up(first, last - 1, first, comp);//last中并不放元素，迭代器左开右闭
	}
	/*****************pop_heap****************************
	算法描述：
	大顶堆中最大元素的取出
	*/
	template<class RandomAccessIterator>
	void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
		WKstl::pop_heap(first, last,
			WKstl::less<typename WKstl::iterator_traits<RandomAccessIterator>::value_type>());
	}
	template<class RandomAccessIterator,class Compare>
	void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		WKstl::swwap(*first, *(last - 1));
		if (last - first >= 2)
			WKstl::down(first, last - 2, first, comp);
	}
	/*****************sort_heap****************************
	算法描述：
	将大顶堆中的元素依次pop,构成一个单调递增的vector
	*/
	template<class RandomAccessIterator>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
		return WKstl::sort_heap(first, last,
			WKstl::less<typename WKstl::iterator_traits<RandomAccessIterator>::value_type>());
	}
	template<class RandomAccessIterator,class Compare>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		for (auto cur = last; cur != first; --cur) {
			WKstl::pop_heap(first, cur, comp);
		}
	}
	/*****************is_heap****************************
	算法描述：
	判断是否为大顶堆
	*/
	template <class RandomAccessIterator>
	bool is_heap(RandomAccessIterator first, RandomAccessIterator last) {
		return TinySTL::is_heap(first, last,
			TinySTL::less<typename TinySTL::iterator_traits<RandomAccessIterator>::value_type>());
	}
	template<class RandomAccessIterator,class Compare>
	bool is_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		const auto range = last - first;
		auto index = range / 2 - 1;
		for (auto cur = first + range / 2 - 1; cur >= first; --cur, --index) {
			//对左右孩子和父节点进行比较，看是否满足大顶堆的性质
			if (*(first + (index * 2 + 1)) > *cur ||
				((first + (index * 2 + 2)) <= last && *(first + (index * 2 + 2)) > *cur)) {
				return false;
			}
			if (cur == first)
				break;
		}
		return true;
	}
	/*****************count****************************
	算法描述：

	*/
	/*****************count****************************
	算法描述：

	*/
	/*****************count****************************
	算法描述：

	*/
	/*****************count****************************
	算法描述：

	/*****************count****************************
	算法描述：

	/*****************count****************************
	算法描述：

	/*****************count****************************
	算法描述：

	/*****************count****************************
	算法描述：

	*/
	
}
/*
1.关于输入迭代器、输出迭代器
同一个输入迭代器不能两次遍历同一个序列
同一个输出迭代器不能两次遍历同一个序列
*/
#endif