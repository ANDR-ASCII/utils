#pragma once

template <typename FunctionType>
class Callback;

template <typename ReturnType, typename... Args>
class Callback<ReturnType(Args...)>
{
public:
	virtual ~Callback() = default;
	virtual ReturnType call(Args&&...) const = 0;
};

template <typename FunctionType>
class CallbackFunction;

template <typename ReturnType, typename... Args>
class CallbackFunction<ReturnType(*)(Args...)> : public Callback<ReturnType(Args...)>
{
private:
	using FunctionPointer = ReturnType(*)(Args...);

public:
	CallbackFunction(FunctionPointer pCallableFunction)
		: m_pCallable(pCallableFunction)
	{
	}

	virtual ReturnType call(Args&&... args) const override
	{
		m_pCallable(std::forward<Args>(args)...);
	}

private:
	FunctionPointer m_pCallable;
};

template <typename ReturnType, typename... Args>
class CallbackFunction<ReturnType(Args...)> final : public CallbackFunction<ReturnType(*)(Args...)>
{
public:
	using CallbackFunction<ReturnType(*)(Args...)>::CallbackFunction;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

template <typename ObjectType, typename MemberFunctionType>
class CallbackMemberFunction;

template <typename ObjectType, typename ReturnType, typename... Args>
class CallbackMemberFunction<ObjectType, ReturnType(*)(Args...)> final : public Callback<ReturnType(Args...)>
{
public:

private:

};