#include "Alloc.h"
namespace WKstl {
	//内存池起始地址
	char *alloc::start_free = 0;
	//内存池结束地址
	char *alloc::end_free = 0;
	//内存池初始容量大小
	size_t alloc::heap_size = 0;
	//obj属于类alloc,free_list属于类alloc，这都需要进行说明
	//这就是那16个free-lists
	alloc::obj *alloc::free_list[alloc::ENFreeLists::NFREELISTS] =
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	void *alloc::allocate(size_t bytes) {
		//大于128则利用malloc()直接分配内存
		if (bytes > EMaxBytes::MAXBYTES) {
			return malloc(bytes);
		}
		//寻找16个free lists中适当的一个
		size_t index = FREELIST_INDEX(bytes);
		//list这个链就是存放的某个字节对应的空余区块
		obj *node = free_list[index];
		//还有区块可以使用
		if (node) {
			free_list[index] = node->next;//下一个可用区块
			return node;//返回可用区块
		}
		//此时，无可用区块，去找内存池。ROUND_UP(bytes)是上调至8的倍数
		else {
			return refill(ROUND_UP(bytes));
		}
	}
	void alloc::deallocate(void *ptr, size_t bytes) {
		//大于128则利用free()直接释放内存
		if (bytes > EMaxBytes::MAXBYTES) {
			free(ptr);
		}
		//将区块还给16个free lists中适当的一个
		else{
			size_t index = FREELIST_INDEX(bytes);
			//将void *ptr强制转为obj *
			obj *node = static_cast<obj *> (ptr);
			//归还的区块成对应的字节链表中的第一个可用区块
			node->next = free_list[index];
			free_list[index] = node;
		}
	}
	void *alloc::reallocate(void *ptr, size_t old_sz, size_t new_sz) {
		deallocate(ptr, old_sz);
		ptr = allocate(new_sz);
		return ptr;
	}
	//调用refill的地方bytes是8的倍数
	void *alloc::refill(size_t bytes) {
		int nobjs = 20;//每次希望获得20个新区块
		/*调用chunk_alloc(),尝试获得nobjs个区块作为free_list的新节点，
		但如果内存池中区块数不足20个，将会更新nobjs的值
		*/
		char *chunk = chunk_alloc(bytes, nobjs);//注意：chunk现在保存的是从内存池中拿到的区块的起始地址
		obj **my_free_list=0;//如果有多余的区块，放到这里
		obj *result = 0;
		obj *current_obj = 0, *next_obj = 0;
		//如果只获得一个区块，这个区块就分配给调用者用，free_list无新节点
		if (nobjs == 1) {
			return chunk;
		}
		//如果是多于一个区块，则调整free_list，纳入新节点，即相应的free_list
		else {
			//先找到放多余区块的位置
			my_free_list = free_list + FREELIST_INDEX(bytes);
			//这一块返回给客端供使用
			result = (obj *)chunk;
			//将一个区块（bytes）一个区块的放
			*my_free_list = next_obj = (obj *)(chunk + bytes);
			//for循环停止的条件是所有的从内存池中拿到的区块数放到相应的free list
			for (int i = 1; i + 1 < nobjs; i++) {//i+1是因为你已经拿了一个区块给客端
				current_obj = next_obj;
				next_obj = (obj *)((char *)next_obj + bytes);
				current_obj->next = next_obj;
			}
			//如果nobjs-1==i,那么current_obj就是最后一个区块了
			current_obj->next = 0;
		}
		return result;
	}
	//从内存池取空间给free_list使用，其实是chunk_alloc()的工作
	char *alloc::chunk_alloc(size_t bytes, int &nobjs) {//这里的nobjs是引用，因为有可能更新nobjs的值
		char *result = 0;
		size_t total_bytes = bytes*nobjs;
		size_t bytes_left = end_free - start_free;//这是内存池的剩余空间，也就是内存池起始地址和结束地址的间隔
		//内存池剩余空间完全满足需求量
		if (bytes_left > total_bytes) {
			result = start_free;
			start_free = start_free + total_bytes;//更新内存池的起始地址
			return result;//这就是给free list的起始地址
		}
		//内存池剩余空间不能完全满足需求量，但足够供应一个以上的区块
		else if (bytes_left>=bytes){
			nobjs = bytes_left / bytes;//更新nobjs的值，因为不够20个区块，看看内存池能拿出多少区块
			total_bytes = bytes*nobjs;//由更新的nobjs值更新total_bytes的值
			result = start_free;
			start_free += total_bytes;//更新内存池的起始地址
			return result;
		}
		//内存池剩余空间连一个区块的大小都无法提供
		else {
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			//不够一个区块，但是也不是一点都没有
			if (bytes_left > 0) {
				obj **my_free_list = free_list + FREELIST_INDEX(bytes);
				//将这不足一个区块的空间给了内存池
				((obj *)start_free)->next = *my_free_list;
				*my_free_list = (obj *)start_free;
			}
			/*
			内存池已经没有足够的内存来支付申请了，只好向heap申请空间了
			申请的空间大小是所要求的2倍还要多一点
			也就是配置heap空间，用来补充内存池；为内存池注入源头活水以应付需求
			*/
			start_free = (char *)malloc(bytes_to_get);//新的内存池的首地址
			//下面这是最恶心的，heap空间不足，malloc()失败
			if (start_free==0) {
				//只能去链表的其他节点看看有没有空余了
				obj **my_free_list = 0, *p = 0;
				for (int i = 0; i <= EMaxBytes::MAXBYTES; i += EAlign::ALIGN) {
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (p != 0) {
						*my_free_list = p->next;
						start_free = (char *)p;
						end_free = start_free + i;
						//递归调用自己，因为内存池里面又有了空间
						return chunk_alloc(bytes, nobjs);
					}
				}
				end_free = 0;//如果走到了这一步，就代表山穷水尽了。更复杂的情况，这里不做考虑
			}
			heap_size += bytes_to_get;//这里的heap_size和bytes_left
			end_free = start_free + bytes_to_get;
			////递归调用自己，因为内存池里面又有了空间
			return chunk_alloc(bytes, nobjs);
		}
	}
}
/*
1.malloc()函数：
功能：分配长度为bytes字节的内存块
说明：如果分配成功则返回指向被分配内存的指针，否则返回空指针NULL
当内存不再使用时，应使用free()函数将内存块释放
2.static_cast：
用法：static_cast < type-id > ( expression )，该运算符把expression转换为type-id类型。主要用途如下：
（1）用于基本数据类型之间的转换
（2）把空指针转换成目标类型的空指针
（3）把任何类型的表达式类型转换成void类型
（4）用于类层次结构中父类和子类之间指针和引用的转换
*/