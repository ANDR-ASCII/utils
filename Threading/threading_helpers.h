#pragma once

#include <thread>
#include <mutex>
#include <queue>

namespace ThreadingHelpers
{

using ThreadIdentifier = std::thread::id;
using Function = std::function<void()>;

template <typename T>
class LockingQueue
{
public:
	using size_type = typename std::queue<T>::size_type;

	void push(T const& element)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_queue.push(element);
	}

	T pop()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		T element = std::move(m_queue.front());
		m_queue.pop();

		return element;
	}

	bool empty() const noexcept
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_queue.empty();
	}

	size_type size() const noexcept
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_queue.size();
	}

private:
	std::queue<T> m_queue;
	std::mutex mutable m_mutex;
};

}