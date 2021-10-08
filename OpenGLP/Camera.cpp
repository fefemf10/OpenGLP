#include "Camera.hpp"

Camera::Camera(Player& player) : player(player), projection(glm::perspective(glm::radians(55.f), 16.f / 9.f, 0.05f, 1500.f)), frustum(), look(), right(), up()
{

}

void Camera::update(const float& dt)
{
	look = player.getLook();
	right = player.getRight();
	up = glm::normalize(glm::cross(look, right));
	frustum.clip = glm::transpose(projection * getView());
	frustum.update();
}

const glm::vec3& Camera::getUp() const
{
	return up;
}

glm::mat4 Camera::getView() const
{
	return glm::lookAt(player.getPosition() + glm::vec3(0.f, 1.8f, 0.f), player.getPosition() + look + glm::vec3(0.f, 1.8f, 0.f), up);
}

const glm::mat4& Camera::getProjection() const
{
	return projection;
}

const glm::vec3& Camera::getLook() const
{
	return look;
}

Frustum& Camera::getFrustum()
{
	return frustum;
}