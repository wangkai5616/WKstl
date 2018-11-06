#ifndef _BINARY_SEARCH_TREE_H_
#define _BINARY_SEARCH_TREE_H_

#include "Allocator.h"
#include "Iterator.h"
#include "Queue.h"
#include "Stack.h"
#include<string>

#include <set>

namespace WKstl {
	namespace Detail {
		template<class T>
		class bst_iter;
	}
	//class of binary_search_tree
	template<class T>
	class binary_search_tree {
	private:
		template<class T>
		friend class Detail::bst_iter;
	private:
		struct node {
			typedef T value_type;
			T data_;
			node *left_;
			node *right_;
			explicit node(T d = T(), node *l = 0, node *r = 0)
				:data_(d), left_(l), right_(r) {}
		};
		typedef WKstl::allocator<node> nodeAllocator;
	public:
		typedef T value_type;
		typedef Detail::bst_iter<node> const_iterator;
		typedef const T& const_reference;
		typedef size_t size_type;
	private:
		node *root_;
		size_t size_;
	public:
		binary_search_tree() :root_(0), size_(0) {}
		binary_search_tree(const binary_search_tree&) = delete;
		binary_search_tree& operator=(const binary_search_tree&) = delete;
		~binary_search_tree();

		void insert(const T& val);
		template<class Iterator>
		void insert(Iterator first, Iterator last);
		void erase(const T& val);

		bool empty()const { return root_ == 0; }
		size_t size()const { return size_; }
		size_t height()const { return height_aux(root_); }
		const_iterator root()const { return const_iterator(root_, this); }

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
		void deallocateAllNodes(node *ptr);
		size_t height_aux(node *p)const;
		void erase_elem(const T& val, node *&ptr);
		void insert_elem(const T& val, node *&ptr);
		const_iterator find_min_aux(const node *ptr)const;
		const_iterator find_max_aux(const node *ptr)const;
		const_iterator find_aux(const T& val, const node *ptr)const;
		void print_preorder_aux(const std::string& delim, std::ostream& os, const node *ptr)const;
		void print_inorder_aux(const std::string& delim, std::ostream& os, const node *ptr)const;
		void print_postorder_aux(const std::string& delim, std::ostream& os, const node *ptr)const;
	};//end of bst class

	namespace Detail {
		//class of bst iterator
		template<class T>//T = node
		class bst_iter :
			public iterator<forward_iterator_tag, typename ::WKstl::binary_search_tree<typename T::value_type>::value_type> {
		private:
			template<class T>
			friend class ::WKstl::binary_search_tree;
		private:
			//typedef typename ::TinySTL::binary_search_tree<typename T::value_type>::value_type value_type;
			typedef typename ::WKstl::binary_search_tree<typename T::value_type>::const_reference const_reference;
			typedef typename const T::value_type *const_pointer;
			typedef const ::WKstl::binary_search_tree<typename T::value_type> * cntrPtr;
		private:
			const T *ptr_;
			cntrPtr container_;
			stack<const T *> parent_;//�����root��ptr_�ĸ��ڵ��·��
									 //std::set<const T *> rvisited_;//��ǰ�ڵ���������Ƿ񱻷��ʹ�
			std::set<const T *> visited_;//��ǰ�ڵ��Ƿ񱻷��ʹ�����node������˵�����������ѱ������ˣ�
		public:
			bst_iter(const T *ptr, cntrPtr container);

			operator const T*() { return ptr_; }
			const_reference operator*()const { return ptr_->data_; }
			const_pointer operator ->()const { return &(operator*()); }

			bst_iter left()const { return bst_iter(ptr_->left_, container_); }
			bst_iter right()const { return bst_iter(ptr_->right_, container_); }

			bst_iter& operator ++();
			bst_iter operator ++(int);
		public:
			template<class T>
			friend bool operator ==(const bst_iter<T>& it1, const bst_iter<T>& it2);
			template<class T>
			friend bool operator !=(const bst_iter<T>& it1, const bst_iter<T>& it2);
		};//end of bst_iter
	}
}
#endif