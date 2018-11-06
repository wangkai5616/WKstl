#ifndef _FUNCTIONAL_H_
#define _FUNCTIONAL_H_
namespace WKstl {
	//***************less*************************
	template<class T>
	struct less {
		typedef T first_argument_type;
		typedef T second_argument_type;
		typedef bool result_type;
		result_type operator()(const first_argument_type& x, const second_argument_type& y) {
			return x < y;
		}
		//***************equal*************************
		template<class T>
		struct equal_to {
			typedef T first_argument_type;
			typedef T second_argument_type;
			typedef bool result_type;
			result_type operator()(const first_argument_type& x, const second_argument_type& y) {
				return x == y;
			}
		};


	};
	//unary_function用来呈现一元函数的参数型别和回反值型别
	template <class Arg,class Result>
	struct unary_function {
		typedef Arg argument_type;
		typedef Result result_type;
	};

	//binary_function用来呈现二元函数的第一参数型别、第二参数型别
	//以及回返值型别
	template <class Arg1,class Arg2,class Result>
	struct binary_function {
		typedef Arg1 first_argument_type;
		typedef Arg2 second_argument_type;
		typedef Result result_type;
	};

	/*
	以下是6个算术类仿函数
	*/
	template <class T>
	struct plus :public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const {
			return x + y;
		}
	};

	template <class T>
	struct minus :public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const {
			return x - y;
		}
	};

	template <class T>
	struct multipies :public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const {
			return x * y;
		}
	};
	
	template <class T>
	struct divides :public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const {
			return x / y;
		}
	};

	template <class T>
	struct modulus :public binary_function<T, T, T> {
		T operator()(const T& x, const T& y) const {
			return x % y;
		}
	};

	template <class T>
	struct negate :public unary_function<T, T> {
		T operator()(const T& x) const {
			return -x;
		}
	};

	/*
	以下是6个关系类仿函数
	*/
	template <class T>
	struct equal_to :public binary_function<T, T, bool> {
		bool operator()(const T&x, const T& y) const {
			return x == y;
		}
	};

	template <class T>
	struct not_equal_to :public binary_function<T, T, bool> {
		bool operator()(const T&x, const T& y) const {
			return x != y;
		}
	};

	template <class T>
	struct greater :public binary_function<T, T, bool> {
		bool operator()(const T&x, const T& y) const {
			return x > y;
		}
	};
	/*
	template <class T>
	struct less :public binary_function<T, T, bool> {
		bool operator()(const T&x, const T& y) const {
			return x < y;
		}
	};
	*/

	template <class T>
	struct greater_equal :public binary_function<T, T, bool> {
		bool operator()(const T&x, const T& y) const {
			return x >= y;
		}
	};

	template <class T>
	struct less_equal :public binary_function<T, T, bool> {
		bool operator()(const T&x, const T& y) const {
			return x <= y;
		}
	};

	/*
	以下是3个逻辑运算类仿函数
	*/
	template <class T>
	struct logical_and :public binary_function<T, T, bool> {
		bool operator()(const T&x, const T& y) const {
			return x && y;
		}
	};

	template <class T>
	struct logical_or :public binary_function<T, T, bool> {
		bool operator()(const T&x, const T& y) const {
			return x || y;
		}
	};

	template <class T>
	struct logical_not :public unary_function<T, bool> {
		bool operator()(const T&x) const {
			return !x;
		}
	};
}
/*
当const在函数名前面的时候修饰的是函数返回值，在函数名后面表示是常成员函数，
该函数不能修改对象内的任何成员，只能发生读操作，不能发生写操作。
*/
#endif
