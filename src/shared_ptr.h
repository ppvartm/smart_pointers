#pragma once
#include<functional>

template<typename T>
class weak_ptr;

template <typename T>
class shared_ptr
{
private:

	friend class weak_ptr<T>;

	//T* ptr;
	//size_t* counter;
	template<typename T, typename ... D>
	friend shared_ptr<T> make_shared(D&&... a);

	struct object
	{
		T obj;
		object(const T& ptr_) :obj(ptr_) {};
		object(T&& ptr_) :obj(ptr_) {};
	};
	struct control_block_for_make_shared : public object
	{
		size_t shared_counter;
		size_t weak_counter;
		control_block_for_make_shared(const T& ob, size_t sc, size_t wc) :object(ob), shared_counter(sc), weak_counter(wc) {};
	};
	struct control_block_for_constructor
	{
		size_t shared_counter;
		size_t weak_counter;
		std::function<void(T*)> deleter = [](T* ptr) {delete ptr; };
		control_block_for_constructor(size_t sc, size_t wc) :shared_counter(sc), weak_counter(wc) {};
		control_block_for_constructor(size_t sc, size_t wc, const std::function<void(T*)>& del) :shared_counter(sc), weak_counter(wc), deleter(del) {};
	};

	object* pointer;
	control_block_for_constructor* defoalt_control_block = nullptr;

	shared_ptr(control_block_for_make_shared* cb) : pointer(cb) {};
	shared_ptr(object* pointer_, control_block_for_constructor* control_block_) :pointer(pointer_), defoalt_control_block(control_block_)
	{
		if (defoalt_control_block == nullptr)
			++(static_cast<control_block_for_make_shared*>(pointer)->shared_counter);
		else ++(defoalt_control_block->shared_counter);
	}
public:
	shared_ptr() :pointer(nullptr), defoalt_control_block(nullptr) {}
	shared_ptr(T* ptr_) :pointer(reinterpret_cast<object*>(ptr_)), defoalt_control_block(new control_block_for_constructor(1, 0)) {}
	template<typename Del>
	shared_ptr(T* ptr_, const Del& deleter_) : pointer(reinterpret_cast<object*>(ptr_)), defoalt_control_block(new control_block_for_constructor(1, 0, deleter_)) {}


	shared_ptr(const shared_ptr<T>& s_p) {
		if (s_p.defoalt_control_block != nullptr)
		{
			pointer = s_p.pointer;
			defoalt_control_block = s_p.defoalt_control_block;
			++(defoalt_control_block->shared_counter);
		}
		else {
			pointer = s_p.pointer;
			++(static_cast<control_block_for_make_shared*>(pointer)->shared_counter);
		}
	}
	shared_ptr(shared_ptr<T>&& s_p) noexcept
	{
		if (s_p.defoalt_control_block != nullptr)
		{
			pointer = s_p.pointer;
			defoalt_control_block = s_p.defoalt_control_block;
			s_p.pointer = nullptr;
			s_p.defoalt_control_block = nullptr;
		}
		else {
			pointer = s_p.pointer;
			s_p.pointer = nullptr;
		}
	}
	~shared_ptr()
	{
		if (defoalt_control_block == nullptr)
		{
			if (--(static_cast<control_block_for_make_shared*>(pointer)->shared_counter) == 0)
			{
				pointer->obj.~T();
				if (static_cast<control_block_for_make_shared*>(pointer)->weak_counter == 0)
					delete(pointer);
			}
		}
		else
		{
			if (--(defoalt_control_block->shared_counter) == 0)
			{
				defoalt_control_block->deleter(reinterpret_cast<T*>(pointer));
				if (defoalt_control_block->weak_counter == 0)
					delete(defoalt_control_block);
			}
		}
	}
	shared_ptr<T>& operator=(const shared_ptr<T>& s_p)
	{
		if (this == &s_p) return *this;
		this->~shared_ptr();


		if (s_p.defoalt_control_block != nullptr)
		{
			pointer = s_p.pointer;
			defoalt_control_block = s_p.defoalt_control_block;
			++(defoalt_control_block->shared_counter);
		}
		else {
			pointer = s_p.pointer;
			++(static_cast<control_block_for_make_shared*>(pointer)->shared_counter);
		}
		return *this;
	}

