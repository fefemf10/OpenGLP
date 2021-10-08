#include "Window.hpp"

int main()
{
	if (glfwInit() == GLFW_FALSE)
	{
		return 1;
	}
	else
	{
		//Logger::init();
		Window window("Game");
		window.loop();
		//Logger::deinit();
		glfwTerminate();
	}
	return 0;
}