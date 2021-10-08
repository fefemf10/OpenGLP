#pragma once
#include <vector>
#include <unordered_set>
#include <algorithm>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
class queuep
{
public:
	queuep();
	size_t count() const;
	void enqueue(glm::ivec2 pos, bool less = true);
	glm::ivec2 peek();
	glm::ivec2 dequeue();
	void remove(glm::ivec2 pos);
	void update(glm::ivec2 pos, bool less = true);
private:
	struct item
	{
		glm::ivec2 pos;
		int32_t priority;
		bool operator<(const item& other);
		bool operator>(const item& other);
	};
	std::vector<item> items;
	std::unordered_set<glm::ivec2> set;
	glm::ivec2 playerPos{};
};
