#pragma once
#include <fstream>
#include <GLEW/glew.h>

struct Settings
{
	int width = 1280;
	int height = 720;
	bool fullscreen = false;
	bool vsync = true;
};

class GL
{
public:
	static void useTexture(GLuint id, GLuint unit = 0);
	static void useShader(GLuint id);
	static void loadSettings();
	static void saveSettings();
	static Settings settings;
private:
	static GLuint texture;
	static GLuint shader;
};
