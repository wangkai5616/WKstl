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
���Է�ֹ�����ض������
*
#include <new>
/*
����#include <new>�Ľ���
��new��ͷ�ļ���ֻ����6�����ص�new��delete����һЩnew��������iostream��C++���ض����������
�������Լ�һЩ�������ú����Ͳ�������������������ʵ���ǰ�����newͷ�ļ������ݣ�һ��ֻ��Ҫ��
C++����ͷ����"#include<iostream>"�� using namespace std;���������Ϳ����ˡ�����Ҫ���������new��
������C++�а���iostreamͷ�ļ���������Щ΢���.h�ĺͲ���.h��iostream��ʹ��ʱҲ�ǲ�һ�µģ�
��.h��һ������������ȫ�ֵģ�����ֱ�����ã����ǲ���.h������������std���ֿռ䡣����ֱ��ʹ��
����.h��iostream��ʹ�ñ�׼���ֿռ䡣
*/
/*
allocator��װ��Alloc�����������˹����������������Vector��list����ʹ��
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
		//ptrdiff_t��C/C++��׼���ж����һ���������ص��������͡�ptrdiff_t���ͱ���ͨ��������������ָ����������Ľ����
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

	//������Щ�����ڴ���ջ��ڴ�ĺ�����ʵ��������һ�·�װ
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

	//ע��,static�������ⶨ�岻��дstatic��ֻ��������ʱ��д
	template<class T>
	void allocator<T>::construct(T *ptr) {
		new(ptr)T();//placement new;����T::T(),��ָ��ptr��ָ���ڴ�ռ䴴��һ������ΪT�Ķ���
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
��̬������
�ں�����������ǰ�ӹؼ���static�������Ͷ���ɾ�̬�����������Ķ����������Ĭ�����
�¶���extern�ģ�����̬����ֻ�������������ļ����пɼ������ܱ������ļ����á�
*/
#endif