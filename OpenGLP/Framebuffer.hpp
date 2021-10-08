#pragma once
#include <vector>
#include <GLM/glm.hpp>
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
class Framebuffer
{
public:
	Framebuffer();
	Framebuffer(const Framebuffer& other) = delete;
	Framebuffer(Framebuffer&& other) noexcept;
	~Framebuffer();
	GLuint getFBO() const;
	GLuint getTexture(size_t index) const;
	GLuint getDepthTexture() const;
	GLenum checkStatus() const;
	void bind();
	void unbind();
	void attachTexture(GLenum attachment, GLenum sizedFormat, GLint width, GLint height);
	void attachDepth(GLint width, GLint height);
	void useTextures(GLuint startUnit = GL_TEXTURE0);
	void useBuffer(GLenum startAttachment = GL_COLOR_ATTACHMENT0);
private:
	GLuint fbo{};
	std::vector<GLuint> textures;
	GLuint depthTexture{};
};

