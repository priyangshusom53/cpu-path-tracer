#include "Node.h"

namespace vista
{
	Node::Node(aiNode* _node, Node* _parent, int _nodeId)
	{
		node = _node;
		if (node->mName.length == 0)
		{
			std::string rOpen  = "(";
			std::string rClose = ")";
			nodeName = std::string("New_Node") + rOpen + std::to_string(_nodeId) + rClose;
		}
		else { nodeName = node->mName.C_Str(); }
		nodeId = _nodeId;
		parent = _parent;
		numMeshes = node->mNumMeshes;
		meshes.resize(numMeshes);
		transformation = glm::mat4(1.0f);
		globalTransform = glm::mat4(1.0f);
	}

	void Node::loadMeshes(aiMesh** _meshes)
	{
		for (int i = 0; i < numMeshes; i++)
		{
			BasicMesh* mesh = new BasicMesh(_meshes[node->mMeshes[i]]);
			mesh->loadMesh(_meshes[node->mMeshes[i]]);
			meshes[i] = mesh;
		}
	}

	void Node::loadMaterial(aiMaterial* material, unsigned int materialId)
	{

	}
}
