#pragma once

#include <typeinfo>
#include <type_traits>

class AnyCallback
{
public:

	template <typename F>
	AnyCallback(F* f, std::enable_if<std::is_function<F>::value>::type* = nullptr)
		: m_callbackHolder(f)
	{
	}

	template <typename... Args>
	void call(Args&&... args)
	{
		m_callbackHolder->type()
	}

private:

	struct ICallbackHolder
	{
		virtual std::type_info const& type() const noexcept = 0;
	};

	template <typename F>
	struct CallbackHolder : ICallbackHolder
	{
		virtual std::type_info const& type() const noexcept override
		{
			return typeid(F);
		}

		template <typename... Args>
		std::result_of_t<F> operator()(Args&&... args)
		{
			return callback(std::forward<Args>(args)...);
		}

		F callback;
	};

	ICallbackHolder* m_callbackHolder;
};
