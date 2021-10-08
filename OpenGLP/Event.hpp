#pragma once
#include <GLFW/glfw3.h>

class Event
{
public:
	static bool* _keys;
	static unsigned int* _frames;
	static unsigned int _current;
	static float deltaX;
	static float deltaY;
	static float x;
	static float y;
	static bool _cursor_locked;
	static bool _cursor_started;
	static float deltaScroll;
	static float scroll;
	static int initialize(GLFWwindow* window);
	static void pullEvents();

	static bool pressed(int keycode);
	static bool jpressed(int keycode);

	static bool clicked(int button);
	static bool jclicked(int button);
};

