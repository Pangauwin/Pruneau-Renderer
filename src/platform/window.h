#pragma once

#include "../core/event/event.h"

struct GLFWwindow;

namespace Renderer {
	class Renderer;
}

namespace Platform {

struct WindowParams {
	unsigned int width;
	unsigned int height;
	const char* title;
};

class Window {

friend class Renderer::Renderer;

public:
	Window(WindowParams& _params);
	~Window();

	void SwapBuffers();
	void PollEvents();

private:
	GLFWwindow* m_glfw_window;
};

class WindowCloseEvent : public Core::Event
{
public:
	static Core::event_type GetStaticType() { return Core::event_type::event_type_window_close; }
	Core::event_type GetEventType() const override { return GetStaticType(); }
	const char* GetName() const override { return "WindowClose"; }
	int GetCategoryFlags() const override { return Core::event_category_application; }
};

}