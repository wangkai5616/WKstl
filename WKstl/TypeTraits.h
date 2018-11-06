#ifndef _TYPE_TRAITS_H_
#define _TYPE_TRAITS_H_
namespace WKstl {
	//�������հ�structû���κγ�Ա������������⸺��������˵����١����������������Ƶ�
	//��Ϊ������ֻ�����class object��ʽ�Ĳ������Ż��������Ƶ�
	struct _true_type {};
	struct _false_type {};
	/*
	iterator_traits������ȡ�����������ԣ�_type_traits������ȡ�ͱ�type���������������඼��
	trivial ctor/dtor/copy/assignment���������Ƕ��������й��졢�����������͸�ֵʱ���Բ�������
	Ч�ʵķ��������������������������Щctor/dtor�ȣ���ֱ�Ӳ����ڴ������malloc()��memcpy()����
	�����ܣ���Ҳ��SGI STL�ڲ��ɵ����顣������������ֻ�õ��Ļ������ͣ���int char double�ȣ�ϵͳ
	��Ĭ������������ʵʲô��û�����������ʹ��������������vector��string�ȣ�ϵͳ��Ĭ������������
	�������Щ��������������������Լ�д���������Ļ��������������������ϵͳ��Դ����new���ڴ�
	�ռ䣬�����ļ��ȣ���ô��������������оͱ����ͷ���Ӧ���ڴ�ռ�͹ر���ص��ļ�������ϵͳ��
	���Զ�����������������ͷ���Դ�������ڴ�й©
	*/
	template<class T>
	struct _type_traits
	{
		typedef _false_type has_trivial_default_constructor;
		typedef _false_type has_trivial_copy_constructor;
		typedef _false_type has_trivial_assignment_operator;
		typedef _false_type has_trivial_destructor;
		typedef _false_type is_POD_type;//������Ľ���//POD:Plain Old Data
	};
	/*
	�������C++�����ͱ�char,signed char,unsigned char,short,unsigned short,int,unsigned int,
	long,unsigned long,float,double,long double�ṩ�ػ��汾��ÿһ����Ա��ֵ����_true_type��
	��ʾ��Щ�ͱ𶼿��Բ�����췽ʽ��memcpy�����п�����ֵ����
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
1.unsigned��signed
�޷�����������Ϊ�洢�����ݷ�Χ��
unsigned char���޷����ֽ��ͣ�char���ͱ����Ĵ�Сͨ��Ϊ1���ֽڣ�1�ֽ�=8��λ�������������͡����͵�ÿһ�ֶ����޷��ţ�unsigned����
�з��ţ�signed���������ͣ�float��double���Ǵ����ŵģ�����Ĭ����������������ͱ��������з��ŵ����ͣ�char�е��ر𣩣����������
�޷������͵Ļ�����Ҫ������ǰ����unsigned���޷��Ű汾���з��Ű汾����������޷��������ܱ���2�����з������͵����ݣ�����16λϵͳ
��һ��int�ܴ洢�����ݵķ�ΧΪ-32768~32767����unsigned�ܴ洢�����ݷ�Χ����0~65535
2.wchar_t
����֪��char���ͱ������Դ洢һ���ֽڵ��ַ�������������Ӣ���ַ��ͱ������ǿ��Եģ����Ƕ��ں��֡������Լ������������ַ�ȴ����
�ԣ���Ϊ���֡������Լ�����ÿһ�����ֶ�ռ�������ֽ�(�������ർ�µģ�2^8λ������ʾ)��Ϊ�˽��������⣬c++�����wchar_t���ͣ���
֮Ϊ˫�ֽ����ͣ��ֳƿ��ַ����͡�
3.POD
POD��ָPlain Old Data,Ҳ���Ǵ�ͳ��C struct�ͱ�POD�ͱ����ӵ��trival ctor/dtor/copy/assignment������
*/
#endif
