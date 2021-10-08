#include "Event.hpp"

bool* Event::_keys;
unsigned int* Event::_frames;
unsigned int Event::_current = 0;
float Event::deltaX = 0.0f;
float Event::deltaY = 0.0f;
float Event::x = 0.0f;
float Event::y = 0.0f;
bool Event::_cursor_locked = false;
bool Event::_cursor_started = false;
float Event::deltaScroll = 0;
float Event::scroll = 0;

constexpr int MOUSE_BUTTONS = 1024;

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	if (Event::_cursor_started) {
		Event::deltaX += xpos - Event::x;
		Event::deltaY += Event::y - ypos;
	}
	else {
		Event::_cursor_started = true;
	}
	Event::x = xpos;
	Event::y = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mode) {
	if (action == GLFW_PRESS) {
		Event::_keys[MOUSE_BUTTONS + button] = true;
		Event::_frames[MOUSE_BUTTONS + button] = Event::_current;
	}
	else if (action == GLFW_RELEASE) {
		Event::_keys[MOUSE_BUTTONS + button] = false;
		Event::_frames[MOUSE_BUTTONS + button] = Event::_current;
	}
}

void mouse_scroll_callback(GLFWwindow* window, double xpos, double ypos) {
	Event::deltaScroll += Event::scroll - ypos;
	Event::scroll += ypos / 2.0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		Event::_keys[key] = true;
		Event::_frames[key] = Event::_current;
	}
	else if (action == GLFW_RELEASE) {
		Event::_keys[key] = false;
		Event::_frames[key] = Event::_current;
	}
}

int Event::initialize(GLFWwindow* window)
{
	_keys = new bool[1032]{};
	_frames = new unsigned int[1032]{};

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);
    return 0;
}

void Event::pullEvents()
{
	++_current;
	deltaX = 0.0f;
	deltaY = 0.0f;
	deltaScroll = 0.f;
	glfwPollEvents();
}

bool Event::pressed(int keycode)
{
	if (keycode < 0 || keycode >= MOUSE_BUTTONS)
		return false;
	return _keys[keycode];
}

bool Event::jpressed(int keycode)
{
	if (keycode < 0 || keycode >= MOUSE_BUTTONS)
		return false;
	return _keys[keycode] && _frames[keycode] == _current;
}

bool Event::clicked(int button)
{
	int index = MOUSE_BUTTONS + button;
	return _keys[index];
}

bool Event::jclicked(int button)
{
	int index = MOUSE_BUTTONS + button;
	return _keys[index] && _frames[index] == _current;
}
