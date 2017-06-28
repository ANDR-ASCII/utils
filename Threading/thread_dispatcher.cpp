#include "thread_dispatcher.h"
#include <chrono>

ThreadDispatcher& ThreadDispatcher::instance()
{
	static ThreadDispatcher dispatcher;
	return dispatcher;
}

void ThreadDispatcher::destroy()
{
	m_onAboutToStop.store(true);
}

ThreadDispatcher::ThreadDispatcher()
	: m_onAboutToStop(false)
{
	//
	// start event loop in another thread
	//
	m_dispatcherThread = std::thread(&ThreadDispatcher::loop, this);
}

void ThreadDispatcher::loop()
{
	using namespace std::chrono_literals;

	while (true)
	{
		if (onAboutToStop())
		{
			break;
		}

		if (!m_requests.empty())
		{
			Request const* request = m_requests.front();

			//
			// transfer this request to thread
			// write code for calculating payload per thread
			//
		}

		std::this_thread::sleep_for(5ms);
	}
}

bool ThreadDispatcher::onAboutToStop()
{
	return m_onAboutToStop.load();
}

void ThreadDispatcher::handleRequest(Request const& request)
{
	m_requests.push(request);
}
