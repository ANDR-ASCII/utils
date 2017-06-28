#pragma once

#include <stdexcept>
#include <typeinfo>

namespace Common
{

class BadAnyCastException : public std::runtime_error
{
public:
	BadAnyCastException(const std::string& err)
		: std::runtime_error(err)
	{
	}
};

class Any
{
private:
	template <typename T>
	struct IsConstReference : public std::false_type {};

	template <typename T>
	struct IsConstReference<const T&> : public std::true_type {};

public:
	Any()
		: m_placeHolder(nullptr)
	{
	}

	template <typename T,
		typename = std::enable_if_t<!std::is_same<T, Any>::value>
	>
	Any(T&& value)
		: m_placeHolder(new Holder<std::decay_t<T>>(static_cast<T&&>(value)))
	{
	}

	Any(const Any& other)
		: m_placeHolder(other.m_placeHolder ? other.m_placeHolder->clone() : nullptr)
	{
	}

	Any(Any&& other)
		: m_placeHolder(other.m_placeHolder)
	{
		other.m_placeHolder = nullptr;
	}

	~Any()
	{
		delete m_placeHolder;
	}

	Any& operator=(const Any& other)
	{
		Any(other).swap(*this);
		return *this;
	}

	Any& operator=(Any&& other)
	{
		other.swap(*this);
		Any().swap(other);
		return *this;
	}

	template <typename T>
	Any& operator=(T&& object)
	{
		Any(static_cast<T&&>(object)).swap(*this);
		return *this;
	}

	void reset() noexcept
	{
		delete m_placeHolder;
		m_placeHolder = nullptr;
	}

	bool hasValue() const noexcept
	{
		return m_placeHolder != nullptr;
	}

	const type_info& type() const noexcept
	{
		return m_placeHolder ? m_placeHolder->type() : typeid(void);
	}

	Any& swap(Any& other) noexcept
	{
		std::swap(m_placeHolder, other.m_placeHolder);
		return *this;
	}

	template <typename T>
	friend T* anyCast(const Any* anyObject)
	{
		if (!anyObject->m_placeHolder || anyObject->m_placeHolder->type() != typeid(T))
		{
			throw BadAnyCastException("Bad AnyObject cast");
		}

		return &static_cast<Any::Holder<T>*>(anyObject->m_placeHolder)->value;
	}	
	
	template <typename T>
	friend T* anyCast(const Any* anyObject, std::nothrow_t)
	{
		return anyObject->m_placeHolder->type() == typeid(T) ?
			&static_cast<Any::Holder<T>*>(anyObject->m_placeHolder)->value : nullptr;
	}

private:

	struct PlaceHolder
	{
		virtual ~PlaceHolder() noexcept = default;

		virtual const type_info& type() const noexcept = 0;

		virtual PlaceHolder* clone() const = 0;
	};

	template <typename T>
	struct Holder : public PlaceHolder
	{
		Holder(const T& val) noexcept
			: value(val)
		{
		}

		Holder(T&& val) noexcept
			: value(static_cast<T&&>(val))
		{
		}

		virtual const type_info& type() const noexcept override
		{
			return typeid(T);
		}

		virtual PlaceHolder* clone() const override
		{
			return new Holder<T>(value);
		}
	
		T value;
	};

private:
	PlaceHolder* m_placeHolder;
};

template <typename T>
T* anyCast(const Any* anyObject);

}
