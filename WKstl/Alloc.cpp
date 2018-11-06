#include "Alloc.h"
namespace WKstl {
	//�ڴ����ʼ��ַ
	char *alloc::start_free = 0;
	//�ڴ�ؽ�����ַ
	char *alloc::end_free = 0;
	//�ڴ�س�ʼ������С
	size_t alloc::heap_size = 0;
	//obj������alloc,free_list������alloc���ⶼ��Ҫ����˵��
	//�������16��free-lists
	alloc::obj *alloc::free_list[alloc::ENFreeLists::NFREELISTS] =
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	void *alloc::allocate(size_t bytes) {
		//����128������malloc()ֱ�ӷ����ڴ�
		if (bytes > EMaxBytes::MAXBYTES) {
			return malloc(bytes);
		}
		//Ѱ��16��free lists���ʵ���һ��
		size_t index = FREELIST_INDEX(bytes);
		//list��������Ǵ�ŵ�ĳ���ֽڶ�Ӧ�Ŀ�������
		obj *node = free_list[index];
		//�����������ʹ��
		if (node) {
			free_list[index] = node->next;//��һ����������
			return node;//���ؿ�������
		}
		//��ʱ���޿������飬ȥ���ڴ�ء�ROUND_UP(bytes)���ϵ���8�ı���
		else {
			return refill(ROUND_UP(bytes));
		}
	}
	void alloc::deallocate(void *ptr, size_t bytes) {
		//����128������free()ֱ���ͷ��ڴ�
		if (bytes > EMaxBytes::MAXBYTES) {
			free(ptr);
		}
		//�����黹��16��free lists���ʵ���һ��
		else{
			size_t index = FREELIST_INDEX(bytes);
			//��void *ptrǿ��תΪobj *
			obj *node = static_cast<obj *> (ptr);
			//�黹������ɶ�Ӧ���ֽ������еĵ�һ����������
			node->next = free_list[index];
			free_list[index] = node;
		}
	}
	void *alloc::reallocate(void *ptr, size_t old_sz, size_t new_sz) {
		deallocate(ptr, old_sz);
		ptr = allocate(new_sz);
		return ptr;
	}
	//����refill�ĵط�bytes��8�ı���
	void *alloc::refill(size_t bytes) {
		int nobjs = 20;//ÿ��ϣ�����20��������
		/*����chunk_alloc(),���Ի��nobjs��������Ϊfree_list���½ڵ㣬
		������ڴ��������������20�����������nobjs��ֵ
		*/
		char *chunk = chunk_alloc(bytes, nobjs);//ע�⣺chunk���ڱ�����Ǵ��ڴ�����õ����������ʼ��ַ
		obj **my_free_list=0;//����ж�������飬�ŵ�����
		obj *result = 0;
		obj *current_obj = 0, *next_obj = 0;
		//���ֻ���һ�����飬�������ͷ�����������ã�free_list���½ڵ�
		if (nobjs == 1) {
			return chunk;
		}
		//����Ƕ���һ�����飬�����free_list�������½ڵ㣬����Ӧ��free_list
		else {
			//���ҵ��Ŷ��������λ��
			my_free_list = free_list + FREELIST_INDEX(bytes);
			//��һ�鷵�ظ��Ͷ˹�ʹ��
			result = (obj *)chunk;
			//��һ�����飨bytes��һ������ķ�
			*my_free_list = next_obj = (obj *)(chunk + bytes);
			//forѭ��ֹͣ�����������еĴ��ڴ�����õ����������ŵ���Ӧ��free list
			for (int i = 1; i + 1 < nobjs; i++) {//i+1����Ϊ���Ѿ�����һ��������Ͷ�
				current_obj = next_obj;
				next_obj = (obj *)((char *)next_obj + bytes);
				current_obj->next = next_obj;
			}
			//���nobjs-1==i,��ôcurrent_obj�������һ��������
			current_obj->next = 0;
		}
		return result;
	}
	//���ڴ��ȡ�ռ��free_listʹ�ã���ʵ��chunk_alloc()�Ĺ���
	char *alloc::chunk_alloc(size_t bytes, int &nobjs) {//�����nobjs�����ã���Ϊ�п��ܸ���nobjs��ֵ
		char *result = 0;
		size_t total_bytes = bytes*nobjs;
		size_t bytes_left = end_free - start_free;//�����ڴ�ص�ʣ��ռ䣬Ҳ�����ڴ����ʼ��ַ�ͽ�����ַ�ļ��
		//�ڴ��ʣ��ռ���ȫ����������
		if (bytes_left > total_bytes) {
			result = start_free;
			start_free = start_free + total_bytes;//�����ڴ�ص���ʼ��ַ
			return result;//����Ǹ�free list����ʼ��ַ
		}
		//�ڴ��ʣ��ռ䲻����ȫ���������������㹻��Ӧһ�����ϵ�����
		else if (bytes_left>=bytes){
			nobjs = bytes_left / bytes;//����nobjs��ֵ����Ϊ����20�����飬�����ڴ�����ó���������
			total_bytes = bytes*nobjs;//�ɸ��µ�nobjsֵ����total_bytes��ֵ
			result = start_free;
			start_free += total_bytes;//�����ڴ�ص���ʼ��ַ
			return result;
		}
		//�ڴ��ʣ��ռ���һ������Ĵ�С���޷��ṩ
		else {
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			//����һ�����飬����Ҳ����һ�㶼û��
			if (bytes_left > 0) {
				obj **my_free_list = free_list + FREELIST_INDEX(bytes);
				//���ⲻ��һ������Ŀռ�����ڴ��
				((obj *)start_free)->next = *my_free_list;
				*my_free_list = (obj *)start_free;
			}
			/*
			�ڴ���Ѿ�û���㹻���ڴ���֧�������ˣ�ֻ����heap����ռ���
			����Ŀռ��С����Ҫ���2����Ҫ��һ��
			Ҳ��������heap�ռ䣬���������ڴ�أ�Ϊ�ڴ��ע��Դͷ��ˮ��Ӧ������
			*/
			start_free = (char *)malloc(bytes_to_get);//�µ��ڴ�ص��׵�ַ
			//������������ĵģ�heap�ռ䲻�㣬malloc()ʧ��
			if (start_free==0) {
				//ֻ��ȥ����������ڵ㿴����û�п�����
				obj **my_free_list = 0, *p = 0;
				for (int i = 0; i <= EMaxBytes::MAXBYTES; i += EAlign::ALIGN) {
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (p != 0) {
						*my_free_list = p->next;
						start_free = (char *)p;
						end_free = start_free + i;
						//�ݹ�����Լ�����Ϊ�ڴ�����������˿ռ�
						return chunk_alloc(bytes, nobjs);
					}
				}
				end_free = 0;//����ߵ�����һ�����ʹ���ɽ��ˮ���ˡ������ӵ���������ﲻ������
			}
			heap_size += bytes_to_get;//�����heap_size��bytes_left
			end_free = start_free + bytes_to_get;
			////�ݹ�����Լ�����Ϊ�ڴ�����������˿ռ�
			return chunk_alloc(bytes, nobjs);
		}
	}
}
/*
1.malloc()������
���ܣ����䳤��Ϊbytes�ֽڵ��ڴ��
˵�����������ɹ��򷵻�ָ�򱻷����ڴ��ָ�룬���򷵻ؿ�ָ��NULL
���ڴ治��ʹ��ʱ��Ӧʹ��free()�������ڴ���ͷ�
2.static_cast��
�÷���static_cast < type-id > ( expression )�����������expressionת��Ϊtype-id���͡���Ҫ��;���£�
��1�����ڻ�����������֮���ת��
��2���ѿ�ָ��ת����Ŀ�����͵Ŀ�ָ��
��3�����κ����͵ı��ʽ����ת����void����
��4���������νṹ�и��������֮��ָ������õ�ת��
*/