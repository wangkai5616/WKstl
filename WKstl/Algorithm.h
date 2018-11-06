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
	//�汾һ��ʹ��<���������Ƚϴ�С
	template<class T>
	const T& max(const T& a, const T& b) {
		return a < b ? b : a;
	}
	//�汾����ʹ�÷º���comp���жϴ�С
	template<class T,class Compare>
	const T& max(const T& a, const T& b,Compare comp) {
		return comp(a, b) ? b : a;
	}
	//*****************min******************************
	//�汾һ��ʹ��<���������Ƚϴ�С
	template<class T>
	const T& min(const T& a, const T& b) {
		return a < b ? a : b;
	}
	//�汾����ʹ�÷º���comp���жϴ�С
	template<class T, class Compare>
	const T& min(const T& a, const T& b, Compare comp) {
		return comp(a, b) ? a : b;
	}
	//*****************equal****************************
	//�汾һ��ʹ��!=���������ж��Ƿ����
	template<class InputIterator1,class InputIterator2>
	bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator1 first2) {
		for (; first1 != last1; ++first1, ++first2)
			if (*first1 != *first2)
				return false;
		return true;
	}
	//�汾����ʹ�÷º���pred���ж��Ƿ����
	template<class InputIterator1, class InputIterator2,class BinaryPredicate>
	bool equal(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2,BinaryPredicate binary_pred) {
			for (; first1 != last1; ++first1, ++first2)
				if(!binary_pred(first1,first2))
					return false;
			return true;
	}
	/******************mismatch***************************
	����������
	����ƽ�бȽ��������У�ָ������֮��ĵ�һ����ƥ��㣬����һ�Ե��������ֱ�ָ��
	�������еĲ�ƥ��㡣ע�⣺�������Ե�һ�Ե�����Ϊ��׼����������ڶ����е�Ԫ��
	�����ȵ�һ���ж࣬�������Ԫ�غ��Բ��ơ��෴����ᷢ��δ��Ԥ�ڵ���Ϊ��
	*/
	//�汾һ��ʹ��!=���������ж��Ƿ����
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
	//�汾����ʹ�÷º���pred���ж��Ƿ����
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
	��������;
	���������������������
	ע�⣺��utility�����ж�����
	template<class T>
	void swap(T& a,T& b){
		T tmp = a;
		a = b;
		b = tmp;
	}
	*/
	/*****************copy****************************
	�㷨������
	copy�㷨�ɽ���������[first,last)�ڵ�Ԫ�ظ��Ƶ��������[result,result+(last-first))
	�ڣ�����һ��������result+(last-first)��copy()�㷨������������ֻ����InputIterators
	���ɼ��ɣ��������ֻ����OutputIterator���ɼ���
	*/

	//�������ȫ������1������汾1������汾2����������
	//��ȫ������1
	template<class InputIterator,class OutputIterator>
	OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
		//__copy_dispatch<InputIterator, OutputIterator>()�������������������ʽ
		//Ȼ��������������()
		return __copy_dispatch<InputIterator, OutputIterator>()
			(first, last, result);
	}
	//����汾1���ڴ�ײ�������ٶȼ���
	char* copy(const char* first, const char* last, char* result) {
		memmove(result,first,sizeof(char)*(last-first));
		return result + (last - first);
	}
	//����汾2���ڴ�ײ�������ٶȼ���
	wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
		memmove(result, first, sizeof(wchar_t)*(last - first));
		return result + (last - first);
	}


	//��ȫ�����汾1.1������ȫ�����汾1������
	template<class InputIterator, class OutputIterator>
    struct __copy_dispatch
	{
		OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) {
			result __copy(first, last, result, iterator_category(first));
		}
	};
	//ƫ�ػ��汾1.2������ȫ�����汾1��������������ԭ��ָ��
	template<class T>
	struct __copy_dispatch<T*, T*>//<T*,T*>����˵����ƫ�ػ�
	{
		T* operator()(T* first, T* last, T* result) {
			typedef typename _type_traits<T>::has_trival_assignment_operator t;
			return __copy_t(first, last, result, t());
		}
	};
	//ƫ�ػ��汾1.3������ȫ�����汾1��������������ԭ��ָ��
	template<class T>
	struct __copy_dispatch<const T*, T*>//<T*,T*>����˵����ƫ�ػ�
	{
		T* operator()(const T* first, T* last, T* result) {
			typedef typename _type_traits<T>::has_trival_assignment_operator t;
			return __copy_t(first, last, result, t());
		}
	};


	//�汾1.1.1������ȫ�����汾1.1������,��Ե�������InputIterator
	template<class InputIterator,class OutputIterator>
	OutputIterator __copy(InputIterator first, InputIterator last,
		OutputIterator result, input_iterator_tag) {
		//�Ե�������ͬ��񣬾���ѭ���Ƿ�������ٶȽ���
		for (; first != last; ++first, ++result)
			*result = *first;
		return result;
	}
	//�汾1.1.2������ȫ�����汾1.1������,��Ե�������RandomAccessIterator
	template<class RandomAccessIterator,class OutputIterator>
	OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last,
		OutputIterator result, random_access_iterator_tag) {
		return __copy_d(first, last, result, difference_type(first));
	}
	template<class RandomAccessIterator,class OutputIterator,class Distance>
	OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last,
		OutputIterator result, Distance*) {//?????????????????????????????????????Ϊʲô����Distance
		for (Dostance n = last - first; n > 0; --n, ++first, ++result) {
			*result = *first;
		return result;
		}
	}

	//�汾1.2.1��1.3.1��ָ����ָ����߱�trival assignment operator*
	template<class T>
	T* __copy_t(const T* first, const T* last, T* result, _true_type) {
		memmove(result, first, sizeof(T)*(last - first));
		return result + (last - first);
	}
	//�汾1.2.2��1.3.2��ָ����ָ����߱�non-trival assignment operator*
	template<class T>
	T* __copy_t(const T* first, const T* last, T* result, _false_type) {
		return __copy_d(first,last,result,ptrdiff_t*)//������(ptrdiff_t*)0
	}
	///////////////////////////////////////////���������ݴ���//////////////////////////////////////////////////////////////////////////
	//�������е��㷨��ֻ�ǵ����������ƶ������Բ��ҡ�������ѭ����������һ��Ԫ��ʩ��ָ������Ȳ���
	/*****************adjant_find****************************
	�㷨������
	�ҳ�һ����������������Ԫ�ء���ν���������ڵ�һ�汾����ָ����Ԫ����ȡ������ڶ��汾
	�����û�ָ��һ����Ԫ���㣬�����������ֱ������ڵĵ�һԪ�غ͵ڶ�Ԫ��
	*/
	//�汾һ
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
	//�汾��
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
	�㷨������
	����==����������[first,last)�����ڵ�ÿһ��Ԫ��������ָ��ֵvalue�Ƚϣ������غ�value
	��ȵ�Ԫ�ظ���
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
	�㷨������
	��ָ��������һ���º�����predʵʩ��[first,last)�����ڵ�ÿһ���������ϣ����������pred
	֮������Ϊtrue��������Ԫ�صĸ�������
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
	�㷨������
	����equality��������ѭ������[first,last)�ڵ�����Ԫ�أ��ҳ���һ��ƥ�䡰��ͬ�������ߡ����
	�ҵ����ͷ���һ��InputIteratorָ���Ԫ�أ����򷵻ص�����last
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
	�㷨������
	�������������ڵķ���������Ԫ��
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
	�㷨������
	������һ[first,last1)�����ǵ������У��������ж�[first2,last2)�������һ�γ��ֵ㡣�������һ
	֮�ڲ����ڡ���ȫƥ�����ж����������У��㷵�ص�����last1�����㷨�������汾���汾һʹ��==������
	�����жϣ��汾�������û�ָ��ĳ����Ԫ���㣬��Ϊ�ж�Ԫ�������������
	*/
	//�汾һ
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
	//�汾��

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
	�㷨������
	������[first2,last2)�����ڵ�ĳЩԪ����Ϊ����Ŀ�꣬Ѱ��������[first1,last1)����
	�ڵĵ�һ�γ��ֵ�
	*/
	//�汾һ
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
	//�汾��
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
	�㷨������
	���º���fnʩ����[first,last)�����ڵ�ÿһ��Ԫ�����ϡ�fn�����Ըı�Ԫ�����ݣ���Ϊ
	first��last����InputIterator,�����ܸ�ֵ��Ϊ
	*/
	template<class InputIterator,class Function>
	
	Function for_each(InputIterator first, InputIterator last, Function fn) {
		for (; first != last; ++first)
			fn(*first);
		return fn;
	}
	/*****************generate****************************
	�㷨������
	���º���fn����������д����[first,last)�����ڵ�����Ԫ������
	*/
	template<class OutputIterator,class Function>
	void generate(OutputIterator first, OutputIterator last, Function fn) {
		for (; first != last; ++first)
			*first = gen();
	}
	/*****************generate_n****************************
	�㷨������
	���º���fn����������д����first��ʼ��n��Ԫ������
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
	�㷨������
	����һ���󶥶�
	*/
	template<class RandomAccessIterator,class Compare>
	//heap�����㷨
	//�����first,last,head���ǵ�������head��������λ�ã�Ҳ��i��vector�е�һ��Ԫ�ص�λ��
	static void up(RandomAccessIterator first, RandomAccessIterator last,
		RandomAccessIterator head, Compare comp) {
		if (first != last) {
			//index������Ǿ��룬��������indexԶ�Ľڵ�Ԫ�����丸�ڵ���бȽ�
			int index = last - head;
			//���ڵ���������ľ���
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
	//heap�½��㷨
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
				cur = child;//ʵ�ʵĵ�����ָ��
				index = cur - head;//����
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
	�㷨������
	��󶥶��в���Ԫ��
	*/
	template<class RandomAccessIterator>
	void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
		WKstl::push_heap(first, last,
			WKstl::less<typename WKstl::iterator_traits<RandomAccessIterator>::value_type>());
	}
	template<class RandomAccessIterator,class Compare>
	void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		WKstl::up(first, last - 1, first, comp);//last�в�����Ԫ�أ����������ұ�
	}
	/*****************pop_heap****************************
	�㷨������
	�󶥶������Ԫ�ص�ȡ��
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
	�㷨������
	���󶥶��е�Ԫ������pop,����һ������������vector
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
	�㷨������
	�ж��Ƿ�Ϊ�󶥶�
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
			//�����Һ��Ӻ͸��ڵ���бȽϣ����Ƿ�����󶥶ѵ�����
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
	�㷨������

	*/
	/*****************count****************************
	�㷨������

	*/
	/*****************count****************************
	�㷨������

	*/
	/*****************count****************************
	�㷨������

	/*****************count****************************
	�㷨������

	/*****************count****************************
	�㷨������

	/*****************count****************************
	�㷨������

	/*****************count****************************
	�㷨������

	*/
	
}
/*
1.������������������������
ͬһ������������������α���ͬһ������
ͬһ������������������α���ͬһ������
*/
#endif