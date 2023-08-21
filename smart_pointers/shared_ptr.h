#pragma once


template<typename T>
class shared_ptr
{
private:

	T* ptr = nullptr;
	size_t* counter = nullptr;

	
	template<typename U>
	struct control_block
	{
		size_t* counter_;
		U* object;

		template<typename ... Args>
		control_block(size_t count, Args&& ... args) :counter_(count), object(std::forward<U>(args)...) {};
	};

	template <typename U, typename ... Args>
	friend shared_ptr<U> make_shared(Args&& ... args);

	shared_ptr(control_block<T>* cb) :ptr(cb->object), counter(cb->counter_) {};


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


template<typename T, typename ... Args>
shared_ptr<T> make_shared(Args&& ... args)
{

	shared_ptr<int> ptr(new int(4));
	auto ptr_ = new control_block<T>(1, std::forward<T>(args)...);
	return shared_ptr<T>(ptr_);
}