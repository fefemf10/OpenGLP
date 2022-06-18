#pragma once
#include <functional>
#include <queue>
#include "Event.hpp"
class Dispatcher
{
public:
	using Slot = std::function<void(const Event*)>;
	void subscribe(Events event, Slot&& slot);
	void post(const Event* event);
	void process();
private:
	std::unordered_map<Events, std::vector<Slot>> observers;
	std::queue<const Event*> queueEvents;
};