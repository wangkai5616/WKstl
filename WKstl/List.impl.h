#ifndef _LIST_IMPL_H_
#define _LIST_IMPL_H_
namespace WKstl {
	namespace Detail {
		template<class T>
		listIterator<T>& listIterator<T>::operator++() {
			p = p->next; //nodePtr p,p是一个节点类型的指针
			return *this;
		}
		template<class T>
		listIterator<T> listIterator<T>::operator++(int) {
			auto res = *this;
			++*this;
			return res;
		}
		template<class T>
		listIterator<T>& listIterator<T>::operator--() {
			p = p->prev; 
			return *this;
		}
		template<class T>
		listIterator<T> listIterator<T>::operator--(int) {
			auto res = *this;
			--*this;
			return res;
		}
		template<class T>
		bool operator==(const listIterator<T>& lhs, const listIterator<T>& rhs) {
			return lhs.p == rhs.p;
		}
		template<class T>
		bool operator!=(const listIterator<T>& lhs, const listIterator<T>& rhs) {
			return !(lhs.p == rhs.p);
		}
	}

	//插入操作相关的系列函数
	//?????????????????????????????????true_type和false_type到底是什么鬼，感觉没必要的
	template<class T>
	void list<T>::insert_aux(iterator position, size_type n, const T& val, std::true_type) {
		for (auto i = n; i != 0; --i) {
			position = insert(position, val);
		}
	}
	template<class T>
	template<class InputIterator>
	void list<T>::insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type) {
		////迭代器所指范围左开右闭，所以last首先需要--
		for (--last; first != last; --last) {
			position = insert(position, *last);//从后向前插入
		}
		insert(position, *last);//最后插入first所指内容
	}
	template<class T>
	typename list<T>::iterator list<T>::insert(iterator position, const value_type& val) {
		if (position == begin()) {
			push_front(val);
			return begin();
		}
		else if(position==end()){
			auto ret = position;
			push_back(val);
			return ret;
		}
		/*
		typedef Detail::listIterator<T> iterator
		typedef node<T>* nodePtr,nodePtr代表此指针是节点类型
		newNode(val)的返回类型是nodePtr
		*/
		auto node = newNode(val);
		auto prev = position.p->prev;
		node->next = position.p;
		node->prev = prev;
		prev->next = node;
		position.p->prev = node;
		return iterator(node);//？？？？？？？？？？？？？？？？？？？？？？？？？留有疑问
	}
	template<class T>
	void list<T>::insert(iterator position, size_type n, const value_type& val) {
		insert_aux(position, n, val, typename std::is_integral<InputIterator>::type());
	}
	template<class T>
	template<class InputIterator>
	void list<T>::insert(iterator position, InputIterator first, InputIterator last) {
		insert_aux(position, first, last, typename std::is_integral<InputIterator>:: type());
	}

	//创建一个新的节点
	template<class T>
	typename list<T>::nodePtr list<T>::newNode(const T& val = T()) {
		//typedef allocator<Detail::node<T>> nodeAllocator,分配单位不是T，而是节点
		nodePtr res = nodeAllocator::allocate();
		/*
		template<class T>
		void allocator<T>::construct(T *ptr, const T& value) {
		new (ptr)T(value);
		}
		*/
		nodeAllocator::construct(res, Detail::node<T>(val, nullptr, nullptr, this));
		return res;
	}

	//销毁一个节点，释放空间
	template<class T>
	void list<T>::deleteNode(nodePtr p) {
		p->prev = nullptr;
		p->next = nullptr;
		nodeAllocator::destroy(p);
		nodeAllocator::deallocate(p);
	}

	template<class T>
	void list<T>::ctorAux(size_type n, const value_type& val, std::true_type) {
		head.p = newNode();
		tail.p = head.p;
		while (n--)
			push_back(val);
	}
	template<class T>
	template<class InputIterator>
	void list<T>::ctorAux(InputIterator first, InputIterator last, std::false_type) {
		head.p = newNode();
		tail.p = head.p;
		for (; first != last; ++first) {
			push_back(*first);
		}
	}

	//list系列构造函数
	template<class T>
	list<T>::list() {
		head.p = newNode();
		tail.p = head.p;
	}
	template<class T>
	list<T>::list(size_type n, const value_type& val = value_type()) {
		ctorAux(n, val, std::is_integral<value_type>());
	}
	template<class T>
	template<class InputIterator>
	list<T>::list(InputIterator first, InputIterator last) {
		ctorAux(first, last, std::is_integral<InputIterator>());
	}
	template<class T>
	list<T>::list(const list& l) {
		head.p = newNode();
		tail.p = head.p;
		for (auto node = l.head.p; node != l.tail.p; node = node->next)
			push_back(node->data);
	}

	//list析构函数
	template<class T>
	list<T>::~list() {
		for (; head != tail;) {
			auto temp = head++;
			nodeAllocator::destroy(temp.p);
			nodeAllocator::deallocate(temp.p);
		}
		nodeAllocator::deallocate(tail.p);
	}

	template<class T>
	typename list<T>::size_type list<T>::size()const {
		size_type length = 0;
		for (auto h = head; h != tail; ++h)
			++length;
		return length;
	}
 	
	//*this代表当前链表
	/*
	巧妙之处：
	链表l是const &,所以swap操作不会改变l，而且会把链表
	内容拷贝给*this
	*/
	template<class T>
	list<T>& list<T>::operator=(const list& l) {
		if (this != &l) {
			list(l).swap(*this);
		}
		return *this;
	}
	
	//list在头部和尾部插入删除节点操作
	template<class T>
	void list<T>::push_front(const value_type& val) {
		auto node = newNode(val);
		head.p->prev = node;
		node->next = head.p;
		head.p = node;
	}
	template<class T>
	void list<T>::pop_front() {
		auto oldNode = head.p;
		head.p = oldNode->next;
		(head.p)->prev = nullptr;
		deleteNode(oldNode);
	}
	template<class T>
	void list<T>::push_back(const value_type& val) {
		auto node = newNode();
		(tail.p)->data = val;
		(tail.p)->next = node;
		node->prev = tail.p;
		tail.p = node;
	}
