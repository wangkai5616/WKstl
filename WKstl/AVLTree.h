#ifndef _AVL_TREE_H_
#define _AVL_TREE_H_
#include "Allocator.h"
#include "Iterator.h"
#include "Stack.h"
#include <string>
#include <set>
namespace WKstl {
	namespace Detail {
		template<class T>
		class avl_iter;
	}
	template<class T>
	class avl_tree {
	private:
		template<class T>
		friend class Detail::avl_iter;
	private:
		struct node {
			T data_;
			node* left_;
			node* right_;
			size_t height_;
			typedef T value_type;
			explicit node(T d = T(), node *l = 0, node *r = 0, size_t h = 1)
				:data_(d), left_(l), right_(r), height_(h) {}
		};
		typedef WKstl::allocator<node> dataAllocator;
	public:
		typedef T value_type;
		typedef Detail::avl_iter<node> const_iterator;
		typedef const T& const_reference;
		typedef size_t size_type;
	private:
		node* root_;
		size_t size_;
	public:
		avl_tree() :root_(0), size_(0) {}
		avl_tree(const avl_tree&) = delete;
		avl_tree& operator=(const avl_tree&) = delete;
		~avl_tree();

		void insert(const T& val);
		template<class Iterator>
		void insert(Iterator first, Iterator last);
		void erase(const T& val);

		size_t height()const { return getHeight(root_); }
		size_t size()const { return size_; }
		bool empty()const { return root_ == 0; }
		const_iterator root()const { return const_iterator(boot_, this); }

		const_iterator cbegin()const { return find_min(); }
		const_iterator cend()const { return const_iterator(0, this); }

		const_iterator find_min()const;
		const_iterator find_max()const;
		const_iterator find(const T& val)const;

		void print_preorder(const std::string& delim = " ", std::ostream& os = std::cout)const;
		void print_inorder(const std::string& delim = " ", std::ostream& os = std::cout)const;
		void print_postorder(const std::string& delim = " ", std::ostream& os = std::cout)const;
		void print_levelorder(const std::string& delim = " ", std::ostream& os = std::cout)const;
	private:
		node *singleLeftLeftRotate(node *k2);
		node *doubleLeftRightRotate(node *k3);
		node *doubleRightLeftRotate(node *k3);
		node *singleRightRightRotate(node *k2);
	private:
		void insert_elem(const T& val, node *&p);
		void erase_elem(const T& val, node *&p);
		void destroyAndDeallocateAllNodes(node *p);
		size_t getHeight(const node *p)const { return p == 0 ? 0 : p->height_; }
		const_iterator find_min_aux(const node *ptr)const;
		const_iterator find_max_aux(const node *ptr)const;
		const_iterator find_aux(const T& val, const node *ptr)const;
		void print_preorder_aux(const std::string& delim, std::ostream& os, const node *ptr)const;
		void print_inorder_aux(const std::string& delim, std::ostream& os, const node *ptr)const;
		void print_postorder_aux(const std::string& delim, std::ostream& os, const node *ptr)const;
	};
	namespace Detail {
		template<class T>//这里的T其实就是指node ,listIterator中的node是作为成员而存在的
		class avl_iter :
			public iterator<forward_iterator_tag, typename avl_tree<typename T::value_type>::value_type> {
		private:
			template<class T>
			friend class avl_tree;
		private:
			typedef typename avl_tree<typename T::value_type>::const_reference const_reference;
			typedef typename const T::value_type *const_pointer;
			typedef const avl_tree<typename T::value_type> *cntrPtr;
		private:
			const T *ptr_;
			cntrPtr container_;
			stack<const T*> parent_;//保存从root到ptr_的父节点的路径
			std::set<const T*>visited_;//当前节点是否被访问过（若当前节点被访问说明其左子树已被访问了）
		public:
			//注意：这里的ptr不是指向元素的指针，而是指向节点node的指针 
			//container是指向AVLtree的指针
			avl_iter(const T *ptr, cntrPtr container);
			//operator是重载操作符
			//这里是重载const T*() ???????????????????????????????????
			operator const T*()const { return ptr_; }
			const_reference operator*()const { return ptr_->data_; }
			const_pointer operator->()const { return &(operator*()); }
			
			avl_iter left()const { return avl_iter(ptr_->left_, container_); }
			avl_iter right()const { return avl_iter(ptr_->right_, container_); }

			avl_iter& operator++();//前置++
			avl_iter operator ++(int);//后置++
		public:
			template<class T>
			friend bool operator==(const avl_iter<T>& it1, const avl_iter<T>& it2);
			template<class T>
			friend bool operator!=(const avl_iter<T>& it1, const avl_iter<T>& it2);
		};
	}
}
#endif
