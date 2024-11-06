#pragma once
#include"Dependencies.h"
#include"assimp/Importer.hpp"
#include"assimp/scene.h"
#include"assimp/postprocess.h"

namespace vista
{
	class BasicMaterial
	{
	private:
		unsigned char* baseColorTexture;

	public:
		unsigned int uvIndex;
		BasicMaterial(aiMaterial* _material);
	};
}

