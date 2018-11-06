#ifndef _VECTOR_IMPL_H_
#define _VECTOR_IMPL_H_

namespace WKstl {
	/*****************���죬���ƣ�������صĲ���******************************/
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
		//����ָ������ּ������ĺ���
		vector_aux(first, last, typename std::is_integral<InputIterator>::type());
	}
	template<class T, class Alloc>
	vector<T, Alloc>::vector(const vector& v) {
		allocateAndCopy(v.start_, v.finish_);
	}
	//��ֵ����
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
	//��һ�������Ҫ��ֵ��v�е�Ԫ�ظ���������������������
	if (v.size() > capacity()) {
	destroyAndDeallocateAll();
	allocateAndCopy(v.start_,v.finish_);

	//destroyAndDeallocateAll();//�Լ�����������ĳ�����һ��
	//start_ = tmp;
	//endOfStorage_ = start_ + (v.end() - v.begin());
	}
	//�ڶ��������Ҫ��ֵ��v�е�Ԫ�ظ���С������������Ԫ�ظ���
	else if (size() >= v.size()) {
	iterator i = std::copy(v.begin(), v.end(), begin());
	dataAllocator::destroy(i, finish_);
	finish_ = start_ + v.size();
	}
	//�����������Ҫ��ֵ��v�е�Ԫ�ظ���С����������������������������������Ԫ�ظ���
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
	/******************������صĲ���****************************/
	//resize() ����ָ��������Ч��Ԫ�ظ���
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

	/**************�޸���������ز���*****************************/
	//erase��������ɾ����ĳ��λ��position��һ������first, last)�е�Ԫ�أ�������size
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
	//���·���һ���ַ������vector��ԭ����Ԫ�غ�Ҫ����ĵ�����֮���Ԫ��
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

	//���·���һ���ַ������vector��ԭ����Ԫ�غ�Ҫ�����n��ֵΪvalԪ��
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
	//����ж�����ԭ�����copy��uninitialized_copy������
	if (finish_ - position > locationNeed) {
	WKstl::uninitialized_copy(finish_ - locationNeed, finish_, finish_);
	//���ƹ����Ǵ�����Ԫ�ؿ�ʼ���ƣ�ֱ����Ԫ�ظ��Ƴ�����Ҳ����˵�����Ʋ����Ǵ�last-1��ʼ��ֱ��first����
	//�ȸ��Ƶ��ĵ�ַ��finish_��Ȼ����ǰ
	std::copy_backward(position, finish_ - locationNeed, finish_);
	//���ƹ����Ǵ�firstԪ�ؿ�ʼ���ƣ�ֱ��last-1Ԫ�ظ��Ƴ������ȸ��Ƶ��ĵ�ַ��position��Ȼ�����
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
	//��vector����Ԫ�صĶ���ӿڣ����ڻ����Ժ󲹳�
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
	/******************�߼��Ƚ���صĲ���***********************/
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

	//��������������������sizeƥ��
	template<class T, class Alloc>
	void vector<T, Alloc>::shrink_to_fit() {
		T *t = (T*)dataAllocator::allocate(size());
		finish_ = WKstl::uninitialized_copy(start_, finish_, t);
		start_ = t;
		endOfStorage_ = finish_;
	}

	//������������е�Ԫ��
	template <class T, class Alloc>
	void vector<T, Alloc>::clear() {
		dataAllocator::destroy(start_, finish_);
		finish_ = start_;
	}

	//��������vector�е�Ԫ��
	template<class T, class Alloc>
	void vector<T, Alloc>::swap(vector& v) {
		if (this != &v) {
			WKstl::swap(start_, v.start_);//���õ���Utility.h�еĺ���swap()
			WKstl::swap(finish_, v.finish_);
			WKstl::swap(endOfStorage_, v.endOfStorage_);
		}
	}

	//����βԪ��
	template<class T, class Alloc>
	void vector<T, Alloc>::pop_back() {
		--finish_;
		dataAllocator::destroy(finish_);
	}

	//���������е�Ԫ�ز��ͷ������ռ�
	template<class T, class Alloc>
	void vector<T, Alloc>::destroyAndDeallocateAll() {
		if (capacity() != 0) {
			dataAllocator::destroy(start_, finish_);
			dataAllocator::deallocate(start_, capacity());
		}
	}

	//����һ��ռ���n������ΪTֵΪvalue_type��Ԫ��
	template<class T, class Alloc>
	void vector<T, Alloc>::allocateAndFillN(const size_type n, const value_type& value) {
		start_ = dataAllocator::allocate(n);
		finish_ = WKstl::uninitialized_fill_n(start_, n, value);
		endOfStorage_ = finish_;
	}

	//����������Χ�ڵ�Ԫ�ؿ������µĵ�ַ�ռ���
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

	//���Ƿ���ռ���߹������ֻ��ȷ���ռ�Ĵ�С
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
1.copy��uninitialized_copy������
uninitialized_copy ����Ŀ���ַ������󡣵�Ҫ�Ѷ����Ƶ�һ��δ��ʼ�����ڴ��ַ��ʱ����
��copy�����Ѿ���ʼ�����ڴ�ռ���и�ֵ
*/
#endif
