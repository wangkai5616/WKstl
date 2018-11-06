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
		//构造，复制，析构函数
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

		//比较操作
		bool operator==(const vector& v)const;
		bool operator!=(const vector& v)const;

		//迭代器操作
		//返回一个当前vector容器中起始元素的迭代器
		iterator begin() { return start_; }
		//注意技巧，这里的const可以用来重载函数begin()
		const_iterator begin()const { return start_; }
		//你可以对这个迭代器进行加减操作，和一般的迭代器一样。但是你不能通过这个迭代器来修改其所指向的内容
		const_iterator cbegin()const { return start_; }
		//返回一个当前vector容器中末尾元素的迭代器
		iterator end() { return finish_; }
		//注意技巧，这里的const可以用来重载函数end()
		const_iterator end()const { return finish_; }
		const_iterator cend()const { return finish_; }
		//关于反向迭代器的4个操作
		reverse_iterator rbegin() { return reverse_iterator(finish_); }
		const_reverse_iterator crbegin()const { return const_reverse_iterator(finish_); }
		reverse_iterator rend() { return reverse_iterator(start_); }
		const_reverse_iterator crend()const { return const_reverse_iterator(start_); }
		//容量相关的操作
		//size()指目前存在的元素数
		difference_type size()const { return finish_ - start_; }
		//capacity()指容器能存储数据的个数
		difference_type capacity()const { return endOfStorage_ - start_; }
		bool empty()const { return start_ == finish_; }
		/*
		resize()，设置size;
		reserve()，设置capacity;
		*/
		//void resize(size_type n, const T& val);
		void resize(size_type n, value_type val = value_type());
		void reserve(size_type n);
		//请求容器降低其容量和size匹配。
		void shrink_to_fit();

		//访问元素的操作
		reference operator[](const difference_type i) { return *(begin() + i); }
		const_reference operator[](const difference_type i)const { return *(cbegin() + i); }
		//返回当前vector容器中起始元素的引用
		reference front() { return *(begin()); }
		//返回当前vector容器中末尾元素的引用
		reference back() { return *(end() - 1); }
		pointer data() { return start_; }

		//修改容器相关的操作
		//移除vector内所有元素（并销毁他们）,使容器大小为变为0（size而不是capacity）
		void clear();
		//交换两个相同类型容器的内容，两个容器的元素将会将换
		void swap(vector& v);
		void push_back(const value_type& value);
		void pop_back();
		//向容器中插入元素
		void insert(iterator position, const size_type& n, const value_type& val);
		iterator insert(iterator position, const value_type& val);
		template<class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);

		//删除容器中的元素
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);

		//容器的空间配置器相关的操作
		Alloc get_allocator() { return dataAllocator; }
	private:
		void destroyAndDeallocateAll();
		void allocateAndFillN(const size_type n, const value_type& value);
		template<class InputIterator>
		void allocateAndCopy(InputIterator first, InputIterator last);
		//下面两个函数是干嘛的

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
		//下面两个函数是干嘛的
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
