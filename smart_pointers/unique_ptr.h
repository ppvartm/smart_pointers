#pragma once

template <typename T>
class unique_ptr_deleter
{
public:
	void operator()(T* obj)
	{
		delete obj;
	}
};

template <typename T>
class unique_ptr_deleter<T[]>
{
public:
	void operator()(T* obj)
	{
		delete[] obj;
	}
};


template<typename T, typename D = unique_ptr_deleter<T>>
class unique_ptr
{
private:
	T* ptr;
	D deleter;
public:
	unique_ptr(T* ptr_ = nullptr) noexcept :ptr(ptr_) {};
	unique_ptr(T* ptr_, const D& deleter_)noexcept :ptr(ptr_), deleter(deleter_) {};
	unique_ptr(const unique_ptr<T>& obj) = delete;
	unique_ptr& operator=(const unique_ptr<T>& obj) = delete;

	unique_ptr(unique_ptr<T>&& obj)noexcept : ptr(obj.ptr)
	{
		obj.ptr = nullptr;
	}

	unique_ptr& operator=(unique_ptr<T>&& obj) noexcept
	{
		if (this == &obj)
			return *this;
		delete ptr;
		ptr = obj.ptr;
		obj.ptr = nullptr;
		return *this;
	}





	~unique_ptr()
	{
		deleter(ptr);
	}
};









template<typename T, typename D >
class unique_ptr<T[], D>
{
private:
	T* ptr;
	D deleter;
public:
	unique_ptr(T* ptr_ = nullptr)noexcept :ptr(ptr_) {};
	unique_ptr(T* ptr_, const D& deleter_)noexcept :ptr(ptr_), deleter(deleter_) {};
	unique_ptr(const unique_ptr<T>& obj) = delete;
	unique_ptr& operator=(const unique_ptr<T>& obj) = delete;

	unique_ptr(unique_ptr<T>&& obj)noexcept : ptr(obj.ptr)
	{
		obj.ptr = nullptr;
	}

	unique_ptr& operator=(unique_ptr<T>&& obj) noexcept
	{
		if (this == &obj)
			return *this;
		delete ptr;
		ptr = obj.ptr;
		obj.ptr = nullptr;
		return *this;
	}

	~unique_ptr()
	{
		deleter(ptr);
	}
};
