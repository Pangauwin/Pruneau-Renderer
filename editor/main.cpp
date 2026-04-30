#include "core/application.h"

int main()
{
	Core::AppParams app_params;
	app_params.window_params.width = 1080;
	app_params.window_params.height = 720;
	app_params.window_params.title = "Pruneau Renderer v1";

	Core::Application app(app_params);
	app.Run();

	return 0;
}