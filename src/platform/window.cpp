#include "window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Windows.h> // Message boxes

#include "../core/application.h"

#pragma region glfw_callbacks

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void window_close_callback(GLFWwindow* window) {
	Core::Application::Get()->CloseApplication();
}

#pragma endregion


Platform::Window::Window(WindowParams& _params)
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_glfw_window = glfwCreateWindow(_params.width, _params.height, _params.title, NULL, NULL);

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

	glViewport(0, 0, _params.width, _params.height);

	glfwSetFramebufferSizeCallback(m_glfw_window, frame_buffer_size_callback);
	glfwSetWindowCloseCallback(m_glfw_window, window_close_callback);

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
