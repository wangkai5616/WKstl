#ifndef _LIST_IMPL_H_
#define _LIST_IMPL_H_
namespace WKstl {
	namespace Detail {
		template<class T>
		listIterator<T>& listIterator<T>::operator++() {
			p = p->next; //nodePtr p,p��һ���ڵ����͵�ָ��
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

	//���������ص�ϵ�к���
	//?????????????????????????????????true_type��false_type������ʲô���о�û��Ҫ��
	template<class T>
	void list<T>::insert_aux(iterator position, size_type n, const T& val, std::true_type) {
		for (auto i = n; i != 0; --i) {
			position = insert(position, val);
		}
	}
	template<class T>
	template<class InputIterator>
	void list<T>::insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type) {
		////��������ָ��Χ���ұգ�����last������Ҫ--
		for (--last; first != last; --last) {
			position = insert(position, *last);//�Ӻ���ǰ����
		}
		insert(position, *last);//������first��ָ����
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
		typedef node<T>* nodePtr,nodePtr�����ָ���ǽڵ�����
		newNode(val)�ķ���������nodePtr
		*/
		auto node = newNode(val);
		auto prev = position.p->prev;
		node->next = position.p;
		node->prev = prev;
		prev->next = node;
		position.p->prev = node;
		return iterator(node);//����������������������������������������������������������
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

	//����һ���µĽڵ�
	template<class T>
	typename list<T>::nodePtr list<T>::newNode(const T& val = T()) {
		//typedef allocator<Detail::node<T>> nodeAllocator,���䵥λ����T�����ǽڵ�
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

	//����һ���ڵ㣬�ͷſռ�
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

	//listϵ�й��캯��
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

	//list��������
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
 	
	//*this����ǰ����
	/*
	����֮����
	����l��const &,����swap��������ı�l�����һ������
	���ݿ�����*this
	*/
	template<class T>
	list<T>& list<T>::operator=(const list& l) {
		if (this != &l) {
			list(l).swap(*this);
		}
		return *this;
	}
	
	//list��ͷ����β������ɾ���ڵ����
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
//?????????????????????????????????????????????????δ��֮��??????????????????????????????????????????
	template<class T>
	void list<T>::pop_back() {
		auto newTail = tail.p->prev;
		newTail->next = nullptr;
		deleteNode(tail.p);
		tail.p = newTail;
	}
	//ɾ����������ָ�ڵ㲢����һ��������ָ����һ���ڵ�
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
	//ɾ��һ�Ե������ڵĽڵ㣬���ұ�
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
		auto temp = (list<const T>*const)this;//this��ָ������list��ָ��
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
	//�������˲���
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
				it = erase(it);//��Ϊerase()������ɺ󷵻صĵ�����ָ����һ���ڵ㣬���Բ���Ҫ++����
			else
				++it;
		}
	}
	template<class T>
	template<class Predicate>
	void list<T>::remove_if(Predicate pred) {
		for (auto it = begin(); it != end();) {
			if (pred(*it))
				it = erase(it);//��Ϊerase()������ɺ󷵻صĵ�����ָ����һ���ڵ㣬���Բ���Ҫ++����
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
	//unique()���õ������list�Ƿǵݼ���
	template<class T>
	void list<T>::unique() {
		nodePtr curNode = head.p;
		while (curNode != tail.p) {
			nodePtr nextNode = curNode->next;
			if (curNode->data == nextNode->data) {
				/*
				��������п��ܷ�������Ϊ�հ׽ڵ��д�ŵ���T���͵�Ĭ��ֵ
				����T��int����հ׽ڵ��д��ڵ���0,��ʱ��tail.p��������
				��curNode��Ϊβ�ڵ�
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
	ʵ��listƴ�ӵĹ��ܡ���Դlist�����ݲ��ֻ�ȫ��Ԫ��ɾ����ƴ���뵽Ŀ��list
	��x��Ԫ���ƶ���Ŀ��list��ָ��λ�ã���Ч�Ľ����ǲ��뵽Ŀ��list����x��ɾ����
    Ŀ��list�Ĵ�С�����ӣ����ӵĴ�СΪ����Ԫ�صĴ�С��x�Ĵ�С��Ӧ�Ļ����ͬ���Ĵ�С
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
		//��һ��if else�ǰ���first.p�Ƿ�Ϊ����ͷ�ڵ�ֱ���д���
		if (x.head.p == first.p) {
			x.head.p = last.p;
			x.head.p->prev = nullptr;
		}
		else {
			first.p->prev->next = last.p;
			last.p->prev = first.p->prev;
		}
		//�ڶ���if else�ǰ���position��head.p��λ�ù�ϵ�ֱ���д���
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
	�������������кϲ���һ����������
	�ܵ�˼·�����ڵ�һ���������ҵ��ڶ��������нڵ�Ĳ���λ��
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
	list����ʹ��STL�㷨sort()������ʹ���Լ���sort()member function,
	��ΪSTL�㷨sort()ֻ����RandomAccessIterator
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
������Ҫtypename��ԭ���Ǹ��߱�����iterator����������
���ʱ��typename�����þ��Ǹ���c++��������typename������ַ���Ϊһ���������ƣ�
�����ǳ�Ա�������߳�Ա���������ʱ�����ǰ��û��
typename��������û���κΰ취֪��list<T>::LengthType��һ�����ͻ���һ����Ա����(��̬���ݳ�Ա���߾�̬����)��
���Ա��벻�ܹ�ͨ����
*/
#endif