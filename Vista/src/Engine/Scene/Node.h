#pragma once

#include"Dependencies.h"
#include"BasicMesh.h"
#include"assimp/Importer.hpp"
#include"assimp/scene.h"
#include"assimp/postprocess.h"


namespace vista
{
	class Node
	{
	private:
		aiNode* node;
		std::string nodeName;
		Node* parent;
		unsigned int numMeshes;
		glm::mat4 transformation;
		glm::mat4 globalTransform;
	public:
		int nodeId;
		Node(aiNode* assimpNode, Node* _parent, int _id);
		void loadMeshes(aiMesh** _meshes);
		void loadMaterial(aiMaterial* material, unsigned int _materialId);
		void calculateGlobalTransform();
		void drawNode();
	};
}