//?????????????????????????????????????????????????未解之谜??????????????????????????????????????????
	template<class T>
	void list<T>::pop_back() {
		auto newTail = tail.p->prev;
		newTail->next = nullptr;
		deleteNode(tail.p);
		tail.p = newTail;
	}
	//删除迭代器所指节点并返回一个迭代器指向下一个节点
	template<class T>
	typename list<T>::iterator list<T>::erase(iterator position) {
		if (position == head) {
			pop_front();
			return head;
		}
		else {
			auto prev = position.p->prev;
			prev->next = position.p->next;
			position.p->next->prev = prev;
			deleteNode(position.p);
			return iterator(prev->next);
		}
	}
	//删除一对迭代器内的节点，左开右闭
	template<class T>
	typename list<T>::iterator list<T>::erase(iterator first, iterator last) {
		typename list<T>::iterator res;
		for (; first != last;) {
			auto temp = first++;
			res = erase(temp);
		}
		return res;
	}
	template<class T>
	void list<T>::clear() {
		erase(begin(), end());
	}
	template<class T>
	typename list<T>::iterator list<T>::begin() {
		return head;
	}
	template<class T>
	typename list<T>::iterator list<T>::end() {
		return tail;
	}
	
	template<class T>
	typename list<T>::const_iterator list<T>::changeIteratorToConstIterator(iterator& it)const {
		using nodeP = Detail::node<const T>*;
		auto temp = (list<const T>*const)this;//this是指向整个list的指针
		auto ptr = it.p;
		Detail::node<const T> node(ptr->data, (nodeP)(ptr->prev), (nodeP)(ptr->next), temp);
		return const_iterator(&node);
	}
	template<class T>
	typename list<T>::const_iterator list<T>::begin()const {
		auto temp = (list*const)this;
		return changeIteratorToConstIterator(temp->head);
	}
	template<class T>
	typename list<T>::const_iterator list<T>::end()const {
		auto temp = (list*const)this;
		return changeIteratorToConstIterator(temp->tail);
	}
	template<class T>
	typename list<T>::reverse_iterator list<T>::rbegin() {
		return reverse_iterator(tail);
	}
	template<class T>
	typename list<T>::reverse_iterator list<T>::rend() {
		return reverse_iterator(head);
	}
	//详解见本人博客
	template<class T>
	void list<T>::reverse() {
		if (empty() || head.p->next == tail.p)
			return;
		auto curNode = head.p;
		head.p = tail.p->prev;
		head.p->prev = nullptr;
		do {
			auto nextNode = curNode->next;
			curNode->next = head.p->next;
			head.p->next->prev = curNode;
			head.p->next = curNode;
			curNode->prev = head.p;
			curNode = nextNode;
		} while (curNode != head.p);
	}
	template<class T>
	void list<T>::remove(const value_type& val) {
		for (auto it = begin(); it != end();) {
			if (*it == val)
				it = erase(it);//因为erase()操作完成后返回的迭代器指向下一个节点，所以不需要++操作
			else
				++it;
		}
	}
	template<class T>
	template<class Predicate>
	void list<T>::remove_if(Predicate pred) {
		for (auto it = begin(); it != end();) {
			if (pred(*it))
				it = erase(it);//因为erase()操作完成后返回的迭代器指向下一个节点，所以不需要++操作
			else
				++it;
		}
	}
	
	template<class T>
	void list<T>::swap(list& x) {
		WKstl::swap(head.p, x.head.p);
		WKstl::swap(tail.p, x.tail.p);
	}
	template<class T>
	void swap(list<T>& x, list<T>& y) {
		x.swap(y);
	}
	//unique()适用的情况是list是非递减的
	template<class T>
	void list<T>::unique() {
		nodePtr curNode = head.p;
		while (curNode != tail.p) {
			nodePtr nextNode = curNode->next;
			if (curNode->data == nextNode->data) {
				/*
				这种情况有可能发生，因为空白节点中存放的是T类型的默认值
				比如T是int，则空白节点中存在的是0,此时将tail.p析构掉，
				把curNode作为尾节点
				*/
				if (nextNode == tail.p) {
					curNode->next = nullptr;
					tail.p = curNode;
				}
				else {
					curNode->next = nextNode->next;
					nextNode->next->prev = curNode;
				}
				deleteNode(nextNode);
			}
			else {
				curNode = nextNode;
			}
		}
	}
	template<class T>
	template<class BinaryPredicate>
	void list<T>::unique(BinaryPredicate binary_pred) {
		nodePtr curNode = head.p;
		while (curNode != tail.p) {
			nodePtr nextNode = curNode->next;
			if (binary_pred(curNode->data,nextNode->data)) {
				if (nextNode == tail.p) {
					curNode->next = nullptr;
					tail->p = curNode;
				}
				else {
					curNode->next = next->nextNode->next;
					nextNode->next->prev = curNode;
				}
				deleteNode(nextNode);
			}
		}
	}
	/*
	splice:
	实现list拼接的功能。将源list的内容部分或全部元素删除，拼插入到目的list
	将x的元素移动到目的list的指定位置，高效的将他们插入到目的list并从x中删除。
    目的list的大小会增加，增加的大小为插入元素的大小。x的大小相应的会减少同样的大小
	*/
	template<class T>
	void list<T>::splice(iterator position, list& x) {
		this->insert(position, x.begin(), x.end());
		x.head.p = x.tail.p;
	}
	//void splice(iterator position, list& x, iterator first, iterator last);
	template<class T>
	void list<T>::splice(iterator position, list& x, iterator first, iterator last) {
		if (first.p == last.p) return;
		auto tailNode = last.p->prev;
		//第一对if else是按照first.p是否为链表头节点分别进行处理
		if (x.head.p == first.p) {
			x.head.p = last.p;
			x.head.p->prev = nullptr;
		}
		else {
			first.p->prev->next = last.p;
			last.p->prev = first.p->prev;
		}
		//第二对if else是按照position与head.p的位置关系分别进行处理
		if (position.p == head.p) {
			first.p->prev = nullptr;
			tailNode->next = head.p;
			head.p->prev = tailNode;
			head.p = first.p;
		}
		else {
			position.p->prev->next = first.p;
			first.p->prev = position.p->prev;
			tailNode->next = position.p;
			position.p->prev = tailNode;
		}
	}
	template<class T>
	void list<T>::splice(iterator position, list& x, iterator i) {
		auto next = i;
		this->splice(position, x, i, ++next);
	}
	/*
	merge:
	将两个有序序列合并成一个有序序列
	总的思路就是在第一个链表中找到第二个链表中节点的插入位置
	*/
	template<class T>
	void list<T>::merge(list& x) {
		auto it1 = begin(), it2 = x.begin();
		while (it1 != end() && it2 != x.end()) {
			if (*it1 < *it2)
				++it1;
			else{
				auto temp = it2++;
				this->splice(it1, x, temp);
			}
		}
		if (it1 == end()) {
			this->splice(it1, x, it2, x.end());
		}
	}
	template<class T>
	template <class Compare>
	void list<T>::merge(list& x, Compare comp) {
		auto it1 = begin(), it2 = x.begin();
		while (it1 != end() && it2 != x.end()) {
			if (comp(*it2, *it1)) {
				auto temp = it2++;
				this->splice(it1, x, temp);
			}
			else
				++it1;
		}
		if (it1 == end()) {
			this->splice(it1, x, it2, x.end());
		}
	}
	//friend bool operator==(const list<T>& lhs, const list<T>& rhs);
	template<class T>
	bool operator==(const list<T>& lhs, const list<T>& rhs) {
		auto node1 = lhs.head.p, node2 = rhs.head.p;
		for (; node1 != lhs.tail.p && node2 != rhs.tail.p; node1 = node1->next, node2 = node2->next) {
			if (node1->data != node2->data)
				break;
		}
		if (node1 == lhs.tail.p && node2 == rhs.tail.p) {
			return true;
		}
		return false;
	}
	template<class T>
	bool operator!=(const list<T>& lhs, const list<T>& rhs) {
		return !(lhs == rhs);
	}

	/*
	list不能使用STL算法sort()，必须使用自己的sort()member function,
	因为STL算法sort()只接受RandomAccessIterator
	*/
	template<class T>
	void list<T>::sort() {
		sort(WKstl::less<T>());
	}
	template<class T>
	template<class Compare>
	void list<T>::sort(Compare comp) {
		if (empty() || head.p->next == tail.p)
			return;
		list carry;
		list counter[64];
		int fill = 0;
		while (!empty()) {
			carry.splice(carry.begin(), *this, begin());
			int i = 0;
			while (i < fill && !counter[i].empty()) {
				counter[i].merge(carry, comp);
				carry.swap(counter[i++]);
			}
			carry.swap(counter[i]);
			if (i == fill)
				++fill;
		}
		for (int i = 0; i != fill; ++i) {
			counter[i].merge(counter[i - 1], comp);
		}
		swap(counter[fill - 1]);
	}
}
/*
1.typename list<T>::iterator list<T>::insert(iterator position, const value_type& val)
这里需要typename的原因是告诉编译器iterator是类型名称
这个时候typename的作用就是告诉c++编译器，typename后面的字符串为一个类型名称，
而不是成员函数或者成员变量，这个时候如果前面没有
typename，编译器没有任何办法知道list<T>::LengthType是一个类型还是一个成员名称(静态数据成员或者静态函数)，
所以编译不能够通过。
*/
#endif