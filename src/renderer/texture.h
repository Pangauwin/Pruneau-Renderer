#pragma once

#include <string>

namespace Renderer {

enum TEXTURE_TYPE
{
	TEXTURE_TYPE_DIFFUSE,
	TEXTURE_TYPE_NORMAL_MAP
};

class Texture {

public:
	Texture(const char* _path);
	Texture(const char* _path, TEXTURE_TYPE _type);
	Texture(void* _data, int _width, int _height);

	~Texture();
	// TODO : implement texture class

	unsigned int GetID() const;


	TEXTURE_TYPE type = TEXTURE_TYPE_DIFFUSE;

	std::string path;

private:
	unsigned int m_id = 0;

	int height = 0;
	int width = 0;
	int channel_nb = 0;
};

}