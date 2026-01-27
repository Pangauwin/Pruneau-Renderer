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

static Renderer::Renderer* _current_renderer = nullptr;
static bool _camera_binded = true;

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

	_current_renderer = this;
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

	glClearColor(0.f, 0.f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
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
			glm::mat4* perspective = _current_camera->GetPerspective();
			glm::mat4* world_transform = &_current_camera->GetOwner()->GetComponent<Core::Transform>()->GetWorldTransformMatrix();
			glm::mat4 view = glm::inverse(*world_transform);

			for (Core::ModelRenderer* _model_renderer : m_render_pool)
			{
				Shader* _shader = _model_renderer->model->shader;
				glm::mat4* _model = &_model_renderer->GetOwner()->GetComponent<Core::Transform>()->GetWorldTransformMatrix();

				_shader->Use();
				_shader->SetMat4("view", view);
				_shader->SetMat4("perspective", *_model);
				_shader->SetMat4("model", view);
				_shader->SetFloat("time", 0); // TODO : implement time module

				_model_renderer->model->Draw();
			}
		}

		// TODO : check what is faster between that and just having _camera_binded = true, each frame
		if(!_camera_binded)
			_camera_binded = true;
	}

	else
	{
		_camera_binded = false;
	}

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

	if (!_camera_binded)
		ImGui::Text("INFO : No camera is rendering");

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
