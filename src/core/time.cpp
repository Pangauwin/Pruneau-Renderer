#include "core/time.h"

#include "application.h"

float Time::last_frame_time = 0.0;
float Time::delta_time = 0.0f;
float Time::frame_per_second = 0.0f;
float Time::time_since_start = 0.0f;
unsigned long Time::frame_count = 0;
float Time::readable_fps = 0.0f;

static float accumulator = 0.0f;
static int frame_counter = 0;

void Time::Init()
{
	last_frame_time = Core::Application::Get()->m_window->GetTime(); // TODO : Simplify (a lot)
}

void Time::Update()
{
	float time_since_start = Core::Application::Get()->m_window->GetTime();

	delta_time = (time_since_start - last_frame_time);
	last_frame_time = time_since_start;

	frame_count++;

	if (delta_time > 0.0f)
		frame_per_second = 1.0f / delta_time;

	accumulator += delta_time;
	frame_counter++;

	if (accumulator >= 0.5f)
	{
		readable_fps = frame_counter / accumulator;
		accumulator = 0.0f;
		frame_counter = 0;
	}
}
