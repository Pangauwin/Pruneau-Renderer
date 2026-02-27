#include "window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Windows.h> // Message boxes

#include "../core/application.h"
#include "core/event/events/file_drop.h"
#include "core/event/events/mouse_button.h"

#pragma region glfw_callbacks

static void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height); // TODO : Remove this ?

	Core::Application::Get()->m_window->params->height = height;
	Core::Application::Get()->m_window->params->width = width;
}

static void window_close_callback(GLFWwindow* window) {
	Core::Application::Get()->CloseApplication();
}

static void drop_file_callback(GLFWwindow* window, int count, const char** paths)
{
	std::vector<std::string> filepaths;
	filepaths.reserve(count);

	for (int i = 0; i < count; i++)
	{
		filepaths.emplace_back(paths[i]);
	}

	Core::FileDropEvent event(std::move(filepaths));

	Core::Application::Get()->OnEvent(event);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Core::MouseButtonEvent event(button, action, mods);
	Core::Application::Get()->OnEvent(event);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//TODO : implement
}

static void char_callback(GLFWwindow* window, unsigned int codepoint)
{
	//TODO : implement
}
#pragma endregion


Platform::Window::Window(WindowParams& _params) : params(new WindowParams()), m_glfw_window(NULL)
{
	params->height = _params.height;
	params->width = _params.width;
	params->title = _params.title;

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_glfw_window = glfwCreateWindow(params->width, params->height, params->title, NULL, NULL);

	if (m_glfw_window == NULL)
	{
		MessageBox(NULL, "Window Creation Failed !", NULL, MB_OK);
		glfwTerminate();

		window_close_callback(m_glfw_window);
	}

	glfwMakeContextCurrent(m_glfw_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		MessageBox(NULL, "Glad Loading Procedure Failed !", NULL, MB_OK);
		glfwTerminate();

		window_close_callback(m_glfw_window);
	}

	glViewport(0, 0, params->width, params->height);

	glfwSetFramebufferSizeCallback(m_glfw_window, frame_buffer_size_callback);
	glfwSetWindowCloseCallback(m_glfw_window, window_close_callback);
	glfwSetDropCallback(m_glfw_window, drop_file_callback);

	glfwSetMouseButtonCallback(m_glfw_window, mouse_button_callback);
	glfwSetKeyCallback(m_glfw_window, key_callback);
	glfwSetCharCallback(m_glfw_window, char_callback);

	glfwSwapInterval(1);
}

Platform::Window::~Window()
{
	glfwTerminate();
}

void Platform::Window::SwapBuffers()
{
	glfwSwapBuffers(m_glfw_window);
}

void Platform::Window::PollEvents()
{
	glfwPollEvents();
}

float Platform::Window::GetTime()
{
	return static_cast<float>(glfwGetTime());
}