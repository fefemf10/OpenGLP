#include "Rect.hpp"

Rect::Rect() : position(0.f), size(0.f)
{
}

Rect::Rect(const glm::vec2& position, const glm::vec2& size) : position(position), size(size)
{
}

void Rect::setPosition(const glm::vec2& position)
{
	this->position = position;
}

void Rect::setSize(const glm::vec2& size)
{
	this->size = size;
}

const glm::vec2& Rect::getPosition() const
{
	return position;
}

const glm::vec2& Rect::getSize() const
{
	return size;
}

bool Rect::contains(const glm::vec2& point) const
{
	return (point.x >= position.x && point.x <= position.x + size.x) && (point.y >= position.y && point.y <= position.y + size.y);
}

bool Rect::intersects(const Rect& rect) const
{
	return contains(rect.position) || contains(rect.position + glm::vec2(rect.size.x, 0.f)) || contains(rect.position + glm::vec2(0.f, rect.size.y)) || contains(rect.position + rect.size);
}

bool Rect::contains(const Rect& rect) const
{
	return contains(rect.position) && contains(rect.position + glm::vec2(rect.size.x, 0.f)) && contains(rect.position + glm::vec2(0.f, rect.size.y)) && contains(rect.position + rect.size);;
}