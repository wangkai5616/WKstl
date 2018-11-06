#ifndef _ITERATOR_H_
#define _ITERATOR_H_
namespace WKstl {
	//����5�����������ͣ���������õ��ͱ��ں���֮�е���;���������������ػ���
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag :public input_iterator_tag {};
	struct bidirectional_iterator_tag :public forward_iterator_tag {};
	struct random_access_iterator_tag :public bidirectional_iterator_tag {};
	//��ֹ������д����ʱ©������ü̳��������iterator
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
	//��������������Զ�ȡ�����е�Ԫ��
	struct input_iterator
	{
		typedef input_iterator_tag		iterator_category;
		typedef T                       value_type;
		typedef Distance                difference_type;
		typedef T*                      pointer;
		typedef T&                      reference;
	};
	//�����������ֻд������Ԫ��
	struct output_iterator
	{
		typedef output_iterator_tag		iterator_category;
		typedef void	                value_type;
		typedef void                    difference_type;
		typedef void                    pointer;
		typedef void                    reference;
	};
	//ǰ������������Զ�дԪ�أ����������ֻ������������һ�������ƶ�
	template <class T, class Distance>
	struct forward_iterator
	{
		typedef forward_iterator_tag		iterator_category;
		typedef T                           value_type;
		typedef Distance                    difference_type;
		typedef T*                          pointer;
		typedef T&                          reference;
	};
	//˫���������������ǰ/���д�����е�Ԫ��
	template <class T, class Distance>
	struct bidirectional_iterator
	{
		typedef bidirectional_iterator_tag		iterator_category;
		typedef T                               value_type;
		typedef Distance                        difference_type;
		typedef T*                              pointer;
		typedef T&                              reference;
	};
	//������ʵ�����������ĵ�����
	template <class T, class Distance>
	struct random_access_iterator
	{
		typedef random_access_iterator_tag		iterator_category;
		typedef T                               value_type;
		typedef Distance                        difference_type;
		typedef T*                              pointer;
		typedef T&                              reference;
	};
	//iterator_traits������ȡ������������
	template<class Iterator>
	struct iterator_traits
	{
		/*
		ÿ�����������������5���ͱ�
		����Ҫ�йؼ���typename����ΪIterator��һ��template���������������������廯֮ǰ��
		��������Tһ����֪�����仰˵����������ʱ����֪��Iterator::iterator_category��һ
		���ͱ����һ����Ա�������������ݳ�Ա�����Թؼ���typename���������ڸ��߱�������
		��һ���ͱ���������ͨ������
		*/
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::value_type           value_type;
		typedef typename Iterator::difference_type      difference_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
	};
	//ƫ�ػ���-���ԭ��ָ�����Ƶ�ƫ�ػ���
	template<class T>
	struct iterator_traits<T*>//����˵��T*��ԭ��ָ��
	{
		//ԭ��ָ����һ��Random Access Iterator
		typedef  random_access_iterator_tag	    iterator_category;
		//ptrdiff_t��C/C++��׼���ж����һ���������ص��������͡�ptrdiff_t���ͱ���ͨ��������������ָ����������Ľ��
		typedef  T                              value_type;
		typedef  ptrdiff_t                      difference_type;
		typedef  T*				                pointer;
		typedef  T&                 			reference;
	};
	//ƫ�ػ���-���������Ǹ�pointer-to-constʱ����ȡ�������ͱ�Ӧ����T����const T
	template<class T>
	struct iterator_traits<const T*>
	{
		typedef  random_access_iterator_tag	    iterator_category;
		typedef  T                              value_type;
		typedef  ptrdiff_t                      difference_type;
		typedef  T*				                pointer;
		typedef  T&                 			reference;
	};

	//�����ж�ĳ��������������
	template<class Iterator>
	typename iterator_traits<Iterator>::iterator_category
		iterator_category(const Iterator& It) {
			typedef typename iterator_traits<Iterator>::iterator_category category;
			/*
			category()������һ����ʱ����int()�����һ��int���͵���ʱ���󣩣����ͱ�����ǰ��4������������
			��input_iterator_tag��forward_iterator_tag��bidirectional_iterator_tag��random_access_iterator_tag����
			֮һ��Ȼ���������ͱ𣬱���������������һ�����Ӧ�����غ�����
			*/
			return category();
	}

	//�����ж�ĳ����������distance_type
	template <class Iterator>
	inline typename iterator_traits<Iterator>::difference_type*
		difference_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}

	//�����ж�ĳ����������value_type
	template <class Iterator>
	inline typename iterator_traits<Iterator>::value_type*
		value_type(const Iterator& ) {
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}
	////////////////////////////////////////////////////////////////////

}
#endif
