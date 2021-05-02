#ifndef HISTORYQUEUE_H
#define HISTORYQUEUE_H

#include <iterator>

template<typename T>
class HistoryQueue
{
private:
    class Node; // Forwad declaration of the node

public:
	using value_type = T;
	using reference = value_type&;
	using const_reference = const value_type&;
	using size_type = size_t;

	//////// Iterators
	// Bidirectional iterator
	class iterator
	{
	private:
		Node* ptr_, *end_;

	public:
		using value_type = HistoryQueue::value_type;
		using reference = HistoryQueue::reference;
		using const_reference = HistoryQueue::reference;
		using pointer = HistoryQueue::value_type*;
		using iterator_category = std::bidirectional_iterator_tag;

		iterator(Node* item, Node* end);
		iterator(const iterator& it);
		iterator(const iterator&& it);

		reference operator*();
		const_reference operator*() const;
		pointer operator->();
		iterator operator++(int);
		iterator& operator++();
		iterator operator--(int);
		iterator& operator--();

		iterator& operator=(const iterator& it);
		iterator& operator=(const iterator&& it);

		bool operator==(const iterator& it) const;
		bool operator!=(const iterator& it) const;
	};

	// Constant bidirectional iterator
	class const_iterator
	{
	private:
        const Node* ptr_, *end_;

	public:
		using value_type = HistoryQueue::value_type;
		using const_reference = HistoryQueue::const_reference;
		using pointer = const HistoryQueue::value_type*;
		using iterator_category = std::bidirectional_iterator_tag;

		const_iterator(const Node* item, const Node* end);
		const_iterator(const const_iterator& it);
		const_iterator(const const_iterator&& it);

		const_reference operator*() const;
		pointer operator->();
		const_iterator operator++(int);
		const_iterator& operator++();
		const_iterator operator--(int);
		const_iterator& operator--();

		const_iterator& operator=(const const_iterator& it);
		const_iterator& operator=(const const_iterator&& it);

		bool operator==(const const_iterator& it) const;
		bool operator!=(const const_iterator& it) const;
	};

	//////// Methods
	HistoryQueue();
	~HistoryQueue();

	void push_front(const T& item);
	void push_back(const T& item);
	reference front();
	const_reference front() const;
	reference back();
	const_reference back() const;
	void pop_front();
	void pop_back();
	size_type size() const;
	bool empty() const;

	// Iterator methods
	iterator begin();
	iterator end();
	const_iterator cbegin() const;
	const_iterator cend() const;
	const_iterator begin() const;
	const_iterator end() const;

private:
	class Node
	{
	public:
		T data_;
		Node* next_, *prev_;

		Node(const value_type& item, Node* nextPtr = nullptr, Node* prevPtr = nullptr);
	};

	Node* front_, * back_;
	size_t size_;
};

#endif // !HISTORYQUEUE_H

template<typename T>
inline HistoryQueue<T>::HistoryQueue() : front_(nullptr), back_(nullptr), size_(0) {}

template<typename T>
inline HistoryQueue<T>::~HistoryQueue()
{
	while (front_)
		pop_front();
}

template<typename T>
inline void HistoryQueue<T>::push_front(const T& item)
{
	if (!front_)
	{
		front_ = new Node(item);
		back_ = front_;
	}
	else
	{
		Node* tmp = new Node(item, front_);
		front_->prev_ = tmp;
		front_ = tmp;
	}
	
	++size_;
}

template<typename T>
inline void HistoryQueue<T>::push_back(const T& item)
{
    if (!back_)
	{
		back_ = new Node(item);
		front_ = back_;
	}
	else
	{
		Node* tmp = new Node(item, nullptr, back_);
		back_->next_ = tmp;
		back_ = tmp;
	}

	++size_;
}

template<typename T>
inline typename HistoryQueue<T>::reference HistoryQueue<T>::front()
{
	return front_->data_;
}

