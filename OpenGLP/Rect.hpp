#pragma once
#include <GLM/glm.hpp>
class Rect
{
public:
	Rect();
	Rect(const glm::vec2& position, const glm::vec2& size);
	void setPosition(const glm::vec2& position);
	void setSize(const glm::vec2& size);
	const glm::vec2& getPosition() const;
	const glm::vec2& getSize() const;
	bool contains(const glm::vec2& point) const;
	bool intersects(const Rect& rect) const;
	bool contains(const Rect& rect) const;
private:
	glm::vec2 position;
	glm::vec2 size;
};

