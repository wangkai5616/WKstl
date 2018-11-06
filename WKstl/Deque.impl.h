#ifndef _DEQUE_IMPL_H_
#define _DEQUE_IMPL_H_
namespace WKstl {
	namespace Detail {
		template<class T>
		dq_iter<T>& dq_iter<T>::operator++() {
			if (cur_ != getBuckTail(mapIndex_))
				++cur_;
			else if (mapIndex_ + 1 < container_->mapSize_) {
				++mapIndex_;
				cur_ = getBuckHead(mapIndex_);
			}
			else {
				mapIndex_ = container_->mapSize_;
				cur_ = container_->map_[mapIndex_];
			}
			return *this;
		}
		template<class T>
		dq_iter<T> dq_iter<T>::operator++(int) {
			auto res = *this;
			++(*this);
			return res;
		}
		template<class T>
		dq_iter<T>& dq_iter<T>::operator--() {
			if (cur_ != getBuckHead(mapIndex_))
				--cur_;
			else if (mapIndex_ - 1 >= 0) {
				--mapIndex_;
				cur_ = getBuckTail(mapIndex_);
			}
			else {
				mapIndex_ = 0;
				cur_ = container_->map_[mapIndex_];
			}
			return *this;
		}
		template<class T>
		dq_iter<T> dq_iter<T>::operator--(int) {
			auto res = *this;
			--(*this);
			return this;
		}
		template<class T>
		bool dq_iter<T>::operator==(const dq_iter& it)const {
			return((mapIndex_ == it.mapIndex_) && (cur_ == it.cur_) && (container_ == it.container_));
		}
		template<class T>
		bool dq_iter<T>::operator!=(const dq_iter& it)const {
			return !(*this == it);
		}
		template<class T>
		dq_iter<T>& dq_iter<T>::operator=(const dq_iter& it) {
			if (this != &it) {
				mapIndex_ = it.mapIndex_;
				cur_ = it.cur_;
				container_ = it.container_;
			}
			return *this;
		}
		//两个迭代器指向同一双端数组
		template<class T>
		void dq_iter<T>::swap(dq_iter& it) {
			WKstl::swap(mapIndex_, it.mapIndex_);
			WKstl::swap(cur_, it.cur_);
		}
		template<class T>
		dq_iter<T> operator+(const dq_iter<T>& it, typename dq_iter<T>::difference_type n) {
			dq_iter<T> res(it);
			auto m = res.getBuckTail(res.mapIndex_) - res.cur_;
			if (n <= m) {
				res.cur_ += n;
			}
			else {
				n = n - m;
				res.mapIndex_ += (n / it.getBuckSize() + 1);
				res.cur_ = res.getBuckHead(res.mapIndex_) + (n%it.getBuckSize() - 1);
			}
			return res;
		}
		//调整上面的形式 
		template<class T>
		dq_iter<T>operator+(typename dq_iter<T>::difference_type n, const dq_iter<T>& it) {
			return (it + n);
		}
		template<class T>
		dq_iter<T> operator-(const dq_iter<T>& it, typename dq_iter<T>::difference_type n) {
			dq_iter<T> res(it);
			auto m = res.cur_ - res.getBuckHead(res.mapIndex_);
			if (n <= m)
				res.cur_ -= n;
			else {
				n = n - m;
				res.mapIndex_ -= (n / res.getBuckSize() + 1);
				res.cur_ = res.getBuckTail(res.mapIndex_) - (n%res.getBuckSize() - 1);
			}
			return res;
		}
		template<class T>
		dq_iter<T> operator-(typename dq_iter<T>::difference_type n, const dq_iter<T>& it) {
			return (it - n);
		}
		template<class T>
		typename dq_iter<T>::difference_type operator-(const dq_iter<T>& it1, const dq_iter<T>& it2) {
			if (it1.container_ == it2.container_ && it1.container_ == 0)
				return 0;
			return typename dq_iter<T>::difference_type(it1.getBuckSize())*(it1.mapIndex_ - it2.mapIndex_ - 1)
				+ (it1.cur_ - it1.getBuckHead(it1.mapIndex_)) + (it2.getBuckTail(it2.mapIndex_) - it2.cur_) + 1;
		}
		template<class T>
		void swap(dq_iter<T>&lhs, dq_iter<T>& rhs) {
			lhs.swap(rhs);
		}
		template<class T>
		T* dq_iter<T>::getBuckTail(size_t mapIndex)const {
			return container_->map_[mapIndex] + (container_->getBuckSize() - 1);
		}
		//container_->map_[mapIndex],为什么要用指针指一下，因为你在迭代器中用deque中的元素
		//如果是在deque中，直接用map[mapIndex]就行
		template<class T>
		T* dq_iter<T>::getBuckHead(size_t mapIndex)const {
			return container_->map_[mapIndex];
		}
		template<class T>
		size_t dq_iter<T>::getBuckSize()const {
			return container_->getBuckSize();
		}
	}