template<typename T>
inline typename HistoryQueue<T>::const_reference HistoryQueue<T>::front() const
{
	return front_->data_;
}

template<typename T>
inline typename HistoryQueue<T>::reference HistoryQueue<T>::back()
{
	return *back_;
}

template<typename T>
inline typename HistoryQueue<T>::const_reference HistoryQueue<T>::back() const
{
	return back_;
}

template<typename T>
inline void HistoryQueue<T>::pop_front()
{
	Node* tmp = front_->next_;
	delete front_;
	
	if (tmp)
	{
		tmp->prev_ = nullptr;
		front_ = tmp;
	}
	else
		front_ = back_ = nullptr;
	
	--size_;
}

template<typename T>
inline void HistoryQueue<T>::pop_back()
{
    Node* tmp = back_->prev_;
	delete back_;

	if (tmp)
	{
		tmp->next_ = nullptr;
		back_ = tmp;
	}
	else
		front_ = back_ = nullptr;

	--size_;
}

template<typename T>
inline typename HistoryQueue<T>::size_type HistoryQueue<T>::size() const
{
	return size_;
}

template<typename T>
inline bool HistoryQueue<T>::empty() const
{
	return size_ == 0;
}

template<typename T>
inline typename HistoryQueue<T>::iterator HistoryQueue<T>::begin()
{
	return iterator(front_, back_);
}

template<typename T>
inline typename HistoryQueue<T>::iterator HistoryQueue<T>::end()
{
	return iterator(nullptr, back_);
}

template<typename T>
inline typename HistoryQueue<T>::const_iterator HistoryQueue<T>::begin() const
{
	return const_iterator(front_, back_);
}

template<typename T>
inline typename HistoryQueue<T>::const_iterator HistoryQueue<T>::end() const
{
	return const_iterator(nullptr, back_);
}

template<typename T>
inline typename HistoryQueue<T>::const_iterator HistoryQueue<T>::cbegin() const
{
	return const_iterator(front_, back_);
}

template<typename T>
inline typename HistoryQueue<T>::const_iterator HistoryQueue<T>::cend() const
{
	return const_iterator(nullptr, back_);
}

template<typename T>
inline HistoryQueue<T>::Node::Node(const value_type& item, Node* nextPtr, Node* prevPtr) :
	data_(item), next_(nextPtr), prev_(prevPtr) {}

/////////////////////////////////////////////////
// Iterator
/////////////////////////////////////////////////

template<typename T>
inline HistoryQueue<T>::iterator::iterator(Node* item, Node* end) : ptr_(item), end_(end) {}

template<typename T>
inline HistoryQueue<T>::iterator::iterator(const iterator& it) : ptr_(it.ptr_), end_(it.end_) {}

template<typename T>
inline HistoryQueue<T>::iterator::iterator(const iterator&& it) : ptr_(it.ptr_), end_(it.end_) {}

template<typename T>
inline typename HistoryQueue<T>::iterator::reference HistoryQueue<T>::iterator::operator*()
{
	return ptr_->data_;
}

template<typename T>
inline typename HistoryQueue<T>::iterator::const_reference HistoryQueue<T>::iterator::operator*() const
{
	return ptr_->data_;
}


template<typename T>
inline typename HistoryQueue<T>::iterator::pointer HistoryQueue<T>::iterator::operator->()
{
	return &(ptr_->data_);
}

template<typename T>
inline typename HistoryQueue<T>::iterator HistoryQueue<T>::iterator::operator++(int)
{
	iterator tmp = *this;
	ptr_ = ptr_->next_;
	return tmp;
}

template<typename T>
inline typename HistoryQueue<T>::iterator& HistoryQueue<T>::iterator::operator++()
{
	ptr_ = ptr_->next_;
	return *this;
}

