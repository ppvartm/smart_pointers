#pragma once


template<typename T>
class shared_ptr
{
private:
	T* ptr = nullptr;
	size_t* counter = nullptr;
public:
	shared_ptr(T* ptr_) :ptr(ptr_), counter(new size_t(1)) {}

	shared_ptr(const shared_ptr<T>& ptr_):ptr(ptr_.ptr),counter(ptr_.counter)
	{
		++(*counter);
	}

	T& operator *() const
	{
		return *ptr;
	}

	~shared_ptr()
	{
		if (*counter > 1)
			--(*counter);
		else
		{
			delete ptr;
			delete counter;
		}
	}


	size_t use_count() const
	{
		return *counter;
	}

};