#ifndef _REVERSEITERATOR_H_
#define _REVERSEITERATOR_H_
#include "Iterator.h"
//头文件待定
namespace WKstl {
	template<class Iterator>
	class reverse_iterator_t {
	protected:
		Iterator current;//记录的是对应的正向迭代器
	public:
		typedef typename iterator_traits<Iterator>::iterator_category
			ierator_category;
		typedef typename iterator_traits<Iterator>::value_type
			value_type;
		typedef typename iterator_traits<Iterator>::difference_type
			difference_type;
		typedef typename iterator_traits<Iterator>::pointer
			pointer;
		typedef typename iterator_traits<Iterator>::reference
			reference;
		typedef Iterator iterator_type;//代表正向迭代器
		typedef reverse_iterator_t<Iterator> self;//代表逆向迭代器
	public:
		reverse_iterator_t() {}
		explicit reverse_iterator_t(iterator_type x) :current(x) {}
		reverse_iterator_t(const self& x) :current(x.current) {}
		iterator_type base() const {
			return current;//current是正向迭代器所指示的位置
		}
		reference operator*() const {
			Iterator tmp = current;
			//重要知识点，因为反向迭代器比正向迭代器靠前一位P439，而现在的current代表的是正向迭代器所指的位置
			return *--tmp;
		}
		pointer operator->() const {
			return &(operatir*());
		}
		//前置++
		self& operator++() {
			--current;
			return *this;
		}
		//后置++,利用参数int进行区分
		self operator++(int) {
			self tmp = *this;
			--current;
			return tmp;
		}
		//前置--
		self& operator--() {
			++current;
			return *this;
		}
		//后置--，利用参数int进行区分
		self operator--(int) {
			self tmp = *this;
			++current;
			return tmp;
		}
		//相当于指针进行p+5的操作，p还是指向原来的位置
		self operator+(difference_type n)const {
			return self(current - n);//重新创建一个dui
		}
		//相当于指针进行p+=5的操作，p指向的位置发生了变化
		self& operator+=(difference_type n)const {
			current -= n;
			return *this;
		}
		//相当于指针进行p-5的操作，p还是指向原来的位置
		self operator-(difference_type n)const {
			return self(current + n);
		}
		//相当于指针进行p-=5的操作，p指向的位置发生了变化
		self& operator-=(difference_type n)const {
			current += n;
			return *this;
		}
		////////////////////感觉作者是利用递归////////////////////////////////////////////////////到底是什么意思
		reference operator[](difference_type n)const {
			return *(*this + n);
			//return base()[-n-1];
		}
	};
	/*
	下面是在类外定义的函数，不属于成员函数
	*/
	template<class Iterator>
	bool operator==(const reverse_iterator_t<Iterator>& x,
		const reverse_iterator_t<Iterator>& y) {
		return x.base() == y.base();
	}
	template<class Iterator>
	bool operator<(const reverse_iterator_t<Iterator>& x,
		const reverse_iterator_t<Iterator>& y) {
		return x.base() < y.base();
	}
	template<class Iterator>
	typename reverse_iterator_t<Iterator>::difference_type 
		operator-(const reverse_iterator_t<Iterator>& x,
		const reverse_iterator_t<Iterator>& y) {
		return y.base() - x.base();
	}

	/*
	template<class RandomAccessIterator,class T,class reference=T&,
		class Distance>
	class reverse_iterator {
		typedef reverse_iterator<RandomAccessIterator, T, reference, Distance> self;
	protected:
		RandomAccessIterator current;
	public:
		typedef random_access_iterator_tag		iterator_category;
		typedef T                               value_type;
		typedef Distance                        difference_type;
		typedef T*                              pointer;
		typedef T&                              reference;
		reverse_iterator() {}
		explict reverse_iterator(RandomAccessIterator x) const {}
		RandomAccessIterator babse() const {
			return current;
		}
		reference operator*() const {
			//重要知识点，因为反向迭代器比正向迭代器靠前一位P439，而现在的current代表的是正向迭代器所指的位置
			return *(current - 1);
		}
		
	*/
}
#endif
