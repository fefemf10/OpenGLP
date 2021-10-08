#pragma once
#include <future>
#include <functional>
#include <queue>
class ThreadPool
{
public:
	using Task = std::function<void()>;
	explicit ThreadPool(std::size_t countThread);
	void stop() noexcept;
	~ThreadPool();
	template<class T>
	auto enqueue(T task)->std::future<decltype(task())>
	{
		auto wrapper = std::make_shared<std::packaged_task<decltype(task()) ()>>(std::move(task));
		{
			std::unique_lock<std::mutex> lock(eventMutex);
			tasks.emplace([=] {
				(*wrapper)();
				});
		}
		eventVar.notify_one();
		return wrapper->get_future();
	}
private:
	std::vector<std::thread> threads;
	std::condition_variable eventVar;
	std::mutex eventMutex;
	bool stopping = false;
	std::queue<Task> tasks;
	void start(std::size_t countThread);

};
