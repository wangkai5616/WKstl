#ifndef _DEQUE_H_
#define _DEQUE_H_
#include "Allocator.h"
#include "Iterator.h"
#include "ReverseIterator.h"
#include "Utility.h"
namespace WKstl {
	template <class T,class Alloc=allocator<T>>
	class deque;
	namespace Detail {
		template<class T>
		class dq_iter :public iterator<bidirectional_iterator_tag, T> {
		private:
			template<class T, class Alloc>
			friend class ::WKstl::deque;
		private:
			typedef const ::WKstl::deque<T>* cntrPtr;
			size_t mapIndex_;//map中的第几个节点
			T *cur_;//指向缓冲区中的元素
			cntrPtr container_; //双端数组地址，也就是书上的缓冲区地址
		public:
			dq_iter() :mapIndex_(-1), cur_(0), container_(0) {}
			dq_iter(size_t index, T *ptr, cntrPtr container)
				:mapIndex_(index), cur_(ptr), container_(container) {}
			dq_iter(const dq_iter& it)
				:mapIndex_(it.mapIndex_), cur_(it.cur_), container_(it.container_) {}
			dq_iter& operator=(const dq_iter& it);
			void swap(dq_iter& it);
			reference operator*() { return *cur_; }
			const reference operator*()const { return *cur_; }
			pointer operator->() { return &(operator*()); }
			const pointer operator->()const { return &(operator*()); }
			dq_iter& operator++();
			dq_iter operator++(int);
			dq_iter& operator--();
			dq_iter operator--(int);
			bool operator==(const dq_iter& it)const;
			bool operator!=(const dq_iter& it)const;
		private:
			T *getBuckTail(size_t mapIndex)const;
			T *getBuckHead(size_t mapIndex)const;
			size_t getBuckSize()const;
		public:
			template<class T>
			friend dq_iter<T> operator+(const dq_iter<T>& it, typename dq_iter<T>::difference_type n);
			template<class T>
			friend dq_iter<T> operator+(typename dq_iter<T>::difference_type n, const dq_iter<T>& it);
			template<class T>
			friend dq_iter<T> operator-(const dq_iter<T>& it, typename dq_iter<T>::difference_type n);
			template<class T>
			friend dq_iter<T> operator-(typename dq_iter<T>::difference_type n, const dq_iter<T>& it);
			template<class T>
			friend typename dq_iter<T>::difference_type operator-(const  dq_iter<T>& it1, const dq_iter<T>& it2);
			template<class T>
			friend void swap(dq_iter<T>& lhs, dq_iter<T>& rhs);
		};
	}
		template<class T,class Alloc>//不是默认的
		class deque {
		private:
			template<class T>
			friend class ::WKstl::Detail::dq_iter;
		public:
			typedef T value_type;
			typedef Detail::dq_iter<T> iterator;
			typedef Detail::dq_iter<const T>const_iterator;
			typedef T& reference;
			typedef const reference const_reference;
			typedef size_t size_type;
			typedef ptrdiff_t difference_type;
			typedef Alloc allocator_type;
		private:
			typedef Alloc dataAllocator;
			enum class EBucksSize { BUCKSIZE = 64 };
		private:
			iterator beg_;//指向开始元素
			iterator end_;//指向最后元素的下一个
			size_t mapSize_;//map内容纳指针的个数
			T **map_;//map_是块连续空间，其每个元素都是个指针，指向一个节点(用一个数组进行表示)map_[i][j]就是实际的元素
		public:
			deque();
			explicit deque(size_type n, const value_type& val = value_type());
			template<class InputIterator>
			deque(InputIterator first, InputIterator last);
			deque(const deque& x);
			~deque();
			deque& operator=(const deque& x);
			//deque& operator=(deque&& x);
			iterator begin();
			iterator end();
			iterator begin()const;
			iterator end()const;
		public:
			size_type size()const { return end() - begin(); }
			bool empty()const { return begin() == end(); }
			reference operator[](size_type n);
			reference front();
			reference back();
			const_reference operator[](size_type n) const;
			const_reference front()const;
			const_reference back()const;

			void push_back(const value_type& val);
			void push_front(const value_type& val);
			void pop_back();
			void pop_front();
			void swap(deque& x);
			void clear();
		private:
			T* getANewBuck();
			T** getANewMap(const size_t size);
			size_t getNewMapSize(const size_t size);
			size_t getBuckSize()const;
			void init();
			bool back_full()const;
			bool front_full()const;
			void deque_aux(size_t n, const value_type& val, std::true_type);
			template<class Iterator>
			void deque_aux(Iterator first, Iterator last, std::false_type);
			void reallocateAndCopy();
		public:
			template<class T,class Alloc>
			friend bool operator==(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs);
			template<class T, class Alloc>
			friend bool operator!=(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs);
			template<class T,class Alloc>
			friend void swap(deque<T, Alloc>& x, deque<T, Alloc>& y);
		};
		

}
	/*
	枚举类型是强类型的，从而保证了系统安全性。枚举可以限定参数的个数，对调用者的行为能更加严格地进行控制。
	把一些运行期的参数检查放到了编译期，这点很重要。	
	举个常用的例子,你要表示星期1-7,你可以用int1-7,但是当你把它作为参数的时候,
	有时后你就给考虑传入0,8的情况.而且用数字表示还需要相应的注释和文档.
	这个时候你定义个一个枚举,名字就叫字面就叫Monday , Tuesday ,....就行,直观,并且值可控.
	eg.  enum  Days {Sat,Sun,Mon,Tue,Wed,Thu,Fri};
	ps: 默认从零开始，然后依次加一。即sat=0;sun=1....fri=6;
	也可在初始化时给其赋值。
	*/
#endif
