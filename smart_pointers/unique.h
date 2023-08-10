#pragma once


template<typename T>
class unique_ptr
{
private:
	T* ptr;
public:
	unique_ptr(T* ptr_) :ptr(ptr_) {}
	unique_ptr(const unique_ptr<T>& ptr_) = delete;
	unique_ptr<T>& operator=(const unique_ptr<T>& ptr_) = delete;

	unique_ptr(unique_ptr<T>&& ptr_) :ptr(ptr_) { ptr_ = nullptr; }
	unique_ptr<T>& operator=(unique_ptr<T>&& ptr_)
	{
		delete ptr;
		ptr = ptr_;
		ptr_ = nullptr;
	};

	T operator*() const
	{
		return *ptr;
	}

	T* operator->() const
	{
		return ptr;
	}

	~unique_ptr() { delete ptr; }
};