#pragma once

#include "thread.h"
#include <vector>
#include <memory>

class ThreadManager final
{
public:
	ThreadManager();
	~ThreadManager();

	void executeTask(ThreadingHelpers::Function task, 
		ThreadingHelpers::Function callback, 
		ThreadingHelpers::ThreadIdentifier targetThread);

	Thread* thread(ThreadingHelpers::ThreadIdentifier id);

private:
	std::vector<std::unique_ptr<Thread>> m_threads;
};
