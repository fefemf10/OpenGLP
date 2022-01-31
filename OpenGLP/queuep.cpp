#include "queuep.hpp"

queuep::queuep()
{

}

size_t queuep::count() const
{
	return set.size();
}

void queuep::enqueue(glm::ivec2 pos, bool less)
{
	if (set.insert(pos).second)
	{
		items.push_back({ pos, (int)glm::round(glm::distance((glm::vec2)pos, (glm::vec2)playerPos)) });
		if (less)
			std::push_heap(items.begin(), items.end());
		else
			std::push_heap(items.begin(), items.end(), std::greater<>());
	}
}

glm::ivec2 queuep::peek()
{
	while (true)
	{
		item i = items.front();
		auto it = set.find(i.pos);
		if (it == set.end())
		{
			std::pop_heap(items.begin(), items.end());
			items.pop_back();
		}
		else
		{
			return i.pos;
		}
	}
}

glm::ivec2 queuep::dequeue()
{
	while (true)
	{
		item i = items.front();
		std::pop_heap(items.begin(), items.end());
		items.pop_back();
		auto it = set.find(i.pos);
		if (it != set.end())
		{
			set.erase(it);
			return i.pos;
		}
	}
}

void queuep::remove(glm::ivec2 pos)
{
	set.erase(pos);
}

void queuep::clear()
{
	set.clear();
	items.clear();
}

void queuep::update(glm::ivec2 pos, bool less)
{
	if (pos != playerPos)
	{
		playerPos = pos;
		for (auto& item : items)
		{
			auto it = set.find(item.pos);
			if (it != set.end())
			{
				item.priority = (int)glm::round(glm::distance((glm::vec2)pos, (glm::vec2)item.pos));
			}
		}
		if (less)
			std::make_heap(items.begin(), items.end());
		else
			std::make_heap(items.begin(), items.end(), std::greater<>());
	}
}

bool queuep::item::operator<(const item& other)
{
	return this->priority > other.priority;
}

bool queuep::item::operator>(const item& other)
{
	return this->priority < other.priority;
}
