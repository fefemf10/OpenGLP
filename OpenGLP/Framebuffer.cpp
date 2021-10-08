#include "Framebuffer.hpp"

Framebuffer::Framebuffer()
{
	glCreateFramebuffers(1, &fbo);
}

Framebuffer::Framebuffer(Framebuffer&& other) noexcept
{
	glDeleteFramebuffers(1, &fbo);
	fbo = other.fbo;
	other.fbo = 0;
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &fbo);
}

GLuint Framebuffer::getFBO() const
{
	return fbo;
}

GLuint Framebuffer::getTexture(size_t index) const
{
	return textures[index];
}

GLuint Framebuffer::getDepthTexture() const
{
	return depthTexture;
}

GLenum Framebuffer::checkStatus() const
{
	return glCheckNamedFramebufferStatus(fbo, GL_FRAMEBUFFER);
}

void Framebuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void Framebuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::attachTexture(GLenum attachment, GLenum sizedFormat, GLint width, GLint height)
{
	GLuint id = 0;
	glCreateTextures(GL_TEXTURE_2D, 1, &id);
	textures.push_back(id);
	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureStorage2D(id, 1, sizedFormat, width, height);
	glNamedFramebufferTexture(fbo, attachment, id, 0);
}

void Framebuffer::attachDepth(GLint width, GLint height)
{
	glDeleteTextures(1, &depthTexture);
	glCreateTextures(GL_TEXTURE_2D, 1, &depthTexture);
	glTextureParameteri(depthTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(depthTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureStorage2D(depthTexture, 1, GL_DEPTH_COMPONENT32F, width, height);
	glNamedFramebufferTexture(fbo, GL_DEPTH_ATTACHMENT, depthTexture, 0);
}

void Framebuffer::useTextures(GLuint startUnit)
{
	for (GLuint i = 0; i < textures.size(); ++i)
	{
		glBindTextureUnit(startUnit + i, textures[i]);
	}
}

void Framebuffer::useBuffer(GLenum startAttachment)
{
	std::vector<GLenum> attachments(textures.size());
	for (GLuint i = 0; i < textures.size(); ++i)
	{
		attachments[i] = startAttachment + i;
	}
	glNamedFramebufferDrawBuffers(fbo, static_cast<GLuint>(textures.size()), attachments.data());
}