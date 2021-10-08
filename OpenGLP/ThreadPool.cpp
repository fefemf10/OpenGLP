#include "ThreadPool.hpp"

ThreadPool::ThreadPool(std::size_t countThread)
{
	start(countThread);
}

ThreadPool::~ThreadPool()
{
	stop();
}

void ThreadPool::start(std::size_t countThread)
{
	for (size_t i = 0; i < countThread; ++i)
	{
		threads.emplace_back([=] {
			while (true)
			{
				Task task;
				{
					std::unique_lock<std::mutex> lock(eventMutex);
					eventVar.wait(lock, [=] { return stopping || !tasks.empty(); });
					if (stopping && tasks.empty())
					{
						break;
					}
					task = std::move(tasks.front());
					tasks.pop();
				}
				task();
			}
			});
	}
}

void ThreadPool::stop() noexcept
{
	{
		std::unique_lock<std::mutex> lock(eventMutex);
		stopping = true;
	}
	eventVar.notify_all();
	for (auto& thread : threads)
		if (thread.joinable())
			thread.join();
}