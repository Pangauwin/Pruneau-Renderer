#pragma once

class Time
{
public:
	static void Init();
	static void Update();

	static float last_frame_time;
	static float delta_time;
	static float frame_per_second;
	static float time_since_start;
	static unsigned long frame_count;
	static float readable_fps;

	// TODO : implement getters instead (protect variables)
};