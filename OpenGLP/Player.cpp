#include "Player.hpp"

Player::Player() : yaw{ -90.f }, pitch{}, position{ 0.f, 500.f, 0.f }, renderDistance{ 8 }, speed(1.f)
{
}

void Player::update(const float& dt)
{
	yaw += ImGui::GetIO().MouseDelta.x * 0.1f;
	pitch += ImGui::GetIO().MouseDelta.y * -0.1f;
	if (pitch > 89.0f)
		pitch = 89.0f;
	else if (pitch < -89.0f)
		pitch = -89.0f;

	look = glm::normalize(glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)), sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
	right = glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), look));

	if (ImGui::IsKeyPressed(GLFW_KEY_F))
	{
		fly ^= 1;
	}
	else if (ImGui::IsKeyPressed(GLFW_KEY_3))
	{
		changeSpeed ^= 1;
		if (changeSpeed)
			speed = 32.f;
		else
			speed = 1.f;
	}
	//if (ImGui::IsKeyDown(GLFW_KEY_S))
		//player.move(look * -speed * dt);

	//if (ImGui::IsKeyDown(GLFW_KEY_W))
	//{
	//	/*glm::vec3 m(look.x * speed * dt, 0.f, look.z * speed * dt);*/
	//	if (collide.x || collide.y)
	//		move({ 0.f, 0.f, m.z });
	//	else move({ m.x, 0.f, m.z });
	//	if (collide.w || collide.z)
	//		move({ m.x, 0.f, 0.f });
	//	else move({ m.x, 0.f, m.z });
	//}

	/*if (ImGui::IsKeyDown(GLFW_KEY_S))
	{
		move({ look.x * -speed * dt, 0.f, look.z * -speed * dt });
	}*/
	/*if (ImGui::IsKeyDown(GLFW_KEY_A))
	{
		move(right * speed * dt);
	}

	if (ImGui::IsKeyDown(GLFW_KEY_D))
	{
		move(right * -speed * dt);
	}*/
}

void Player::setPitch(float pitch)
{
	this->pitch = pitch;
}

void Player::setYaw(float yaw)
{
	this->yaw = yaw;
}

void Player::setPosition(const glm::vec3& position)
{
	this->position = position;
}

void Player::move(const glm::vec3& position)
{
	this->position += position;
}

void Player::setRenderDistance(const unsigned char& renderDistance)
{
	this->renderDistance = renderDistance;
}

const unsigned char& Player::getRenderDistance() const
{
	return renderDistance;
}

const glm::vec3& Player::getPosition() const
{
	return position;
}

const float& Player::getPitch() const
{
	return pitch;
}

const glm::vec3& Player::getLook() const
{
	return look;
}

const glm::vec3& Player::getRight() const
{
	return right;
}

const float& Player::getYaw() const
{
	return yaw;
}

const float& Player::getVerticalAcceleration() const
{
	return verticalAcceleration;
}

void Player::draw()
{

}