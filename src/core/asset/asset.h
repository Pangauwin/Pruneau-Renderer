#pragma once

#include <string>

namespace Core {

enum ASSET_TYPE
{
	ASSET_TYPE_MODEL,
	ASSET_TYPE_AUDIO
};

struct Asset
{
	ASSET_TYPE type;
	void* data; // Any kind of asset's data
	std::string name;
	std::string path;
};

}