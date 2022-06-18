#include "Dispatcher.hpp"

void Dispatcher::subscribe(Events event, Slot&& slot)
{
	observers[event].push_back(slot);
}

void Dispatcher::post(const Event* event)
{
	queueEvents.emplace(event);
}

void Dispatcher::process()
{
	while (!queueEvents.empty())
	{
		const Event* event = queueEvents.front();
		const Events& name = event->type();
		if (observers.contains(name))
			for (const Dispatcher::Slot& observer : observers.at(name))
				observer(event);
		delete event;
		queueEvents.pop();
	}
}
