#include "AImporter.h"

namespace vista
{
	AImporter::AImporter()
		:pScene(nullptr),
		numOfScenes(0),
		numOfBaseColorTexPerMaterial(0),
		windingOrderCW(false)
	{

	}

	bool AImporter::loadScene(std::string& path)
	{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		numOfVBO = 3;
		VBOs.resize(numOfVBO);
		
		bool ret = false;
		Assimp::Importer importer;
		pScene = importer.ReadFile(path, ASSIMP_LOAD_FLAGS);
		initBuffers();
		numOfMaterials = pScene->mNumMaterials;
		if (pScene != NULL)
		{
			//ret = initScene(pScene, path);
		}
		else
		{
			CORE_LOG_ERROR("Error parsing {0}: \n{1}", path.c_str(), importer.GetErrorString());
		}
		return ret;
	}

	void AImporter::initTextures(const aiScene* pScene)
	{
		if (pScene->HasMaterials())
		{
			for (int i = 0; i < numOfMaterials; ++i)
			{
				if (numOfBaseColorTexPerMaterial <=
					pScene->mMaterials[i]->GetTextureCount(aiTextureType_BASE_COLOR))
				{
					numOfBaseColorTexPerMaterial =
						pScene->mMaterials[i]->GetTextureCount(aiTextureType_BASE_COLOR);
				}
			}
		}
	}

	void AImporter::initBuffers()
	{
		glGenBuffers(numOfVBO, VBOs.data());
	}
}