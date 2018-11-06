#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <algorithm>
#include <type_traits>

#include "Algorithm.h"
#include "Allocator.h"
#include "Iterator.h"
#include "UninitializedFunctions.h"
#include "ReverseIterator.h"
namespace WKstl {
	template<class T, class Alloc = allocator<T>>
	class vector {
	private:
		T *start_;
		T *finish_;
		T *endOfStorage_;
		typedef Alloc dataAllocator;
	public:
		typedef T									value_type;
		typedef T*									iterator;
		typedef const T*							const_iterator;
		typedef reverse_iterator_t<T*>				reverse_iterator;
		typedef reverse_iterator_t<const T*>		const_reverse_iterator;
		typedef iterator							pointer;
		typedef T&									reference;
		typedef const T&							const_reference;
		typedef size_t								size_type;
		typedef ptrdiff_t							difference_type;
	public:
		//���죬���ƣ���������
		vector() :start_(0), finish_(0), endOfStorage_(0) {}
		explicit vector(const size_type n);
		vector(const size_type n, const value_type& value);
		//vector(int n, const value_type& value) { vector(size_type(n), value); }
		template<class InputIterator>
		vector(InputIterator first, InputIterator last);
		vector(const vector& v);
		vector(vector&& v);
		vector& operator=(const vector& v);
		vector& operator=(vector&& v);
		~vector();

		//�Ƚϲ���
		bool operator==(const vector& v)const;
		bool operator!=(const vector& v)const;

		//����������
		//����һ����ǰvector��������ʼԪ�صĵ�����
		iterator begin() { return start_; }
		//ע�⼼�ɣ������const�����������غ���begin()
		const_iterator begin()const { return start_; }
		//����Զ�������������мӼ���������һ��ĵ�����һ���������㲻��ͨ��������������޸�����ָ�������
		const_iterator cbegin()const { return start_; }
		//����һ����ǰvector������ĩβԪ�صĵ�����
		iterator end() { return finish_; }
		//ע�⼼�ɣ������const�����������غ���end()
		const_iterator end()const { return finish_; }
		const_iterator cend()const { return finish_; }
		//���ڷ����������4������
		reverse_iterator rbegin() { return reverse_iterator(finish_); }
		const_reverse_iterator crbegin()const { return const_reverse_iterator(finish_); }
		reverse_iterator rend() { return reverse_iterator(start_); }
		const_reverse_iterator crend()const { return const_reverse_iterator(start_); }
		//������صĲ���
		//size()ָĿǰ���ڵ�Ԫ����
		difference_type size()const { return finish_ - start_; }
		//capacity()ָ�����ܴ洢���ݵĸ���
		difference_type capacity()const { return endOfStorage_ - start_; }
		bool empty()const { return start_ == finish_; }
		/*
		resize()������size;
		reserve()������capacity;
		*/
		//void resize(size_type n, const T& val);
		void resize(size_type n, value_type val = value_type());
		void reserve(size_type n);
		//��������������������sizeƥ�䡣
		void shrink_to_fit();

		//����Ԫ�صĲ���
		reference operator[](const difference_type i) { return *(begin() + i); }
		const_reference operator[](const difference_type i)const { return *(cbegin() + i); }
		//���ص�ǰvector��������ʼԪ�ص�����
		reference front() { return *(begin()); }
		//���ص�ǰvector������ĩβԪ�ص�����
		reference back() { return *(end() - 1); }
		pointer data() { return start_; }

		//�޸�������صĲ���
		//�Ƴ�vector������Ԫ�أ����������ǣ�,ʹ������СΪ��Ϊ0��size������capacity��
		void clear();
		//����������ͬ�������������ݣ�����������Ԫ�ؽ��Ὣ��
		void swap(vector& v);
		void push_back(const value_type& value);
		void pop_back();
		//�������в���Ԫ��
		void insert(iterator position, const size_type& n, const value_type& val);
		iterator insert(iterator position, const value_type& val);
		template<class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);

		//ɾ�������е�Ԫ��
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);

		//�����Ŀռ���������صĲ���
		Alloc get_allocator() { return dataAllocator; }
	private:
		void destroyAndDeallocateAll();
		void allocateAndFillN(const size_type n, const value_type& value);
		template<class InputIterator>
		void allocateAndCopy(InputIterator first, InputIterator last);
		//�������������Ǹ����

		template<class InputIterator>
		void vector_aux(InputIterator first, InputIterator last, std::false_type);
		template<class Integer>
		void vector_aux(Integer n, const value_type& value, std::true_type);
		/////////////////////////////////////////////////////////
		template<class InputIterator>
		void insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type);
		template<class Integer>
		void insert_aux(iterator position, Integer n, const value_type& value, std::true_type);
		template<class InputIterator>
		//�������������Ǹ����
		void reallocateAndCopy(iterator position, InputIterator first, InputIterator last);
		void reallocateAndFillN(iterator position, const size_type& n, const value_type& val);
		size_type getNewCapacity(size_type len) const;
	public:
		template<class T, class Alloc>
		friend bool operator==(const vector<T, Alloc>& v1, const vector<T, alloc>& v2);
		template<class T, class Alloc>
		friend bool operator!=(const vector<T, Alloc>& v1, const vector<T, alloc>& v2);
	};
}
//#include "Detail\Vector.impl.h"
#endif
