#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_
#include <new>
#include "TypeTraits.h"
namespace WKstl {
	template <class T1,class T2>
	//�������Ĺ���
	inline void construct(T1 *ptr1, const T2& value) {
		new (ptr1) T1(value); //placement new; ����T::T(value), ��ָ��ptr��ָ����
		                      //��ռ䴴��һ������ΪT1�Ķ���,����ֵΪvalue
	}
	//������������
	//destroy()�ĵ�һ���汾������һ��ָ��
	template <class T>
	inline void destroy(T *ptr) {
		ptr->~T();
	}
	/*
	destroy()�ĵڶ����汾�������������������˺����跨�ҳ�Ԫ�ص���ֵ�ͱ�
	��������_type_traits<>��ȡ���ʵ���ʩ
	*/
	template <class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last) {
		_destroy(first, last, value_type(first));//�ط��ͱ�����ָ���ǵ�������ָ������ͱ��ҳ�����������T *��
	}
	//trivial_destructor()�����ж�Ԫ�ص���ֵ�ͱ���trivial dtor����non-trivial dtor
	template <class ForwardIterator,class T>
	inline void _destroy(ForwardIterator first, ForwardIterator last, T*) {
		typedef typename _type_traits<T>::has_trivial_destructor trivial_destructor
			__destroy(first, last, trivial_destructor());
	}
	//���Ԫ�ص���ֵ�ͱ�߱�trivial dtor(����Ҫ����������)�������ڴ�ֱ�Ӵ������
	template <class ForwardIterator>
	inline void __destroy(ForwardIterator first, ForwardIterator last, _true_type) {}
	//����ͱ����ֵ�ͱ�߱�non-trivial dtor(��Ҫ����������)
	template<class ForwardIterator>
	inline void __destroy(ForwardIterator first, ForwardIterator last, _false_type) {
		for (; first < last; ++first)
			destroy(&*first);//���õ�һ���汾������������Ϊʲô��������ʽ����д�������д�
	}
}
/*
֪ʶ�㣺
���������ڱ����ʱ�򽫲����к������ã������������������Ĵ���ճ���ڵ��ã���ʽ�ϵ��ã������������Ч�ʡ�
��������ֻ���Ǵ�����ٺܼ򵥵ĺ�������Ϊ���һ���ܴ�ܸ��ӵĺ�����ʹ��Ϊ������������Ҳ���Զ����øú���Ϊ��������
*/
#endif