	template<class T, class Alloc>
	bool deque<T, Alloc>::back_full()const {
		//end()返回的迭代器指向最后一块缓冲区
		return map_[mapSize_ - 1] && (map_[mapSize_]) == end().cur_;//map_[mapSize_]其实还不知道在哪
	}
	template<class T, class Alloc>
	bool deque<T, Alloc>::front_full()const {
		return map_[0] && map_[0] == begin().cur_;//begin()返回的迭代器指向第一块缓冲区
	}
	template<class T, class Alloc>
	void deque<T, Alloc>::deque_aux(size_t n, const value_type& val, std::true_type) {
		int i = 0;
		for (; i != n / 2; ++i)
			(*this).push_front(val);//*this是指整个双端数组
		for (; i != n; ++i)
			(*this).push_back(val);
	}
	//void deque_aux(Iterator first, Iterator last, std::false_type)
	template<class T, class Alloc>
	template<class Iterator>
	void deque<T, Alloc>::deque_aux(Iterator first, Iterator last, std::false_type) {
		difference_type mid = (last - first) / 2;
		for (auto it = first + mid; it!=first - 1; --it)
			(*this).push_front(*it);
		for (auto it = first + mid + 1; it != last; ++it)
			(*this).push_back(*it);
	}
	template<class T, class Alloc>
	void deque<T, Alloc>::init() {
		mapSize_ = 2;
		map_ = getANewMap(mapSize_);
		beg_.container_ = end_.container_ = this;
		beg_.mapIndex_ = end_.mapIndex_ = mapSize_ - 1;
		beg_.cur_ = end_.cur_ = map_[mapSize_ - 1];
	}
	//分配一块缓冲空间
	template<class T, class Alloc>
	T* deque<T, Alloc>::getANewBuck() {
		return dataAllocator::allocate(getBuckSize());
	}
	//分配新的map,并建立与缓冲空间的对应
	template<class T, class Alloc>
	T** deque<T, Alloc>::getANewMap(const size_t size) {
		T** map = new T*[size];//新的map
		for (int i = 0; i != size; ++i)
			map[i] = getANewBuck();//将map[i]对应到缓冲空间
		return map;
	}
	//确定新map的大小
	template<class T, class Alloc>
	size_t deque<T, Alloc>::getNewMapSize(const size_t size) {
		return (size == 0 ? 2 : size * 2);
	}
	//确定缓冲区的大小size_t getBuckSize(const size_t size)
	template<class T, class Alloc>
	size_t deque<T, Alloc>::getBuckSize()const {
		return (size_t)EBucksSize::BUCKSIZE;
	}
	template<class T, class Alloc>
	void deque<T, Alloc>::clear() {
		for (auto i = 0; i != mapSize_; ++i) {
			for (auto p = map_[i] + 0; !p&&p != map_[i] + getBuckSize(); ++p)
				dataAllocator::destroy(p);
		}
		mapSize_ = 0;
		beg_.mapIndex_ = end_.mapIndex_ = mapSize_ / 2;
		beg_.cur_ = end_.cur_ = map_[mapSize_ / 2];
	}
	template<class T, class Alloc>
	typename deque<T, Alloc>::reference deque<T, Alloc>::operator[](size_type n) {
		return *(begin() + n);
	}
	template<class T, class Alloc>
	typename deque<T, Alloc>::reference deque<T, Alloc>::front() {
		return *begin();
	}
	template<class T, class Alloc>
	typename deque<T, Alloc>::reference deque<T, Alloc>::back() {
		return *(end() - 1);
	}
	template<class T, class Alloc>
	typename deque<T, Alloc>::iterator deque<T, Alloc>::begin() { return beg_; }
	template<class T, class Alloc>
	typename deque<T, Alloc>::iterator deque<T, Alloc>::end() { return end_; }
	//由于const迭代器的设计失误故以下三个const函数会丧失const特性
	template<class T, class Alloc>
	typename deque<T, Alloc>::const_reference deque<T, Alloc>::operator[] (size_type n) const {
		return *(begin() + n);
	}
	template<class T, class Alloc>
	typename deque<T, Alloc>::const_reference deque<T, Alloc>::front() const {
		return *begin();
	}
	template<class T, class Alloc>
	typename deque<T, Alloc>::const_reference deque<T, Alloc>::back() const {
		return *(end() - 1);
	}
	//迭代器设计失误，会使下面两个函数丧失const特性，暂时没有效解决办法故只能先这样妥协
	template<class T, class Alloc>
	typename deque<T, Alloc>::iterator deque<T, Alloc>::begin()const { return beg_; }
	template<class T, class Alloc>
	typename deque<T, Alloc>::iterator deque<T, Alloc>::end()const { return end_; }