template<typename T>
inline typename HistoryQueue<T>::iterator& HistoryQueue<T>::iterator::operator=(const iterator& it)
{
	ptr_ = it.ptr_;
	end_ = it.end_;
	return *this;
}

template<typename T>
inline typename HistoryQueue<T>::iterator& HistoryQueue<T>::iterator::operator=(const iterator&& it)
{
	ptr_ = it.ptr_;
	end_ = it.end_;
	return *this;
}

template<typename T>
inline typename HistoryQueue<T>::iterator HistoryQueue<T>::iterator::operator--(int)
{	
	iterator tmp = *this;
	if (!ptr_)
		ptr_ = end_;
	else
		ptr_ = ptr_->prev_;
	return tmp;
}

template<typename T>
inline typename HistoryQueue<T>::iterator& HistoryQueue<T>::iterator::operator--()
{	
	if (!ptr_)
		ptr_ = end_;
	else
		ptr_ = ptr_->prev_;
	return *this;
}

template<typename T>
inline bool HistoryQueue<T>::iterator::operator==(const iterator& it) const
{
	return ptr_ == it.ptr_;
}

template<typename T>
inline bool HistoryQueue<T>::iterator::operator!=(const iterator& it) const
{
	return ptr_ != it.ptr_;
}

/////////////////////////////////////////////////
// Constant iterator
/////////////////////////////////////////////////

template<typename T>
inline HistoryQueue<T>::const_iterator::const_iterator(const Node* item, const Node* end) : ptr_(item), end_(end) {}

template<typename T>
inline HistoryQueue<T>::const_iterator::const_iterator(const const_iterator& it) : ptr_(it.ptr_), end_(it.end_) {}

template<typename T>
inline HistoryQueue<T>::const_iterator::const_iterator(const const_iterator&& it) : ptr_(it.ptr_), end_(it.end_) {}

template<typename T>
inline typename HistoryQueue<T>::const_iterator::const_reference HistoryQueue<T>::const_iterator::operator*() const
{
	return ptr_->data_;
}

template<typename T>
inline typename HistoryQueue<T>::const_iterator::pointer HistoryQueue<T>::const_iterator::operator->()
{
	return &(ptr_->data_);
}

template<typename T>
inline typename HistoryQueue<T>::const_iterator HistoryQueue<T>::const_iterator::operator++(int)
{
	const_iterator tmp = *this;
	ptr_ = ptr_->next_;
	return tmp;
}

template<typename T>
inline typename HistoryQueue<T>::const_iterator& HistoryQueue<T>::const_iterator::operator++()
{
	ptr_ = ptr_->next_;
	return *this;
}

template<typename T>
inline typename HistoryQueue<T>::const_iterator HistoryQueue<T>::const_iterator::operator--(int)
{	
	const_iterator tmp = *this;
	if (!ptr_)
		ptr_ = end_;
	else
		ptr_ = ptr_->prev_;
	return tmp;
}

template<typename T>
inline typename HistoryQueue<T>::const_iterator& HistoryQueue<T>::const_iterator::operator--()
{	
	if (!ptr_)
		ptr_ = end_;
	else
		ptr_ = ptr_->prev_;
	return *this;
}

template<typename T>
inline typename HistoryQueue<T>::const_iterator& HistoryQueue<T>::const_iterator::operator=(const const_iterator& it)
{
	ptr_ = it.ptr_;
	end_ = it.end_;
	return *this;
}

template<typename T>
inline typename HistoryQueue<T>::const_iterator& HistoryQueue<T>::const_iterator::operator=(const const_iterator&& it)
{
	ptr_ = it.ptr_;
	end_ = it.end_;
	return *this;
}

template<typename T>
inline bool HistoryQueue<T>::const_iterator::operator==(const const_iterator& it) const
{
	return ptr_ == it.ptr_;
}

template<typename T>
inline bool HistoryQueue<T>::const_iterator::operator!=(const const_iterator& it) const
{
	return ptr_ != it.ptr_;
}
