#include "Scene.h"

namespace vista
{
	Scene::Scene(std::string& path)
	{
		filePath = path;
		
		pScene = importer.ReadFile(path, ASSIMP_LOAD_FLAGS);
		sceneName = (pScene->mName).C_Str();
		aiMeshes = pScene->mMeshes;
		numBasicMatrials = pScene->mNumMaterials;
		unsigned int numNodes = 0;
		countNoOfNodes(numNodes, (pScene->mRootNode));
		nodes.resize(numNodes);

		storeNodes(pScene->mRootNode);

		cam = new Camera();
	}

	void Scene::countNoOfNodes(unsigned int& n, aiNode* root)
	{
		n += 1;
		int noOfChildren = root->mNumChildren;
		for (int i = 0; i < noOfChildren; i++)
		{
			countNoOfNodes(n, root->mChildren[i]);
		}
	}

	void Scene::storeNodes(aiNode* root)
	{
		int nodeId = 0;
		std::queue<std::pair<aiNode*,Node*>> assimpNodesQueue;
		mRoot = new Node(root, nullptr, nodeId);
		assimpNodesQueue.push(std::make_pair(root, mRoot));

		while (!assimpNodesQueue.empty())
		{
			aiNode* parent = assimpNodesQueue.front().first;
			Node* mParent = assimpNodesQueue.front().second;

			assimpNodesQueue.pop();
			
			for (int i = 0; i < parent->mNumChildren; i++)
			{
				++nodeId;
				Node* mNode = new Node(parent->mChildren[i], mParent, nodeId);
				nodes[mParent->nodeId].push_back(mNode);
				nodes[mNode->nodeId].push_back(mParent);
				assimpNodesQueue.push(std::make_pair(parent->mChildren[i], mNode));
			}
		}
	}

	void Scene::load()
	{
		mRoot->loadMeshes(aiMeshes);
		std::queue<int> nodeIdQueue;
		nodeIdQueue.push(mRoot->nodeId);

		while (!nodeIdQueue.empty())
		{
			int id = nodeIdQueue.front();
			nodeIdQueue.pop();
			for (auto i = nodes[id].begin(); i != nodes[id].end(); i++)
			{
				if ((*i)->nodeId > id)
				{
					nodeIdQueue.push((*i)->nodeId);
					(*i)->loadMeshes(aiMeshes);
				}
			}
		}
		for (int i = 0; i < pScene->mNumMaterials; i++)
		{
			BasicMaterial* material = new BasicMaterial(pScene->mMaterials[i]);
			
		}
		bool b = pScene->HasTextures();
		CORE_LOG_INFO("\nEMBEDDED TEXTURE {0}", b);
	}
	
}
