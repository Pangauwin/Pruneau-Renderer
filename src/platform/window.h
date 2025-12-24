#pragma once

#include "../core/event/event.h"

struct GLFWwindow;

namespace Renderer {
	class Renderer;
	class Framebuffer;
}

namespace Platform {

struct WindowParams {
	unsigned int width;
	unsigned int height;
	const char* title;
};

class Window {

friend class Renderer::Renderer;
friend class Renderer::Framebuffer;

public:
	Window(WindowParams& _params);
	~Window();

	void SwapBuffers();
	void PollEvents();

public:
	WindowParams* params;

private:
	GLFWwindow* m_glfw_window;

};

}