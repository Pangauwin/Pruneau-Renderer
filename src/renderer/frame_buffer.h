#pragma once

#include <cstdint>

namespace Renderer {

class Framebuffer {

	friend class Renderer; // TODO : remove this

public:
	Framebuffer(uint32_t _width, uint32_t _height);
	~Framebuffer();

	void Bind();
	void UnBind();

	void Resize(uint32_t _width, uint32_t _height);

	uint32_t GetColorAttachmentRendererID() const { return m_color_attachment; }

private:
	void Invalidate();

private:
	uint32_t m_renderer_id = 0;
	uint32_t m_color_attachment = 0;
	uint32_t m_depth_attachment = 0;
	
	uint32_t m_width = 0;
	uint32_t m_height = 0;
};

}