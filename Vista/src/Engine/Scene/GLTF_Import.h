#pragma once
#include<iostream>
#include<string>
#include<vector>
#include"glad/glad.h"

#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"assimp/Importer.hpp"
#include"assimp/scene.h"
#include"assimp/postprocess.h"

#include"stb_image.h"

#include"Engine/Log.h"
#include"Engine/Renderer/OpenGLShader.h"
#include"PBR_Materials.h"
#include"Engine/Scene/Camera.h"

#define POS_LOCATION 0
#define NORMAL_LOCATION 1
#define TEX_COORD_0_LOCATION 2

namespace vista 
{
	enum BufferType
	{
		INDEX_BUFFER = 0,
		POSITION_VB = 1,
		NORMAL_VB = 2,
		TEX_COORDS_VB = 3
	};

	struct BasicTexture;
	struct BasicMaterial;
	struct BasicMesh;

	class GLTF_Importer
	{
	public:
		GLTF_Importer();
		void importScene(std::string& file);

		void importComputeDemoScene(std::string& file);
		void loadFromComputeDemoScene(const aiScene* pScene);
		void importComputeDemoMaterials(const aiScene* pScene);
		void storeComputeDemoVertexData(const aiScene* pScene);
		void loadComputeDemoShaders();
		void populateComputeDemoBuffers();
		void setupComputeQuad();
		void drawComputeDemo(GLFWwindow* window);

		void loadFromScene(const aiScene* pScene);
		void countNodes(aiNode* root, unsigned int& count);
		void storeNodes(aiNode* root);
		void allocateVertexAttributeVectors(const aiScene* pScene);
		void allocateIndexVector(const aiScene* pScene);
		void storeVertexData(const aiScene* pScene);
		void calculateWorldMat(aiNode* node, glm::mat4& wMat);
		void importMaterials(const aiScene* pScene);
		std::string textureAbsolutePath(std::string& relativePath);
		void loadTexture(BasicTexture& texture, bool diffuse, bool specular, bool normal);
		void loadShaders();
		void initOpenglTextures(BasicTexture& texture, bool diffuse, bool specular, bool normal);
		void populateBuffers();

		void draw(GLFWwindow* window);
	public:
		// lighting
		glm::vec3 lightPos;

		std::string filePath;
		std::vector<unsigned int> indexes;
		unsigned int numVertices;
		std::vector<float> pos;
		std::vector<float> normals;
		std::vector<float> texCoord_0;
		std::vector<unsigned int> numMeshesPerNode;
		std::vector<aiNode*> aiNodes;
		std::vector<BasicMaterial> m_Materials;
		std::vector<Material> m_PBR_Materials;
		std::vector<BasicMesh> m_Meshes;

		unsigned int VAO;
		openglShader* shader;
		std::vector<unsigned int> m_Buffers;
		unsigned int numBuffers;

		//rtx
		openglShader* quadShader;
		GLuint quadVAO, quadVBO;
		GLuint texture;
		float quadVertices[24] = {
			// positions // texCoords
			-1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f, 1.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,
			 1.0f, 1.0f, 1.0f, 1.0f
		};

		struct Scene
		{
			Scene()
			{
				cam = new Camera();
			}
			Camera* cam;
		};
		Scene* m_Scene;
	};

	struct BasicMesh
	{
		BasicMesh()
		{
			m_NumIndices = 0;
			m_BaseVertex = 0;
			m_BaseIndex = 0;
			m_MaterialIndex = -1;
			m_WorldMat = glm::mat4(1.0f);
		}
		unsigned int m_NumIndices;
		unsigned int m_BaseVertex;
		unsigned int m_BaseIndex;
		unsigned int m_MaterialIndex;
		glm::mat4 m_WorldMat;
	};

	struct BasicTexture
	{
		BasicTexture()
		{
			data = nullptr;
			width = 0;
			height = 0;
			numChannels = 0;
			texture_0 = 0;
			texture_1 = 0;
			texture_2 = 0;
		}
		void bindTex_0()
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_0);
		}
		void bindTex_1()
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture_1);
		}
		void bindTex_2()
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, texture_2);
		}
		std::string path;
		unsigned char* data;
		int width;
		int height;
		int numChannels;
		unsigned int texture_0;
		unsigned int texture_1;
		unsigned int texture_2;
	};

	struct BasicMaterial
	{
		BasicMaterial() 
		{
			hasTexture_BaseColor = false;
			hasTexture_Specular = false;
			hasTexture_Normal = false;
		}
		bool hasTexture_BaseColor;
		bool hasTexture_Specular;
		bool hasTexture_Normal;
		BasicTexture baseColorTex_0;
		BasicTexture specularTex_1;
		BasicTexture normalTex_2;
	};

}
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)