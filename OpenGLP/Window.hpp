#pragma once
#include <iostream>
#include <random>
#include <string>
#include <GLM/glm.hpp>
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include "../IMGUI/imgui.h"
#include "../IMGUI/imgui_impl_glfw.h"
#include "../IMGUI/imgui_impl_opengl3.h"
#include "VAO.hpp"
#include "UBO.hpp"
#include "Camera.hpp"
#include "GL.hpp"
#include "RM.hpp"
#include "Logger.hpp"
#include "World.hpp"
#include "Block.hpp"
class Window
{
public:
	Window(const std::string& title);
	~Window();
	void loop();
private:
	static void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param);
	GLFWwindow* window;
	short dist = 8;
};