#ifndef _UNINITIALIZED_FUNCTIONS_H_
#define _UNINITIALIZED_FUNCTIONS_H_

#include "Algorithm.h"
#include "Construct.h"
#include "Iterator.h"
#include "TypeTraits.h"

/*
STL������5��ȫ�ֺ�����������δ��ʼ���ռ��ϡ������Ĺ��ܶ���������ʵ�ֺ��а���
construct() destroy() uninitialized_copy()  uninitialized_fill()  uninitialized_fill_n()
*/
namespace WKstl {

	/***************************************************************************/
	/*
	uninitialized_copy()ʹ�����ܹ����ڴ�����������Ĺ�����Ϊ���뿪����Ϊ���Ŀ�ĵ�
	[result,result+(last-first))��Χ�ڵ�ÿһ����������ָ��δ��ʼ��������
	uninitialized_copy()��ʹ��copy constructor����[first,last)��Χ�ڵ�ÿһ���������һ��
	����Ʒ�����������Χ��
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
		ԭ�ͣ�void*memcpy(void*dest, const void*src,unsigned int count);
		���ܣ���src��ָ�ڴ�������count���ֽڵ�dest��ָ�ڴ�����
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
	uninitialized_fill()�ܹ�ʹ�ڴ����úͶ���Ĺ�����Ϊ�ֿ������[first,last)��Χ��
	��ÿ����������ָ��δ��ʼ�����ڴ棬��ôuninitialized_fill()���ڸ÷�Χ�ڲ���x
	�ĸ���Ʒ
	Ҳ����˵[first,lasst)֮����ڴ����ú��ˣ�������������湹�����
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
	uninitialized_fill_n()�ܹ�ʹ�ڴ����úͶ���Ĺ�����Ϊ�ֿ�����Ϊָ����Χ
	[first,first+n)�ڵ�����Ԫ���趨��ͬ��ֵ
	Ҳ����˵[first,first+n)֮����ڴ����ú��ˣ�������������湹�����
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
