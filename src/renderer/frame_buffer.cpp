#include "frame_buffer.h"

#include <glad/glad.h>

Renderer::Framebuffer::Framebuffer(uint32_t _width, uint32_t _height) : m_width(_width), m_height(_height)
{
	Invalidate();
}

Renderer::Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_renderer_id);
	glDeleteTextures(1, &m_color_attachment);
	glDeleteTextures(1, &m_depth_attachment);
}

void Renderer::Framebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);
    glViewport(0, 0, m_width, m_height);
}

void Renderer::Framebuffer::UnBind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::Framebuffer::Resize(uint32_t _width, uint32_t _height)
{
    if (_width == 0 || _height == 0) return;
    m_width = _width;
    m_height = _height;

    Invalidate();
}

void Renderer::Framebuffer::Invalidate()
{
	if (m_renderer_id)
	{
		glDeleteFramebuffers(1, &m_renderer_id);
		glDeleteTextures(1, &m_color_attachment);
		glDeleteTextures(1, &m_depth_attachment);
	}

	glGenFramebuffers(1, &m_renderer_id);
	glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);

    glGenTextures(1, &m_color_attachment);
    glBindTexture(GL_TEXTURE_2D, m_color_attachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        m_color_attachment,
        0
    );

    glGenTextures(1, &m_depth_attachment);
    glBindTexture(GL_TEXTURE_2D, m_depth_attachment);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8,
        m_width, m_height, 0,
        GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr
    );

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT,
        GL_TEXTURE_2D,
        m_depth_attachment,
        0
    );

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        // TODO : Log error
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
