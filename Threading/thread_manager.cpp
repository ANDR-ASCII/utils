#include "thread_manager.h"

ThreadManager::ThreadManager()
{
	for (size_t i = 0; i < std::thread::hardware_concurrency(); ++i)
	{
		m_threads.emplace_back(std::move(std::make_unique<Thread>()));
		m_threads[i]->start();
	}
}

ThreadManager::~ThreadManager()
{
	for (size_t i = 0; i < m_threads.size(); ++i)
	{
		m_threads[i]->finish();
	}

	for (size_t i = 0; i < m_threads.size(); ++i)
	{
		m_threads[i]->wait();
	}
}

Thread* ThreadManager::thread(ThreadingHelpers::ThreadIdentifier id)
{
	auto iter = std::find_if(std::begin(m_threads), std::end(m_threads), 
			[id](std::unique_ptr<Thread> const& thread) { return thread->id() == id; });

	return iter != std::end(m_threads) ? iter->get() : nullptr;
}

void ThreadManager::executeTask(ThreadingHelpers::Function task, 
	ThreadingHelpers::Function callback,
	ThreadingHelpers::ThreadIdentifier targetThread)
{

}
