#ifndef _ALLOC_H_
#define _ALLOC_H_
#include <cstdlib>
/*
就是C中的stdlib.h，在C++中调用该头文件中的函数需要引用cstdlib.用法与C语言中的stdlib.h相同
stdlib 头文件即standard library标准库头文件。stdlib.h里面定义了五种类型、一些宏和通用工具函数。
类型例如size_t、wchar_t、div_t、ldiv_t和lldiv_t； 宏例如EXIT_FAILURE、EXIT_SUCCESS、RAND_MAX和MB_CUR_MAX等等；
常用的函数如malloc()、calloc()、realloc()、free()、system()、atoi()、atol()、rand()、srand()、exit()等等。
*/
namespace WKstl {
	class alloc {
	private:
		enum EAlign {ALIGN=8};//小型区块的上调边界，以8bytes为一个间隔
		enum EMaxBytes{ MAXBYTES = 128 };//小型区块的上限
		enum ENFreeLists {NFREELISTS=(EMaxBytes::MAXBYTES)/(EAlign::ALIGN)};//free-lists个数
		enum ENObjs {NOBJS=20};//每次增加的节点数
	private:
		/*
		obj是free-lists节点结构，这种结构相当巧妙，一物两用。
		从第一字段观察，obj可被视为一个指针，指向相同形式的另一个obj。
		从第二字段观察，obj可被视为一个指针，指向实际区块
		*/
		union obj {
			union obj *next;
			char client[1];
		};
		//表示free_list这个数组里存放的都是obj类型的指针
		static obj *free_list[ENFreeLists::NFREELISTS];//这就是那个链表
	private:
		static char *start_free;//内存池的起始位置
		static char *end_free;//内存池的结束位置
		static size_t heap_size;//内存池的容量大小
	private:
		//将bytes上调至8的倍数
		//SGI的这种配置器会主动将任何小额区块的内存需求量上调至8的倍数
		static size_t ROUND_UP(size_t bytes) {
			return((bytes+EAlign::ALIGN-1)&~(EAlign::ALIGN-1));
		}
		//根据区块的大小，决定使用第几号free_list，n从0开始计算
		static size_t FREELIST_INDEX(size_t bytes) {
			return((bytes + (EAlign::ALIGN - 1)) / (EAlign::ALIGN - 1));
		}
		//若free list中没有可用区块时，refill()函数会为free list重新填充空间。新的空间将取自内存池
		static void *refill(size_t n);
		//配置一大块空间，可容纳nobjs个大小为size的区块
		//如果配置nobjs个区块有所不便，nobjs可能会降低
		static char *chunk_alloc(size_t size, int &nodjs);
	public:
		//内存空间配置函数
		static void *allocate(size_t bytes);
		//内存空间释放函数，区块回收
		static void deallocate(void *ptr, size_t bytes);
		//重新分配内存空间
		static void *reallocate(void *ptr, size_t old_sz, size_t new_sz);
	};
}
/*
1.enum
枚举的定义：
enum 类型名 {枚举值表}；
类型名是变量名，指定枚举类型的名称。
枚举值表也叫枚举元素列表，列出定义的枚举类型的所有可用值，各个值之间用“,”分开。
2.union
（1）什么是联合？
“联合”是一种特殊的类，也是一种构造类型的数据结构。在一个“联合”内可以定义多种不同的数据类型， 一个被说明为该“联合”类型的变量中，
允许装入该“联合”所定义的任何一种数据，这些数据共享同一段内存，已达到节省空间的目的（还有一个节省空间的类型：位域）。 这是一个非常
特殊的地方，也是联合的特征。另外，同struct一样，联合默认访问权限也是公有的，并且，也具有成员函数。
（2）联合与结构的区别？
“联合”与“结构”有一些相似之处。但两者有本质上的不同。在结构中各成员有各自的内存空间， 一个结构变量的总长度是各成员长度之和（空结
构除外，同时不考虑边界调整）。而在“联合”中，各成员共享一段内存空间， 一个联合变量的长度等于各成员中最长的长度。应该说明的是， 这里
所谓的共享不是指把多个成员同时装入一个联合变量内， 而是指该联合变量可被赋予任一成员值，但每次只能赋一种值， 赋入新值则冲去旧值。
*/
#endif