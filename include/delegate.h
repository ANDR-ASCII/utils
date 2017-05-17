#pragma once

namespace My
{

// Implementation delegate for plain global and static functions

template <typename ReturnType, typename... Args>
class Delegate;

template <typename ReturnType, typename... Args>
class Delegate<ReturnType(*)(Args...)>
{
public:
	typedef ReturnType(*InternalCallableType)(Args...);

	Delegate() = default;

	Delegate& operator+=(InternalCallableType function)
	{
		m_callables.insert(function);

		return *this;
	}

	Delegate& operator-=(InternalCallableType function)
	{
		m_callables.erase(function);

		return *this;
	}

	ReturnType operator()(Args... args) const
	{
		ReturnType result{};

		auto saveResultLambda = [&result, &args...](InternalCallableType function)
		{
			constexpr bool canMove =
				!std::is_pod<ReturnType>::value &&
				!std::is_pointer<ReturnType>::value &&
				!std::is_reference<ReturnType>::value &&
				!std::is_member_pointer<ReturnType>::value &&
				!std::is_null_pointer<ReturnType>::value;

			result = canMove ? std::move(function(args...)) : function(args...);
		};

		std::for_each(m_callables.begin(), m_callables.end(), saveResultLambda);

		return result;
	}

private:
	std::unordered_set<InternalCallableType> m_callables;
};

template <typename ReturnType, typename... Args>
class Delegate<ReturnType(Args...)> : public Delegate<ReturnType(*)(Args...)> {};

template <typename ReturnType, typename... Args>
class Delegate<ReturnType(&)(Args...)> : public Delegate<ReturnType(*)(Args...)> {};

// Implementation delegate for methods

template <typename ReturnType, typename ObjectType, typename... Args>
class Delegate<ReturnType(ObjectType::*)(Args...)>
{
public:
	typedef ReturnType(ObjectType::*InternalCallableType)(Args...);

	Delegate() = default;

	Delegate& operator+=(InternalCallableType function)
	{
		m_callables.insert(function);

		return *this;
	}

	Delegate& operator-=(InternalCallableType function)
	{
		m_callables.erase(function);

		return *this;
	}

	ReturnType operator()(ObjectType const& object, Args... args) const
	{
		ReturnType result{};

		auto saveResultLambda = [&result, &args...](InternalFunctionType function)
		{
			constexpr bool canMove =
				!std::is_pod<ReturnType>::value &&
				!std::is_pointer<ReturnType>::value &&
				!std::is_reference<ReturnType>::value &&
				!std::is_member_pointer<ReturnType>::value &&
				!std::is_null_pointer<ReturnType>::value;

			result = canMove ? std::move(object.function(args...)) : function(args...);
		};

		std::for_each(m_callables.begin(), m_callables.end(), saveResultLambda);

		return result;
	}

private:
	std::unordered_set<InternalCallableType> m_callables;
};

}