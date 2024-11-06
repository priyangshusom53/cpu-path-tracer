#pragma once
#include"Dependencies.h"
#include"assimp/Importer.hpp"
#include"assimp/scene.h"
#include"assimp/postprocess.h"


namespace vista
{
	class BasicMesh
	{

	private:
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;
		std::vector<glm::uvec3> vertexIndex;
		/** Number of primitives in this mesh */
		unsigned int numFaces;
		/** Index in the Material array */
		unsigned int materialIndex;
		/** Number of vertices in vertex array of this mesh */
		unsigned int numVertices;

		unsigned int uvIndex;

		unsigned char* baseColorTex;

	public:
		BasicMesh(aiMesh* assimpMesh);
		void loadMesh(aiMesh* _mesh);
		void drawMesh();
		
	};
}

