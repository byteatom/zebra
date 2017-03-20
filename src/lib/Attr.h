#ifndef PROPERTY_H
#define PROPERTY_H

#include <boost/signals2.hpp>

template<typename Type>
class Attr {
public:
	Attr() {}

	Attr(const Attr<Type> &other) : value_{other} {}
	Attr(const Type &other) : value_{other} {}

	template <typename... Types>
	Attr(Types... args) : value_{args...} {}

	typedef boost::signals2::signal<void ()> SignalType;

	template <typename... Types>
	boost::signals2::connection connect(Types... args)
	{
		return signal.connect(args...);
	}

	void set(const Type &value)
	{
		if (value_ != value) {
			value_ = value;
			signal();
		}
	}

	Attr<Type> &operator=(const Attr<Type> &value)
	{
		set(value);
		return *this;
	}

	Type &operator=(const Type &value)
	{
		set(value);
		return value_;
	}

	Type get() const
	{
		return value_;
	}

	Type value() const
	{
		return value_;
	}

	Type& ref()
	{
		return value_;
	}

	Type* operator->() { return &value_; }

	const Type* operator->() const { return &value_; }

	operator Type() const { return value_; }

	void changed()
	{
		signal();
	}

private:
	Type value_ = Type();
	SignalType signal;
};

#endif // PROPERTY_H
