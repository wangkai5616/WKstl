#ifndef _REVERSEITERATOR_H_
#define _REVERSEITERATOR_H_
#include "Iterator.h"
//ͷ�ļ�����
namespace WKstl {
	template<class Iterator>
	class reverse_iterator_t {
	protected:
		Iterator current;//��¼���Ƕ�Ӧ�����������
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
		typedef Iterator iterator_type;//�������������
		typedef reverse_iterator_t<Iterator> self;//�������������
	public:
		reverse_iterator_t() {}
		explicit reverse_iterator_t(iterator_type x) :current(x) {}
		reverse_iterator_t(const self& x) :current(x.current) {}
		iterator_type base() const {
			return current;//current�������������ָʾ��λ��
		}
		reference operator*() const {
			Iterator tmp = current;
			//��Ҫ֪ʶ�㣬��Ϊ����������������������ǰһλP439�������ڵ�current������������������ָ��λ��
			return *--tmp;
		}
		pointer operator->() const {
			return &(operatir*());
		}
		//ǰ��++
		self& operator++() {
			--current;
			return *this;
		}
		//����++,���ò���int��������
		self operator++(int) {
			self tmp = *this;
			--current;
			return tmp;
		}
		//ǰ��--
		self& operator--() {
			++current;
			return *this;
		}
		//����--�����ò���int��������
		self operator--(int) {
			self tmp = *this;
			++current;
			return tmp;
		}
		//�൱��ָ�����p+5�Ĳ�����p����ָ��ԭ����λ��
		self operator+(difference_type n)const {
			return self(current - n);//���´���һ��dui
		}
		//�൱��ָ�����p+=5�Ĳ�����pָ���λ�÷����˱仯
		self& operator+=(difference_type n)const {
			current -= n;
			return *this;
		}
		//�൱��ָ�����p-5�Ĳ�����p����ָ��ԭ����λ��
		self operator-(difference_type n)const {
			return self(current + n);
		}
		//�൱��ָ�����p-=5�Ĳ�����pָ���λ�÷����˱仯
		self& operator-=(difference_type n)const {
			current += n;
			return *this;
		}
		////////////////////�о����������õݹ�////////////////////////////////////////////////////������ʲô��˼
		reference operator[](difference_type n)const {
			return *(*this + n);
			//return base()[-n-1];
		}
	};
	/*
	�����������ⶨ��ĺ����������ڳ�Ա����
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
			//��Ҫ֪ʶ�㣬��Ϊ����������������������ǰһλP439�������ڵ�current������������������ָ��λ��
			return *(current - 1);
		}
		
	*/
}
#endif
