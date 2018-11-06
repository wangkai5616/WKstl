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
	//unary_function��������һԪ�����Ĳ����ͱ�ͻط�ֵ�ͱ�
	template <class Arg,class Result>
	struct unary_function {
		typedef Arg argument_type;
		typedef Result result_type;
	};

	//binary_function�������ֶ�Ԫ�����ĵ�һ�����ͱ𡢵ڶ������ͱ�
	//�Լ��ط�ֵ�ͱ�
	template <class Arg1,class Arg2,class Result>
	struct binary_function {
		typedef Arg1 first_argument_type;
		typedef Arg2 second_argument_type;
		typedef Result result_type;
	};

	/*
	������6��������º���
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
	������6����ϵ��º���
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
	������3���߼�������º���
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
��const�ں�����ǰ���ʱ�����ε��Ǻ�������ֵ���ں����������ʾ�ǳ���Ա������
�ú��������޸Ķ����ڵ��κγ�Ա��ֻ�ܷ��������������ܷ���д������
*/
#endif
