#ifndef CIRCBUFF_H
#define CIRCBUFF_H

#include <iostream>
#include <memory>

template<typename T>
class CircBuff
{
private:
	T* buf_;
	size_t size_, capacity_, head_, tail_;

	void inc_head();
	void inc_tail();

public:
	using value_type = T;
	using reference = value_type&;
	using const_reference = const value_type&;
	using size_type = size_t;

	CircBuff(const size_t& cap);
	~CircBuff();

	// For testing
	friend std::ostream& operator<<(std::ostream& os, const CircBuff& buff)
	{
		size_t i = buff.head_, n = 0;
		while (n < buff.size())
		{
			os << buff.buf_[i++] << ", ";
			if (i == buff.capacity())
				i = 0;
			++n;
		}
		return os;
	}

	void push(const T& element);
	void pop();
	void clear();
	reference front();
	const_reference front() const;
	bool empty() const;
	bool full() const;
	size_type size() const;
	size_type capacity() const;
};

#endif // !CIRCBUFF_H

template<typename T>
inline void CircBuff<T>::inc_head()
{
	if (++head_ == capacity_)
		head_ = 0;
}

template<typename T>
inline void CircBuff<T>::inc_tail()
{
	if (++tail_ == capacity_)
		tail_ = 0;
}

template<typename T>
inline CircBuff<T>::CircBuff(const size_t& cap) :
	buf_(new T[cap]), size_(0), capacity_(cap),
	head_(0), tail_(0) {}

template<typename T>
inline CircBuff<T>::~CircBuff()
{
	delete[] buf_;
}

template<typename T>
inline void CircBuff<T>::push(const T& item)
{
	buf_[tail_] = item;
	inc_tail();
	if (full())
		inc_head();
	else
		++size_;
}

template<typename T>
inline void CircBuff<T>::pop()
{
	inc_head();
	--size_;
}

template<typename T>
inline void CircBuff<T>::clear()
{
	head_ = tail_ = size_ = 0;
}

template<typename T>
inline typename CircBuff<T>::reference CircBuff<T>::front()
{
	return buf_[head_];
}

template<typename T>
inline typename CircBuff<T>::const_reference CircBuff<T>::front() const
{
	return buf_[head_];
}

template<typename T>
inline bool CircBuff<T>::empty() const
{
	return size_ == 0;
}

template<typename T>
inline bool CircBuff<T>::full() const
{
	return size_ == capacity_;
}

template<typename T>
inline typename CircBuff<T>::size_type CircBuff<T>::size() const
{
	return size_;
}

template<typename T>
inline typename CircBuff<T>::size_type CircBuff<T>::capacity() const
{
	return capacity_;
}
