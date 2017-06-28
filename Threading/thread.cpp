#include "thread.h"

Thread::Thread()
	: m_onAboutToFinish(false)
	, m_threadState(ThreadFinishedState)
{
}

Thread::~Thread()
{
	finish();
	wait();
}

void Thread::start()
{
	m_thread = std::thread(&Thread::messageHandlerLoop, this);
}

ThreadingHelpers::ThreadIdentifier Thread::id() const
{
	return m_thread.get_id();
}

void Thread::pushTask(ThreadingHelpers::Function task)
{
	m_taskQueue.push(task);
	m_waitCondition.notify_one();
}

void Thread::finish()
{
	m_onAboutToFinish.store(true);
	m_waitCondition.notify_one();
}

void Thread::wait()
{
	if (m_thread.joinable())
	{
		m_thread.join();
	}
}

int Thread::state() const noexcept
{
	return m_threadState.load();
}

void Thread::messageHandlerLoop()
{
	m_threadState.store(ThreadWorkingState);

	while (true)
	{
		if (m_onAboutToFinish.load())
		{
			break;
		}

		std::unique_lock<std::mutex> lock(m_mutex);
		m_waitCondition.wait(lock, [this] { return !m_taskQueue.empty() || m_onAboutToFinish.load(); });

		while (!m_taskQueue.empty())
		{
			ThreadingHelpers::Function task = m_taskQueue.pop();

			task();
		}
	}

	m_onAboutToFinish.store(false);
	m_threadState.store(ThreadFinishedState);
}
