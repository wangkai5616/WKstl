#ifndef _LIST_H_
#define _LIST_H_
#include "Iterator.h"
#include "Allocator.h"
#include "UninitializedFunctions.h"
#include "ReverseIterator.h"

#include <type_traits>//自己有定义，为什么还要用系统的
namespace WKstl{
	template<class T>
	class list;//先进行声明，节点的定义中会用到
	namespace Detail {
		template<class T>
		//节点结构
		struct node {
			T data;
			node *prev;
			node *next;
			//list本身和list的节点是不同的结构，list中存放的是节点
			//container是存放当前node节点的链表指针
			list <T> *container;
			node(const T& d, node *p, node *n, list<T> *c) :
				data(d), prev(p), next(n), container(c) {}
			bool operator==(const node& n) {
				return data == n.data && prev == n.prev && next == n.next && container == n.container;
			}
		};

		/*
			list不再能够像vector一样以普通指针作为迭代器，因为其节点不保证在储存空间中
			连续存在，list迭代器必须有能力指向list的节点，并有能力进行正确的递增、递减、
			取值、成员存储等操作
		*/
		template<class T>
		/*
		当初说过，自定义的迭代器最好都继承
		template<class Category,class T,class Distance=ptrdiff_t,
		class Pointer=T*,class Reference=T&>
		struct iterator
		{
		typedef Category		        iterator_category;
		typedef T                       value_type;
		typedef Distance                difference_type;
		typedef T*                      pointer;
		typedef T&                      reference;
		};
		*/
		struct listIterator :public iterator<bidirectional_iterator_tag, T> {
			/*
			友元类的所有成员函数都是另一个类的友元函数，都可以访
			问另一个类中的隐藏信息（包括私有成员和保护成员）
			类list的所有成员函数都是类listIterator的友元函数，能存取类listIterator的私有成员和保护成员
			*/
			template<class T>
			friend class list;
		public:
			typedef node<T>* nodePtr;
			nodePtr p;//list迭代器必须有能力指向list的节点
		public:
			explicit listIterator(nodePtr ptr = nullptr) :p(ptr) {}
			listIterator& operator++();//前置
			listIterator operator++(int);//后置
			listIterator& operator--();
			listIterator operator--(int);
			T& operator *() { return p->data; }
			T* operator ->() { return &(operator*()); }
			template<class T>
			friend bool operator==(const listIterator<T>& lhs, const listIterator<T>& rhs);
			template<class T>
			friend bool operator!=(const listIterator<T>& lhs, const listIterator<T>& rhs);
		};
	}
	/*
	list有一个重要性质：插入操作和接合操作都不会造成记忆体重新配置，导致原有的迭代器全部失效。
	甚至list的元素删除操作也只有“指向被删除元素”的哪个迭代器失效，其它迭代器是不受影响的
	*/
	template<class T>
	class list {
		template<class T>
		friend struct listIterator;
	private:
		typedef allocator<Detail::node<T>> nodeAllocator;
		typedef Detail::node<T>* nodePtr;
	public:
		typedef T value_type;
		typedef Detail::listIterator<T> iterator;
		typedef Detail::listIterator<const T> const_iterator;
		typedef reverse_iterator_t<iterator> reverse_iterator;
		typedef T& reference;
		typedef size_t size_type;
	private:
		iterator head;//指向链首
		iterator tail;//指向链尾
	public:
		list();
		explicit list(size_type n, const value_type& val = value_type());
		template<class InputIterator>
		list(InputIterator first, InputIterator last);
		list(const list& l);
		list& operator=(const list& l);
		~list();

		bool empty()const { return head == tail; }
		size_type size()const;
		reference front() { return (head.p->data); }
		reference back() { return(tail.p->prev->data); }

		void push_front(const value_type& val);
		void pop_front();
		void push_back(const value_type& val);
		void pop_back();

		iterator begin();
		iterator end();
		const_iterator begin()const;
		const_iterator end()const;
		reverse_iterator rbegin();
		reverse_iterator rend();

		iterator insert(iterator position, const value_type& val);
		void insert(iterator position, size_type n, const value_type& val);
		template<class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);
		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);
		void swap(list& x);
		void clear();
		void splice(iterator position, list &x);
		void splice(iterator position, list &x, iterator i);
		void splice(iterator position, list& x, iterator first, iterator last);
		void remove(const value_type& val);
		template<class Predicate>
		void remove_if(Predicate pred);
		void unique();
		template<class BinaryPredicate>
		void unique(BinaryPredicate binary_pred);
		void merge(list& x);
		template<class Compare>
		void merge(list& x, Compare comp);
		void sort();
		template<class Compare>
		void sort(Compare comp);
		void reverse();
	private:
		void ctorAux(size_type n, const value_type& val, std::true_type);
		template<class InputIterator>
		void ctorAux(InputIterator first, InputIterator last, std::false_type);
		nodePtr newNode(const T& val = T());
		void deleteNode(nodePtr p);
		void insert_aux(iterator position, size_type n, const T& val, std::true_type);
		template<class InputIterator>
		void insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type);
		const_iterator changeIteratorToConstIterator(iterator& it)const;
	public:
		template<class T>
		friend void swap(list<T>& x, list<T>& y);
		template<class T>
		friend bool operator==(const list<T>& lhs, const list<T>& rhs);
		template<class T>
		friend bool operator!=(const list<T>& lhs, const list<T>& rhs);
	};
}
#endif