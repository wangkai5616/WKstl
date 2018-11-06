#ifndef _TYPE_TRAITS_H_
#define _TYPE_TRAITS_H_
namespace WKstl {
	//这两个空白struct没有任何成员，不会带来额外负担，但能说明真假。可以用来做参数推导
	//因为编译器只有面对class object形式的参数，才会做参数推导
	struct _true_type {};
	struct _false_type {};
	/*
	iterator_traits负责萃取迭代器的特性，_type_traits则负责萃取型别（type）的特性如果这个类都是
	trivial ctor/dtor/copy/assignment函数，我们对这个类进行构造、析构、拷贝和赋值时可以采用最有
	效率的方法，不调用无所事事正真的那些ctor/dtor等，而直接采用内存操作如malloc()、memcpy()等提
	高性能，这也是SGI STL内部干的事情。如果你的类里面只用到的基本类型，如int char double等，系统
	的默认析构函数其实什么都没有做但如果你使用了其他的类如vector，string等，系统的默认析构函数就
	会调用这些类对象的析构函数如果是自己写析构函数的话，如果你的类里面分配了系统资源，如new了内存
	空间，打开了文件等，那么在你的析构函数中就必须释放相应的内存空间和关闭相关的文件；这样系统就
	会自动调用你的析构函数释放资源，避免内存泄漏
	*/
	template<class T>
	struct _type_traits
	{
		typedef _false_type has_trivial_default_constructor;
		typedef _false_type has_trivial_copy_constructor;
		typedef _false_type has_trivial_assignment_operator;
		typedef _false_type has_trivial_destructor;
		typedef _false_type is_POD_type;//看下面的解释//POD:Plain Old Data
	};
	/*
	以下针对C++基本型别char,signed char,unsigned char,short,unsigned short,int,unsigned int,
	long,unsigned long,float,double,long double提供特化版本。每一个成员的值都是_true_type，
	表示这些型别都可以采用最快方式（memcpy）进行拷贝或赋值操作
	*/
	template<>
	struct _type_traits<bool>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};
	template<>
	struct _type_traits<char>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};
	template<>
	struct _type_traits<unsigned char>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};
	template<>
	struct _type_traits<signed char>
	{
		typedef _true_type has_trival_default_constructor;
		typedef _true_type has_trival_copy_constructor;
		typedef _true_type has_trival_assignment_operator;
		typedef _true_type has_trival_destructor;
		typedef _true_type is_POD_type;
	};
	template<>
	struct _type_traits<wchar_t>
	{
		typedef _true_type has_trival_default_constructor;
		typedef _true_type has_trival_copy_constructor;
		typedef _true_type has_trival_assignment_operator;
		typedef _true_type has_trival_destructor;
		typedef _true_type is_POD_type;
	};
	template<>
	struct _type_traits<short>
	{
		typedef _true_type has_trival_default_constructor;
		typedef _true_type has_trival_copy_constructor;
		typedef _true_type has_trival_assignment_operator;
		typedef _true_type has_trival_destructor;
		typedef _true_type is_POD_type;
	};
	template<>
	struct _type_traits<unsigned short>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};
	template<>
	struct _type_traits<int>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};
	template<>
	struct _type_traits<unsigned>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};
	template<>
	struct _type_traits<long>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};
	template<>
	struct _type_traits<unsigned long>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};
	template<>
	struct _type_traits<float>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};
	template<>
	struct _type_traits<double>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};
	template<>
	struct _type_traits<long double>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};
	template<class T>
	struct _type_traits<T*>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};
	template<class T>
	struct _type_traits<const T*>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};
	template<>
	struct _type_traits<char*>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};
	template<>
	struct _type_traits<signed char*>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};
	template<>
	struct _type_traits<unsigned char*>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};
}
/*
1.unsigned、signed
无符号数，作用为存储的数据范围大
unsigned char是无符号字节型，char类型变量的大小通常为1个字节（1字节=8个位），且属于整型。整型的每一种都有无符号（unsigned）和
有符号（signed）两种类型（float和double总是带符号的），在默认情况下声明的整型变量都是有符号的类型（char有点特别），如果需声明
无符号类型的话就需要在类型前加上unsigned。无符号版本和有符号版本的区别就是无符号类型能保存2倍于有符号类型的数据，比如16位系统
中一个int能存储的数据的范围为-32768~32767，而unsigned能存储的数据范围则是0~65535
2.wchar_t
我们知道char类型变量可以存储一个字节的字符，它用来保存英文字符和标点符号是可以的，但是对于汉字、韩文以及日文这样的字符却不可
以，因为汉字、韩文以及日文每一个文字都占据两个字节(数量过多导致的，2^8位不够表示)，为了解决这个问题，c++提出了wchar_t类型，称
之为双字节类型，又称宽字符类型。
3.POD
POD意指Plain Old Data,也就是传统的C struct型别。POD型别必须拥有trival ctor/dtor/copy/assignment函数。
*/
#endif
