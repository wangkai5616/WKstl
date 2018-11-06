#ifndef _ALLOC_H_
#define _ALLOC_H_
#include <cstdlib>
/*
����C�е�stdlib.h����C++�е��ø�ͷ�ļ��еĺ�����Ҫ����cstdlib.�÷���C�����е�stdlib.h��ͬ
stdlib ͷ�ļ���standard library��׼��ͷ�ļ���stdlib.h���涨�����������͡�һЩ���ͨ�ù��ߺ�����
��������size_t��wchar_t��div_t��ldiv_t��lldiv_t�� ������EXIT_FAILURE��EXIT_SUCCESS��RAND_MAX��MB_CUR_MAX�ȵȣ�
���õĺ�����malloc()��calloc()��realloc()��free()��system()��atoi()��atol()��rand()��srand()��exit()�ȵȡ�
*/
namespace WKstl {
	class alloc {
	private:
		enum EAlign {ALIGN=8};//С��������ϵ��߽磬��8bytesΪһ�����
		enum EMaxBytes{ MAXBYTES = 128 };//С�����������
		enum ENFreeLists {NFREELISTS=(EMaxBytes::MAXBYTES)/(EAlign::ALIGN)};//free-lists����
		enum ENObjs {NOBJS=20};//ÿ�����ӵĽڵ���
	private:
		/*
		obj��free-lists�ڵ�ṹ�����ֽṹ�൱���һ�����á�
		�ӵ�һ�ֶι۲죬obj�ɱ���Ϊһ��ָ�룬ָ����ͬ��ʽ����һ��obj��
		�ӵڶ��ֶι۲죬obj�ɱ���Ϊһ��ָ�룬ָ��ʵ������
		*/
		union obj {
			union obj *next;
			char client[1];
		};
		//��ʾfree_list����������ŵĶ���obj���͵�ָ��
		static obj *free_list[ENFreeLists::NFREELISTS];//������Ǹ�����
	private:
		static char *start_free;//�ڴ�ص���ʼλ��
		static char *end_free;//�ڴ�صĽ���λ��
		static size_t heap_size;//�ڴ�ص�������С
	private:
		//��bytes�ϵ���8�ı���
		//SGI���������������������κ�С��������ڴ��������ϵ���8�ı���
		static size_t ROUND_UP(size_t bytes) {
			return((bytes+EAlign::ALIGN-1)&~(EAlign::ALIGN-1));
		}
		//��������Ĵ�С������ʹ�õڼ���free_list��n��0��ʼ����
		static size_t FREELIST_INDEX(size_t bytes) {
			return((bytes + (EAlign::ALIGN - 1)) / (EAlign::ALIGN - 1));
		}
		//��free list��û�п�������ʱ��refill()������Ϊfree list�������ռ䡣�µĿռ佫ȡ���ڴ��
		static void *refill(size_t n);
		//����һ���ռ䣬������nobjs����СΪsize������
		//�������nobjs�������������㣬nobjs���ܻή��
		static char *chunk_alloc(size_t size, int &nodjs);
	public:
		//�ڴ�ռ����ú���
		static void *allocate(size_t bytes);
		//�ڴ�ռ��ͷź������������
		static void deallocate(void *ptr, size_t bytes);
		//���·����ڴ�ռ�
		static void *reallocate(void *ptr, size_t old_sz, size_t new_sz);
	};
}
/*
1.enum
ö�ٵĶ��壺
enum ������ {ö��ֵ��}��
�������Ǳ�������ָ��ö�����͵����ơ�
ö��ֵ��Ҳ��ö��Ԫ���б��г������ö�����͵����п���ֵ������ֵ֮���á�,���ֿ���
2.union
��1��ʲô�����ϣ�
�����ϡ���һ��������࣬Ҳ��һ�ֹ������͵����ݽṹ����һ�������ϡ��ڿ��Զ�����ֲ�ͬ���������ͣ� һ����˵��Ϊ�á����ϡ����͵ı����У�
����װ��á����ϡ���������κ�һ�����ݣ���Щ���ݹ���ͬһ���ڴ棬�Ѵﵽ��ʡ�ռ��Ŀ�ģ�����һ����ʡ�ռ�����ͣ�λ�򣩡� ����һ���ǳ�
����ĵط���Ҳ�����ϵ����������⣬ͬstructһ��������Ĭ�Ϸ���Ȩ��Ҳ�ǹ��еģ����ң�Ҳ���г�Ա������
��2��������ṹ������
�����ϡ��롰�ṹ����һЩ����֮�����������б����ϵĲ�ͬ���ڽṹ�и���Ա�и��Ե��ڴ�ռ䣬 һ���ṹ�������ܳ����Ǹ���Ա����֮�ͣ��ս�
�����⣬ͬʱ�����Ǳ߽�����������ڡ����ϡ��У�����Ա����һ���ڴ�ռ䣬 һ�����ϱ����ĳ��ȵ��ڸ���Ա����ĳ��ȡ�Ӧ��˵�����ǣ� ����
��ν�Ĺ�����ָ�Ѷ����Աͬʱװ��һ�����ϱ����ڣ� ����ָ�����ϱ����ɱ�������һ��Աֵ����ÿ��ֻ�ܸ�һ��ֵ�� ������ֵ���ȥ��ֵ��
*/
#endif