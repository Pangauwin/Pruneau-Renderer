#include "renderer.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <imgui.h>
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include "frame_buffer.h"

Renderer::Renderer::Renderer(Platform::Window* _window) : m_window(_window), m_frame_buffer(Framebuffer(0, 0))
{
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	io.IniFilename = "imgui.ini"; // TODO : Remove this in the future

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Backend init
	ImGui_ImplGlfw_InitForOpenGL(m_window->m_glfw_window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	glEnable(GL_DEPTH_TEST);
}

Renderer::Renderer::~Renderer()
{
}

void Renderer::Renderer::PreRender()
{
	m_frame_buffer.Bind();

	glClearColor(0.f, 0.f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Renderer::PostRender()
{
	m_frame_buffer.UnBind();
}

void Renderer::Renderer::PreGUIRender()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Renderer::Renderer::PostGUIRender()
{
	ImGui::Begin("Scene");

	ImVec2 viewport_size = ImGui::GetContentRegionAvail();

	if (viewport_size.x > 0 && viewport_size.y > 0)
	{
		if ((uint32_t)viewport_size.x != m_frame_buffer.m_width || (uint32_t)viewport_size.y != m_frame_buffer.m_height) // TODO : Remove this and store width and height in the renderer
		{
			//TODO : Once width and height are in the renderer, save it here too
			m_frame_buffer.Resize((uint32_t)viewport_size.x, (uint32_t)viewport_size.y);
		}
		ImGui::Image(
			(void*)(intptr_t)m_frame_buffer.GetColorAttachmentRendererID(),
			viewport_size,
			ImVec2(0, 1),
			ImVec2(1, 0)
		);
	}

	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Multi-viewport support
	ImGuiIO& io = ImGui::GetIO();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup);
	}
}
