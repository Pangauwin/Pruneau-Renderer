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

	unsigned int GetID() const;

	void Bind(int _slot) const;

	TEXTURE_TYPE type;
private:
	unsigned int m_id = 0;

	int height = 0;
	int width = 0;
	int channel_nb = 0;
};

}