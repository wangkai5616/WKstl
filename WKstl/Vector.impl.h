#ifndef _VECTOR_IMPL_H_
#define _VECTOR_IMPL_H_

namespace WKstl {
	/*****************构造，复制，析构相关的操作******************************/
	template<class T, class Alloc>
	vector<T, Alloc>::~vector() {
		destroyAndDeallocateAll();
	}
	template<class T, class Alloc>
	vector<T, Alloc>::vector(const size_type n) {
		allocateAndFillN(n, value_type());
	}
	template<class T, class Alloc>
	vector<T, Alloc>::vector(const size_type n, const value_type& value) {
		allocateAndFillN(n, value);
	}
	template<class T, class Alloc>
	template<class InputIterator>
	vector<T, Alloc>::vector(InputIterator first, InputIterator last) {
		//处理指针和数字间的区别的函数
		vector_aux(first, last, typename std::is_integral<InputIterator>::type());
	}
	template<class T, class Alloc>
	vector<T, Alloc>::vector(const vector& v) {
		allocateAndCopy(v.start_, v.finish_);
	}
	//右值引用
	template<class T, class Alloc>
	vector<T, Alloc>::vector(vector&& v) {
		start_ = v.start_;
		finish_ = v.finish_;
		endOfStorage_ = v.endOfStorage_;
		v.start_ = v.finish_ = v.endOfStorage_ = 0;
	}
	template<class T, class Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(const vector& v) {
		if (this != &v) {
			allocateAndCopy(v.start_, v.finish_);
		}
		return *this;
	}
	/*
	//vector& operator=(const vector& v)
	template<class T,class Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(const vector& v) {
	if (this != &v) {
	//第一种情况：要赋值的v中的元素个数大于现有容器的容量
	if (v.size() > capacity()) {
	destroyAndDeallocateAll();
	allocateAndCopy(v.start_,v.finish_);

	//destroyAndDeallocateAll();//自己由上面两句改成了这一句
	//start_ = tmp;
	//endOfStorage_ = start_ + (v.end() - v.begin());
	}
	//第二种情况：要赋值的v中的元素个数小于现有容器的元素个数
	else if (size() >= v.size()) {
	iterator i = std::copy(v.begin(), v.end(), begin());
	dataAllocator::destroy(i, finish_);
	finish_ = start_ + v.size();
	}
	//第三种情况：要赋值的v中的元素个数小于现有容器的容量，但大于现有容器的元素个数
	else {
	std::copy(v.begin(), v.begin() + size(), begin());
	WKstl::uninitialized_copy(v.begin() + size(), v.end(), finish_);
	finish_ = start_ + v.size();
	}

	}
	return *this;
	}
	*/

