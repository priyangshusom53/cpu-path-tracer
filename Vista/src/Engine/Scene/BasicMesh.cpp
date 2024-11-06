#include "BasicMesh.h"

namespace vista
{
	BasicMesh::BasicMesh(aiMesh* _mesh)
	{
		numVertices = _mesh->mNumVertices;
		positions.resize(numVertices);
		normals.resize(numVertices);
		uvs.resize(numVertices);
		numFaces = _mesh->mNumFaces;
		vertexIndex.resize(numFaces);

		materialIndex = _mesh->mMaterialIndex;
		uvIndex = 999;
		baseColorTex = nullptr;

	}

	void BasicMesh::loadMesh(aiMesh* _mesh)
	{
		for (int i = 0; i < numVertices; i++)
		{
			positions[i].x = _mesh->mVertices[i].x;
			positions[i].y = _mesh->mVertices[i].y;
			positions[i].z = _mesh->mVertices[i].z;

		}
		if (_mesh->mNormals != nullptr)
		{
			for (int i = 0; i < numVertices; i++)
			{

				normals[i].x = _mesh->mNormals[i].x;
				normals[i].y = _mesh->mNormals[i].y;
				normals[i].z = _mesh->mNormals[i].z;

			}
		}

		
	}
}
