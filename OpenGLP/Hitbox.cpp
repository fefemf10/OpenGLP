#include "Hitbox.hpp"

Hitbox::Hitbox(const glm::vec3& position, const glm::vec3& size)
{
    this->position = position;
    this->size = size;
}

void Hitbox::setPosition(const glm::vec3& position)
{
    this->position = position;
}

void Hitbox::setSize(const glm::vec3& size)
{
    this->size = size;
}

bool Hitbox::intersects(const Hitbox& hitbox)
{
    return contains(hitbox.position) ||
        contains(hitbox.position + hitbox.size) ||
        contains({ hitbox.position.x + hitbox.size.x, hitbox.position.y, hitbox.position.z }) ||
        contains({ hitbox.position.x, hitbox.position.y + hitbox.size.y, hitbox.position.z }) ||
        contains({ hitbox.position.x, hitbox.position.y, hitbox.position.z + hitbox.size.z }) ||
        contains({ hitbox.position.x + hitbox.size.x, hitbox.position.y, hitbox.position.z + hitbox.size.z }) ||
        contains({ hitbox.position.x, hitbox.position.y + hitbox.size.y, hitbox.position.z + hitbox.size.z });
}

bool Hitbox::contains(const Hitbox& hitbox)
{
    return contains(hitbox.position) && contains(hitbox.position + hitbox.size);
}

bool Hitbox::contains(const glm::vec3& point)
{
    if (point.x > position.x && point.x < position.x + size.x && point.y > position.y && point.y < position.y + size.y && point.z > position.z && point.z < position.z + size.z)
        return true;
    else return false;
}

const glm::vec3& Hitbox::getPosition() const
{
    return position;
}

const glm::vec3& Hitbox::getSize() const
{
    return size;
}
