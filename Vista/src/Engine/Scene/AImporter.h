#pragma once
#include"Dependencies.h"
#include"assimp/Importer.hpp"
#include"assimp/scene.h"
#include"assimp/postprocess.h"


namespace vista
{
	enum VertexAttributePointers
	{
		VERTEX_ATTRIBUTE_POS = 0,
		VERTEX_ATTRIBUTE_NORMALS = 1,
		VERTEX_ATTRIBUTE_UV0 = 2,
		VERTEX_ATTRIBUTE_UV1 = 3
	};
	class AImporter
	{
	public:
		AImporter();
		bool loadScene(std::string& path);

	private:
		//bool initScene(const aiScene* pScene, std::string& path);
		void initTextures(const aiScene* pScene);
		void initBuffers();
	private:
		const aiScene* pScene;
		unsigned int numOfScenes;
		unsigned int numOfMaterials;
		/* Max number of Diffuse/albedo texture of a material */
		unsigned int numOfDiffuseTexPerMaterial;
		/* Max number of Base color texture for pbrt */
		unsigned int numOfBaseColorTexPerMaterial;
		/* Vertex winding order Default:CCW  */
		bool windingOrderCW;

		//opengl
		unsigned int VAO;
		unsigned int numOfVBO;
		std::vector<unsigned int> VBOs;
		unsigned int EBO;
	};
}

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)