	template<class T, class Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(vector&& v) {
		if (this != &v) {
			destroyAndDeallocateAll();
			start_ = v.start_;
			finish_ = v.finish_;
			endOfStorage_ = v.endOfStorage_;
			v.start_ = v.finish_ = v.endOfStorage_ = 0;
		}
		return *this;
	}
	/******************容量相关的操作****************************/
	//resize() 重新指定容器有效的元素个数
	template<class T, class Alloc>
	void vector<T, Alloc>::resize(size_type n, value_type val = value_type()) {
		if (n < size()) {
			dataAllocator::destroy(start_ + n, finish_);
			finish_ = start_ + n;
		}
		else if (n > size() && n <= capacity()) {
			auto lengthOfInsert = n - size();
			finish_ = WKstl::uninitialized_fill_n(finish_, lengthOfInsert, val);
		}
		else if (n > capacity()) {
			auto lengthOfInsert = n - size();
			T *newStart = dataAllocator::allocate(getNewCapacity(lengthOfInsert));
			T *newFinish = WKstl::uninitialized_copy(begin(), end(), newStart);
			newFinish = WKstl::uninitialized_fill_n(newFinish, lengthOfInsert, val);

			destroyAndDeallocateAll();
			start_ = newStart;
			finish_ = newFinish;
			endOfStorage_ = start_ + n;
		}
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::reserve(size_type n) {
		if (n <= capacity())
			return;
		T *newStart = dataAllocator::allocate(n);
		T *newFinish = WKstl::uninitialized_copy(begin(), end(), newStart);
		destroyAndDeallocateAll();
		start_ = newStart;
		finish_ = newFinish;
		endOfStorage_ = start_ + n;
	}

	/**************修改容器的相关操作*****************************/
	//erase的作用是删除掉某个位置position或一段区域（first, last)中的元素，减少其size
	template<class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator position) {
		return erase(position, position + 1);
	}
	/*
	template<class T,class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last) {
	difference_type lenOfTail = end() - last;
	finish_ = finish_ - (last-first);
	for (; lenOfTail != 0; --lenOfTail) {
	auto temp = first;
	*temp = *(last++);
	}
	return first;
	}
	*/
	template<class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last) {
		iterator i = WKstl::copy(last, finish_, first);
		dataAllocator::destroy(i, finish_);
		finish_ = finish_ - (last - first);
		return first;
	}
	//重新分配一块地址，存入vector中原来的元素和要插入的迭代器之间的元素
	template<class T, class Alloc>
	template<class InputIterator>
	void vector<T, Alloc>::reallocateAndCopy(iterator position, InputIterator first, InputIterator last) {
		difference_type newCapacity = getNewCapacity(last - first);
		T *newStart = dataAllocator::allocate(newCapacity);
		T *newEndOfStorage = newStart + newCapacity;
		T *newFinish = WKstl::uninitialized_copy(begin(), position, newStart);
		newFinish = WKstl::uninitialized_copy(first, last, newFinish);
		newFinish = WKstl::uninitialized_copy(position, end(), newFinish);

		destroyAndDeallocateAll();
		start_ = newStart;
		finish_ = newFinish;
		endOfStorage_ = newEndOfStorage;
	}

	//重新分配一块地址，存入vector中原来的元素和要插入的n个值为val元素
	template<class T, class Alloc>
	void vector<T, Alloc>::reallocateAndFillN(iterator position, const size_type& n, const value_type& val) {
		difference_type newCapacity = getNewCapacity(n);
		T *newStart = dataAllocator::allocate(newCapacity);
		T *newEndOfStorage = newStart + newCapacity;
		T *newFinish = WKstl::uninitialized_copy(begin(), position, newStart);
		newFinish = WKstl::uninitialized_fill_n(newFinish, n, val);
		newFinish = WKstl::uninitialized_copy(position, end(), newFinish);

		destroyAndDeallocateAll();
		start_ = newStart;
		finish_ = newFinish;
		endOfStorage_ = newEndOfStorage;
	}
	/*
	template<class T,class Alloc>
	template<class InputIterator>
	void vector<T, Alloc>::insert_aux(iterator position,
	InputIterator first, InputIterator last,std::false_type) {
	difference_type locationLeft = endOfStorage_ - finish_;
	difference_type locationNeed = distance(first, last);

	if (locationLeft >= locationNeed) {
	//造成判断语句的原因就是copy和uninitialized_copy的区别
	if (finish_ - position > locationNeed) {
	WKstl::uninitialized_copy(finish_ - locationNeed, finish_, finish_);
	//复制过程是从最后的元素开始复制，直到首元素复制出来。也就是说，复制操作是从last-1开始，直到first结束
	//先复制到的地址是finish_，然后向前
	std::copy_backward(position, finish_ - locationNeed, finish_);
	//复制过程是从first元素开始复制，直到last-1元素复制出来。先复制到的地址是position，然后向后
	std::copy(first, last, position);
	}
	else {
	iterator temp = WKstl::uninitialized_copy(first + (finish_ - position), last, finish_);
	WKstl::uninitialized_copy(position, finish_, temp);
	std::copy(first, first + (finish_ - position), position);
	}
	finish_ += locationNeed;
	}
	else {
	reallocateAndCopy(position,first,last);
	}
	}
	*/
	template<class T, class Alloc>
	template<class InputIterator>
	void vector<T, Alloc>::insert_aux(iterator position,
		InputIterator first,
		InputIterator last,
		std::false_type) {
		difference_type locationLeft = endOfStorage_ - finish_; // the size of left storage
		difference_type locationNeed = distance(first, last);//last - first;

		if (locationLeft >= locationNeed) {
			if (finish_ - position > locationNeed) {
				WKstl::uninitialized_copy(finish_ - locationNeed, finish_, finish_);
				std::copy_backward(position, finish_ - locationNeed, finish_);
				std::copy(first, last, position);
			}
			else {
				iterator temp = WKstl::uninitialized_copy(first + (finish_ - position), last, finish_);
				WKstl::uninitialized_copy(position, finish_, temp);
				std::copy(first, first + (finish_ - position), position);
			}
			finish_ += locationNeed;
		}
		else {
			reallocateAndCopy(position, first, last);
		}
	}
	template<class T, class Alloc>
	template<class Integer>
	void vector<T, Alloc>::insert_aux(iterator position, Integer n, const value_type& value, std::true_type) {
		assert(n != 0);
		difference_type locationLeft = endOfStorage_ - finish_; // the size of left storage
		difference_type locationNeed = n;

		if (locationLeft >= locationNeed) {
			auto tempPtr = end() - 1;
			for (; tempPtr - position >= 0; --tempPtr) {//move the [position, finish_) back
														//*(tempPtr + locationNeed) = *tempPtr;//bug
				construct(tempPtr + locationNeed, *tempPtr);
			}
			WKstl::uninitialized_fill_n(position, n, value);
			finish_ += locationNeed;
		}
		else {
			reallocateAndFillN(position, n, value);
		}
	}
	//向vector插入元素的对外接口，对内还待以后补充
	template<class T, class Alloc>
	void vector<T, Alloc>::insert(iterator position, const size_type& n, const value_type& val) {
		insert_aux(position, n, val, typename std::is_integral<InputIterator>::type());
	}


	template<class T, class Alloc>
	template<class InputIterator>
	void vector<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last) {
		insert_aux(position, first, last, typename std::is_integral<InputIterator>::type());
	}

	template<class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, const value_type& val) {
		const auto index = position - begin();
		insert(position, 1, val);
		return begin() + index;
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::push_back(const value_type& value) {
		insert(end(), value);
	}
	/******************逻辑比较相关的操作***********************/
	template<class T, class Alloc>
	bool vector<T, Alloc>::operator==(const vector& v)const {
		if (size() != v.size())
			return false;
		else {
			auto ptr1 = start_;
			auto ptr2 = v.start_;
			for (; ptr1 != finish_&&ptr2 != v.finish_; ++ptr1, ++ptr2) {
				if (*ptr1 != *ptr2)
					return false;
			}
			return true;
		}
	}

	template<class T, class Alloc>
	bool vector<T, Alloc>::operator!=(const vector& v)const {
		return!(*this == v);
	}

	template<class T, class Alloc>
	bool operator==(const vector<T, Alloc>& v1, const vector<T, Alloc>& v2) {
		return v1.operator==(v2);
	}

	template<class T, class Alloc>
	bool operator!=(const vector<T, Alloc>& v1, const vector<T, Alloc>& v2) {
		return !(v1 == v2);
	}

	//请求容器降低其容量和size匹配
	template<class T, class Alloc>
	void vector<T, Alloc>::shrink_to_fit() {
		T *t = (T*)dataAllocator::allocate(size());
		finish_ = WKstl::uninitialized_copy(start_, finish_, t);
		start_ = t;
		endOfStorage_ = finish_;
	}

	//请求清空容器中的元素
	template <class T, class Alloc>
	void vector<T, Alloc>::clear() {
		dataAllocator::destroy(start_, finish_);
		finish_ = start_;
	}

	//交换两个vector中的元素
	template<class T, class Alloc>
	void vector<T, Alloc>::swap(vector& v) {
		if (this != &v) {
			WKstl::swap(start_, v.start_);//调用的是Utility.h中的函数swap()
			WKstl::swap(finish_, v.finish_);
			WKstl::swap(endOfStorage_, v.endOfStorage_);
		}
	}

	//弹出尾元素
	template<class T, class Alloc>
	void vector<T, Alloc>::pop_back() {
		--finish_;
		dataAllocator::destroy(finish_);
	}

	//析构容器中的元素并释放容器空间
	template<class T, class Alloc>
	void vector<T, Alloc>::destroyAndDeallocateAll() {
		if (capacity() != 0) {
			dataAllocator::destroy(start_, finish_);
			dataAllocator::deallocate(start_, capacity());
		}
	}

	//申请一块空间存放n个类型为T值为value_type的元素
	template<class T, class Alloc>
	void vector<T, Alloc>::allocateAndFillN(const size_type n, const value_type& value) {
		start_ = dataAllocator::allocate(n);
		finish_ = WKstl::uninitialized_fill_n(start_, n, value);
		endOfStorage_ = finish_;
	}

	//将迭代器范围内的元素拷贝到新的地址空间中
	template<class T, class Alloc>
	template<class InputIterator>
	void vector<T, Alloc>::allocateAndCopy(InputIterator first, InputIterator last) {
		start_ = dataAllocator::allocate(last - first);
		finish_ = WKstl::uninitialized_copy(first, last, start_);
		endOfStorage_ = finish_;
	}

	template<class T, class Alloc>
	template<class InputIterator>
	void vector<T, Alloc>::vector_aux(InputIterator first, InputIterator last, std::false_type) {
		allocateAndCopy(first, last);
	}

	template<class T, class Alloc>
	template<class Integer>
	void vector<T, Alloc>::vector_aux(Integer n, const value_type& value, std::true_type) {
		allocateAndFillN(n, value);
	}

	//不是分配空间或者构造对象，只是确定空间的大小
	template<class T, class Alloc>
	typename vector<T, Alloc>::size_type vector<T, Alloc>::getNewCapacity(size_type len)const {
		size_type oldCapacity = endOfStorage_ - start_;
		size_type res;
		if (oldCapacity > len)
			res = oldCapacity;
		else
			res = len;
		size_type newCapacity = (oldCapacity != 0 ? (oldCapacity + res) : len);
		return newCapacity;
	}
}
/*
1.copy和uninitialized_copy的区别
uninitialized_copy 会在目标地址构造对象。当要把对象复制到一段未初始化的内存地址的时候用
而copy是在已经初始化的内存空间进行赋值
*/
#endif