	template<class T, class Alloc>
	deque<T, Alloc>::~deque() {
		for (int i = 0; i != mapSize_; ++i) {
			for (auto p = map_[i] + 0; !p&&p != map_[i] + getBuckSize(); ++p)
				dataAllocator::destroy(p);
			//以一块缓冲空间进行deallocate,因为这是连续的，而与下一块缓冲空间不一定连续
			if (!map_[i])
				//static void deallocate(void *ptr, size_t bytes)
				dataAllocator::deallocate(map_[i], getBuckSize());
		}
		delete[] map_;
	}
	template<class T, class Alloc>
	deque<T,Alloc>& deque<T, Alloc>::operator=(const deque& x) { 
		clear();
		for (; x.begin() != x.end(); ++x.begin()) {
			(*this).push_back(*(x.begin()));
		}
		return *this;
	}
	template<class T, class Alloc>
	deque<T, Alloc>::deque()
		:mapSize_(0), map_(0) {}
	template<class T, class Alloc>
	deque<T, Alloc>::deque(size_type n, const value_type& val = value_type()) {
		deque();
		deque_aux(n, val, typename std::is_integral<size_type>::type());
	}
	//template<class InputIterator>
	//deque(InputIterator first, InputIterator last);
	template<class T, class Alloc>
	template<class InputIterator>
	deque<T, Alloc>::deque(InputIterator first, InputIterator last) {
		deque();
		deque_aux(first, last, typename std::is_integral<InputIterator>::type());
	}

	template<class T,class Alloc>
	deque<T, Alloc>::deque(const deque& x) {
		mapSize_ = x.mapSize_;
		map_ = getANewMap(mapSize_);
		for (int i = 0; i + x.beg_.mapIndex_ != x.mapSize_; ++i)
			for (int j = 0; j != getBuckSize(); ++j)
				map_[x.beg_.mapIndex_ + i][j] = x.map_[x.beg_.mapIndex_ + i][j];
		beg_.mapIndex_ = x.beg_.mapIndex_;
		end_.mapIndex_ = x.end_.mapIndex_;
		beg_.cur_ = map_[beg_.mapIndex_] + (x.beg_.cur_ - x.map_[x.beg_.mapIndex_]);
		end_.cur_ = map_[end_.mapIndex_] + (x.end_.cur_ - x.map_[x.end_.mapIndex_]);
		beg_.container_ = end_.container_ = this;
	}
	template<class T, class Alloc>
	void deque<T, Alloc>::reallocateAndCopy() {
		auto newMapSize = getNewMapSize(mapSize_);
		T** newMap = getANewMap(newMapSize);
		size_t startIndex = newMapSize / 4;//从四分之一开始，可以更好的再次加入新元素(因为前后留有空间一样多)
		for (int i = 0; i + beg_.mapIndex_ != mapSize_; ++i)
			for (int j = 0; j != getBuckSize(); ++j)
				newMap[startIndex + i][j] = map_[beg_.mapIndex_ + i][j];

		//记录首元素在旧缓冲空间的编号，新的地址空间也使用同样的编号
		size_t n = beg_.cur_ - map_[beg_.mapIndex_];
		auto size = this->size();
		clear();//可是没有释放空间啊
		mapSize_ = newMapSize;
		map_ = newMap;
		beg_ = iterator(startIndex, newMap[startIndex] + n, this);
		end_ = beg_ + size;
	}
	template<class T, class Alloc>
	void deque<T, Alloc>::push_front(const value_type& val) {
		if (empty()) {
			init();
		}
		else if(front_full()) {
			reallocateAndCopy();
		}
		--beg_;
		WKstl::construct(beg_.cur_, val);
	}
	template<class T, class Alloc>
	void deque<T, Alloc>::push_back(const value_type& val) {
		if (empty()) {
			init();
		}
		else if(back_full()) {
			reallocateAndCopy();
		}
		//迭代器原则，左开右闭
		WKstl::construct(end_.cur_, val);
		++end_;
	}
	template<class T,class Alloc>
	void deque<T, Alloc>::pop_front(){
		dataAllocator::destroy(beg_.cur_);
		++beg_;
	}
	template<class T,class Alloc>
	void deque<T, Alloc>::pop_back(){
		--end_;
		dataAllocator::destroy(end_.cur_);
	}
	template<class T,class Alloc>
	void deque<T, Alloc>::swap(deque<T, Alloc>& x) {
		WKstl::swap(mapSize_, x.mapSize_);
		WKstl::swap(map_, x.map_);
		beg_.swap(x.beg_);
		end_.swap(x.end_);
	}
	template <class T, class Alloc>
	bool operator== (const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs) {
		auto cit1 = lhs.begin(), cit2 = rhs.begin();
		for (; cit1 != lhs.end() && cit2 != rhs.end(); ++cit1, ++cit2) {
			if (*cit1 != *cit2)
				return false;
		}
		if (cit1 == lhs.end() && cit2 == rhs.end())
			return true;
		return false;
	}
	//friend bool operator!=(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs)
	template <class T, class Alloc>
	bool operator!= (const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs) {
		return !(lhs == rhs);
	}
	//friend void swap(deque<T, Alloc>& x, deque<T, Alloc>& y);
	template <class T, class Alloc>
	void swap(deque<T, Alloc>& x, deque<T, Alloc>& y) {
		x.swap(y);
	}

}
#endif