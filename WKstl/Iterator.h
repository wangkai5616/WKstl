#ifndef _ITERATOR_H_
#define _ITERATOR_H_
namespace WKstl {
	//这是5个迭代器类型，用来标记用的型别，在函数之中的用途就是用来激活重载机制
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag :public input_iterator_tag {};
	struct bidirectional_iterator_tag :public forward_iterator_tag {};
	struct random_access_iterator_tag :public bidirectional_iterator_tag {};
	//防止开发者写代码时漏掉，最好继承下面这个iterator
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
	template <class T,class Distance>
	//输入迭代器，可以读取序列中的元素
	struct input_iterator
	{
		typedef input_iterator_tag		iterator_category;
		typedef T                       value_type;
		typedef Distance                difference_type;
		typedef T*                      pointer;
		typedef T&                      reference;
	};
	//输出迭代器，只写而不读元素
	struct output_iterator
	{
		typedef output_iterator_tag		iterator_category;
		typedef void	                value_type;
		typedef void                    difference_type;
		typedef void                    pointer;
		typedef void                    reference;
	};
	//前向迭代器，可以读写元素，这类迭代器只能在序列中沿一个方向移动
	template <class T, class Distance>
	struct forward_iterator
	{
		typedef forward_iterator_tag		iterator_category;
		typedef T                           value_type;
		typedef Distance                    difference_type;
		typedef T*                          pointer;
		typedef T&                          reference;
	};
	//双向迭代器，可以向前/后读写序列中的元素
	template <class T, class Distance>
	struct bidirectional_iterator
	{
		typedef bidirectional_iterator_tag		iterator_category;
		typedef T                               value_type;
		typedef Distance                        difference_type;
		typedef T*                              pointer;
		typedef T&                              reference;
	};
	//随机访问迭代器，最吊的迭代器
	template <class T, class Distance>
	struct random_access_iterator
	{
		typedef random_access_iterator_tag		iterator_category;
		typedef T                               value_type;
		typedef Distance                        difference_type;
		typedef T*                              pointer;
		typedef T&                              reference;
	};
	//iterator_traits负责萃取迭代器的特性
	template<class Iterator>
	struct iterator_traits
	{
		/*
		每个迭代器都有下面的5种型别
		必须要有关键字typename，因为Iterator是一个template参数，在它被编译器具体化之前，
		编译器对T一无所知，换句话说，编译器此时并不知道Iterator::iterator_category是一
		个型别或是一个成员函数或者是数据成员。所以关键字typename的用意在于告诉编译器这
		是一个型别，这样才能通过编译
		*/
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::value_type           value_type;
		typedef typename Iterator::difference_type      difference_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
	};
	//偏特化版-针对原生指针而设计的偏特化版
	template<class T>
	struct iterator_traits<T*>//用来说明T*是原生指针
	{
		//原生指针是一种Random Access Iterator
		typedef  random_access_iterator_tag	    iterator_category;
		//ptrdiff_t是C/C++标准库中定义的一个与机器相关的数据类型。ptrdiff_t类型变量通常用来保存两个指针减法操作的结果
		typedef  T                              value_type;
		typedef  ptrdiff_t                      difference_type;
		typedef  T*				                pointer;
		typedef  T&                 			reference;
	};
	//偏特化版-当迭代器是个pointer-to-const时，萃取出来的型别应该是T而非const T
	template<class T>
	struct iterator_traits<const T*>
	{
		typedef  random_access_iterator_tag	    iterator_category;
		typedef  T                              value_type;
		typedef  ptrdiff_t                      difference_type;
		typedef  T*				                pointer;
		typedef  T&                 			reference;
	};

	//用于判断某个迭代器的类型
	template<class Iterator>
	typename iterator_traits<Iterator>::iterator_category
		iterator_category(const Iterator& It) {
			typedef typename iterator_traits<Iterator>::iterator_category category;
			/*
			category()将产生一个暂时对象（int()会产生一个int类型的暂时对象），其型别属于前面4个迭代器类型
			（input_iterator_tag、forward_iterator_tag、bidirectional_iterator_tag、random_access_iterator_tag、）
			之一。然后根据这个型别，编译器决定调用哪一个相对应的重载函数。
			*/
			return category();
	}

	//用于判断某个迭代器的distance_type
	template <class Iterator>
	inline typename iterator_traits<Iterator>::difference_type*
		difference_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}

	//用于判断某个迭代器的value_type
	template <class Iterator>
	inline typename iterator_traits<Iterator>::value_type*
		value_type(const Iterator& ) {
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}
	////////////////////////////////////////////////////////////////////

}
#endif