	shared_ptr<T>& operator=(shared_ptr<T>&& s_p) noexcept
	{
		this->~shared_ptr();

		if (s_p.defoalt_control_block != nullptr)
		{
			pointer = s_p.pointer;
			defoalt_control_block = s_p.defoalt_control_block;
			s_p.pointer = nullptr;
			s_p.defoalt_control_block = nullptr;
		}
		else {
			pointer = s_p.pointer;
			s_p.pointer = nullptr;
		}
		return *this;
	}



};

template<typename T, typename ... D>
shared_ptr<T> make_shared(D&&... args)
{
	auto p_cb = new  typename shared_ptr<T>::control_block_for_make_shared(T(std::forward<D>(args)...), 1, 0);
	return shared_ptr<T>(p_cb);
}

template <typename T>
class weak_ptr
{
private:
	typename shared_ptr<T>::object* pointer;
	typename shared_ptr<T>::control_block_for_constructor* cb_for_con;
	//typename shared_ptr<T>::control_block_for_make_shared* cb_for_mk_sh;
public:
	weak_ptr(const shared_ptr<T>& sh_ptr)
	{
		if (sh_ptr.defoalt_control_block == nullptr)
		{

			cb_for_con = nullptr;
			pointer = sh_ptr.pointer;
			++(static_cast<typename shared_ptr<T>::control_block_for_make_shared*>(pointer)->weak_counter);
		}
		else
		{
			pointer = sh_ptr.pointer;
			cb_for_con = sh_ptr.defoalt_control_block;
			++(cb_for_con->weak_counter);
		}
	}

	weak_ptr(const weak_ptr<T>& wk_ptr) :pointer(wk_ptr.pointer), cb_for_con(wk_ptr.cb_for_con)
	{
		if (cb_for_con == nullptr)
			++(static_cast<typename shared_ptr<T>::control_block_for_make_shared*>(pointer)->weak_counter);
		else ++(cb_for_con->weak_counter);
	}

	weak_ptr(weak_ptr<T>&& wk_ptr)noexcept :pointer(wk_ptr.pointer), cb_for_con(wk_ptr.cb_for_con)
	{
		wk_ptr.cb_for_con = nullptr;
		wk_ptr.pointer = nullptr;
	}

	bool expired() const
	{
		if (cb_for_con == nullptr)
			return !(static_cast<typename shared_ptr<T>::control_block_for_make_shared*>(pointer)->shared_counter);
		return !(cb_for_con->shared_counter);
	}
	shared_ptr<T> lock()
	{
		if (cb_for_con == nullptr)
		{
			if (static_cast<typename shared_ptr<T>::control_block_for_make_shared*>(pointer)->shared_counter)
				return shared_ptr<T>(pointer, cb_for_con);
			return shared_ptr<T>();
		}
		else
		{
			if (cb_for_con->shared_counter)
				return shared_ptr<T>(pointer, cb_for_con);
			return shared_ptr<T>();
		}
	}


	~weak_ptr()
	{
		if (cb_for_con == nullptr)
		{
			--(static_cast<typename shared_ptr<T>::control_block_for_make_shared*>(pointer)->weak_counter);
			if ((static_cast<typename shared_ptr<T>::control_block_for_make_shared*>(pointer)->weak_counter == 0) &&
				(static_cast<typename shared_ptr<T>::control_block_for_make_shared*>(pointer)->shared_counter == 0))
				delete pointer;
		}
		else
		{
			--(cb_for_con->weak_counter);
			if ((cb_for_con->weak_counter == 0) && (cb_for_con->shared_counter == 0))
				delete cb_for_con;
		}
	}

};