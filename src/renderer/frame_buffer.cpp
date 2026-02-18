#include "frame_buffer.h"

#include <glad/glad.h>

#include "../core/application.h"

Renderer::Framebuffer::Framebuffer(uint32_t _width, uint32_t _height) : width(_width), height(_height)
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
    glViewport(0, 0, width, height);
}

void Renderer::Framebuffer::UnBind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, Core::Application::Get()->m_window->params->width, Core::Application::Get()->m_window->params->height);
}

void Renderer::Framebuffer::Resize(uint32_t _width, uint32_t _height)
{
    if (_width == 0 || _height == 0) return;
    width = _width;
    height = _height;

    Invalidate();
}

void Renderer::Framebuffer::Invalidate()
{
    if (width == 0 || height == 0)
        return;

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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

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
        width, height, 0,
        GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT,
        GL_TEXTURE_2D,
        m_depth_attachment,
        0
    );

    GLenum buffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, buffers);
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::string error_msg = "Framebuffer incomplete: ";

        switch (status)
        {
        case GL_FRAMEBUFFER_UNDEFINED:
            error_msg +=  "UNDEFINED"; break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            error_msg += "INCOMPLETE_ATTACHMENT"; break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            error_msg += "MISSING_ATTACHMENT"; break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            error_msg += "INCOMPLETE_DRAW_BUFFER"; break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            error_msg += "INCOMPLETE_READ_BUFFER"; break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            error_msg += "UNSUPPORTED"; break;
        default:
            error_msg = "UNKNOWN (" + status + (std::string)")";
        }

        Core::Application::Get()->LogMessage(error_msg);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
