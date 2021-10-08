#pragma once
#include <GLM/glm.hpp>
class Hitbox
{
public:
	Hitbox(const glm::vec3& position, const glm::vec3& size);
	void setPosition(const glm::vec3& position);
	void setSize(const glm::vec3& size);
	bool intersects(const Hitbox& hitbox);
	bool contains(const Hitbox& hitbox);
	bool contains(const glm::vec3& point);
	const glm::vec3& getPosition() const;
	const glm::vec3& getSize() const;
private:
	glm::vec3 position;
	glm::vec3 size;
};

