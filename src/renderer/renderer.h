#pragma once

#include "../platform/window.h"
#include "frame_buffer.h"

#include "shader.h"

#include "core/components/model_renderer.h"
#include "core/components/camera.h"

#include <vector>

namespace Core {
	class Application;
}


namespace Renderer {

struct RendererConfig {

};

class Renderer
{
friend class Core::Application;

// Class that can register themselves
friend class Core::ModelRenderer;
friend class Core::Camera;

public:
	Renderer(Platform::Window* _window);
	~Renderer();

	static Renderer* Get();

private:
	void RegisterModel(Core::ModelRenderer* _model);
	void UnRegisterModel(Core::ModelRenderer* _model);

	void RegisterCamera(Core::Camera* _camera);
	void UnRegisterCamera(Core::Camera* _camera);

private:
	void PreRender();
	void PostRender();
	
	void PreGUIRender();
	void PostGUIRender();

public:
	std::vector<Core::Camera*> m_cameras;
	unsigned int m_camera_index;
	Framebuffer m_frame_buffer;



private:
	Platform::Window* m_window;
	std::vector<Core::ModelRenderer*> m_render_pool;

};

extern Shader* default_shader;
}