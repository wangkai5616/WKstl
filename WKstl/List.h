#ifndef _LIST_H_
#define _LIST_H_
#include "Iterator.h"
#include "Allocator.h"
#include "UninitializedFunctions.h"
#include "ReverseIterator.h"

#include <type_traits>//�Լ��ж��壬Ϊʲô��Ҫ��ϵͳ��
namespace WKstl{
	template<class T>
	class list;//�Ƚ����������ڵ�Ķ����л��õ�
	namespace Detail {
		template<class T>
		//�ڵ�ṹ
		struct node {
			T data;
			node *prev;
			node *next;
			//list�����list�Ľڵ��ǲ�ͬ�Ľṹ��list�д�ŵ��ǽڵ�
			//container�Ǵ�ŵ�ǰnode�ڵ������ָ��
			list <T> *container;
			node(const T& d, node *p, node *n, list<T> *c) :
				data(d), prev(p), next(n), container(c) {}
			bool operator==(const node& n) {
				return data == n.data && prev == n.prev && next == n.next && container == n.container;
			}
		};

		/*
			list�����ܹ���vectorһ������ָͨ����Ϊ����������Ϊ��ڵ㲻��֤�ڴ���ռ���
			�������ڣ�list����������������ָ��list�Ľڵ㣬��������������ȷ�ĵ������ݼ���
			ȡֵ����Ա�洢�Ȳ���
		*/
		template<class T>
		/*
		����˵�����Զ���ĵ�������ö��̳�
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
			��Ԫ������г�Ա����������һ�������Ԫ�����������Է�
			����һ�����е�������Ϣ������˽�г�Ա�ͱ�����Ա��
			��list�����г�Ա����������listIterator����Ԫ�������ܴ�ȡ��listIterator��˽�г�Ա�ͱ�����Ա
			*/
			template<class T>
			friend class list;
		public:
			typedef node<T>* nodePtr;
			nodePtr p;//list����������������ָ��list�Ľڵ�
		public:
			explicit listIterator(nodePtr ptr = nullptr) :p(ptr) {}
			listIterator& operator++();//ǰ��
			listIterator operator++(int);//����
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
	list��һ����Ҫ���ʣ���������ͽӺϲ�����������ɼ������������ã�����ԭ�еĵ�����ȫ��ʧЧ��
	����list��Ԫ��ɾ������Ҳֻ�С�ָ��ɾ��Ԫ�ء����ĸ�������ʧЧ�������������ǲ���Ӱ���
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
		iterator head;//ָ������
		iterator tail;//ָ����β
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