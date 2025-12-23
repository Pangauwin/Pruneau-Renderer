#pragma once

#include "../platform/window.h"
#include "frame_buffer.h"

namespace Core {
	class Application;
}


namespace Renderer {

struct RendererConfig {

};

class Renderer
{
friend class Core::Application;

public:
	Renderer(Platform::Window* _window);
	~Renderer();

private:
	void PreRender();
	void PostRender();
	
	void PreGUIRender();
	void PostGUIRender();

private:
	Platform::Window* m_window;
	Framebuffer m_frame_buffer;

};

}