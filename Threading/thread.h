#pragma once

#include "threading_helpers.h"
#include <condition_variable>
#include <functional>
#include <atomic>

class Thread final
{
public:
	enum ThreadState
	{
		ThreadWorkingState,
		ThreadFinishedState
	};

	Thread();
	~Thread();

	void start();
	void finish();
	void wait();

	int state() const noexcept;

	void pushTask(ThreadingHelpers::Function task);

	ThreadingHelpers::ThreadIdentifier id() const;

private:
	void messageHandlerLoop();

private:
	std::thread m_thread;
	ThreadingHelpers::LockingQueue<ThreadingHelpers::Function> m_taskQueue;

	std::atomic_bool m_onAboutToFinish;
	std::atomic_int m_threadState;

	std::mutex mutable m_mutex;
	std::condition_variable m_waitCondition;
};

