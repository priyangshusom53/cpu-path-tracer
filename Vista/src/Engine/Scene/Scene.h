#pragma once

#include<queue>
#include<memory>
#include<utility>
#include"Dependencies.h"
#include"Node.h"
#include"BasicMaterial.h"
#include"Camera.h"
#include"assimp/Importer.hpp"
#include"assimp/scene.h"
#include"assimp/postprocess.h"


namespace vista
{
	class Scene
	{
	private:
		std::vector<std::vector<Node*>> nodes;
		std::vector<BasicMaterial*> materials;
		int numBasicMatrials;
		Node* mRoot;
		Assimp::Importer importer;
		const aiScene* pScene;
		aiMesh** aiMeshes;
		std::string sceneName;
		std::string filePath;
		Camera* cam;
	public:
		Scene(std::string& path);
		void load();
		void draw();
		void countNoOfNodes(unsigned int& n, aiNode* root);
		void storeNodes(aiNode* root);
	};
}
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)