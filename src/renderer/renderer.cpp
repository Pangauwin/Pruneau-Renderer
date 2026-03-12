#include "renderer.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <imgui.h>
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include "frame_buffer.h"

#include <algorithm>

#include "core/entity.h"

#include "core/components/model_renderer.h"

#include "core/application.h"

#include <ImGuizmo.h>

static Renderer::Renderer* _current_renderer = nullptr;

Renderer::Shader* Renderer::default_shader = nullptr;

Renderer::Renderer::Renderer(Platform::Window* _window) : 
	m_window(_window), m_frame_buffer(Framebuffer(0, 0)), m_camera_index(0)
{
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	io.IniFilename = "C:\\Dev\\Pruneau-Suite\\Pruneau-Renderer\\ressources\\imgui.ini"; // TODO : Remove this in the future

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

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	default_shader = new Shader(
		"C:\\Dev\\Pruneau-Suite\\Pruneau-Renderer\\ressources\\shaders\\vertex_shader.glsl",
		"C:\\Dev\\Pruneau-Suite\\Pruneau-Renderer\\ressources\\shaders\\fragment_shader.glsl"
	);;

	_current_renderer = this;

	// Credits to enemymouse : https://github.com/enemymouse
	// TODO : Make so that we can choose the theme in the preferences settings
	/*
#pragma region ImGuiRestyling
	style.Alpha = 1.0;
	//style.WindowFillAlphaDefault = 0.83;
	//style.ChildWindowRounding = 3;
	style.WindowRounding = 3;
	style.GrabRounding = 1;
	style.GrabMinSize = 20;
	style.FrameRounding = 3;


	style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.40f, 0.41f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	//style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 1.00f, 1.00f, 0.65f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.80f, 0.80f, 0.18f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.80f, 0.80f, 0.27f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.81f, 0.86f, 0.66f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.18f, 0.21f, 0.73f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.27f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.22f, 0.29f, 0.30f, 0.71f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.44f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	//style.Colors[ImGuiCol_ComboBg] = ImVec4(0.16f, 0.24f, 0.22f, 0.60f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 1.00f, 1.00f, 0.68f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.36f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.76f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.00f, 0.65f, 0.65f, 0.46f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.01f, 1.00f, 1.00f, 0.43f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.62f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.00f, 1.00f, 1.00f, 0.33f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.42f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
	//style.Colors[ImGuiCol_Column] = ImVec4(0.00f, 0.50f, 0.50f, 0.33f);
	//style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.00f, 0.50f, 0.50f, 0.47f);
	//style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.00f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	//style.Colors[ImGuiCol_CloseButton] = ImVec4(0.00f, 0.78f, 0.78f, 0.35f);
	//style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.00f, 0.78f, 0.78f, 0.47f);
	//style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.00f, 0.78f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 1.00f, 1.00f, 0.22f);
	//style.Colors[ImGuiCol_TooltipBg] = ImVec4(0.00f, 0.13f, 0.13f, 0.90f);
	//style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.04f, 0.10f, 0.09f, 0.51f);
#pragma endregion
	*/
}

Renderer::Renderer::~Renderer()
{
}

Renderer::Renderer* Renderer::Renderer::Get()
{
	return _current_renderer;
}

void Renderer::Renderer::RegisterModel(Core::ModelRenderer* _model)
{
	m_render_pool.push_back(_model);
}

void Renderer::Renderer::UnRegisterModel(Core::ModelRenderer* _model)
{
	m_render_pool.erase(find(m_render_pool.begin(), m_render_pool.end(), _model));
}

void Renderer::Renderer::RegisterCamera(Core::Camera* _camera)
{
	m_cameras.push_back(_camera);
}

void Renderer::Renderer::UnRegisterCamera(Core::Camera* _camera)
{
	m_cameras.erase(find(m_cameras.begin(), m_cameras.end(), _camera));
}

void Renderer::Renderer::PreRender()
{
	m_frame_buffer.Bind();

	glClearColor(0.00f, 0.00f, 0.70f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Renderer::PostRender()
{
	if (m_cameras.size() != 0) {

		if (m_camera_index >= m_cameras.size())
		{
			//TODO : Log error
		}

		else
		{
			Core::Camera* _current_camera = m_cameras[m_camera_index];

			//TODO : switch between perspective and orthographic view
			glm::mat4 perspective = *_current_camera->GetPerspective();
			glm::mat4 world_transform = _current_camera->GetOwner()->GetComponent<Core::Transform>()->GetWorldTransformMatrix();
			glm::mat4 view = glm::inverse(world_transform);

			Core::LogMessageDebug("-- NEW FRAME --");

			for (Core::ModelRenderer* _model_renderer : m_render_pool)
			{
				if (!_model_renderer->model) continue;

				/*glm::mat4 mat = _model_renderer->GetOwner()->GetComponent<Core::Transform>()->GetWorldTransformMatrix();
				Debug_DrawCubeToFramebuffer(mat,
					view,
					*perspective);*/ // TODO : clean test
				glm::mat4 _model = _model_renderer->GetOwner()->GetComponent<Core::Transform>()->GetWorldTransformMatrix();


				_model_renderer->model->Draw(view, _model, perspective);
			}

			//Core::LogMessage("GLError : " + std::to_string(glad_glGetError()));
			//TODO : Actually display it in a window (stats window)
		}
	}
	m_frame_buffer.UnBind();
}

void Renderer::Renderer::PreGUIRender()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void Renderer::Renderer::PostGUIRender()
{
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
