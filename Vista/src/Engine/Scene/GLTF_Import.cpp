#include"GLTF_Import.h"

namespace vista
{
	GLTF_Importer::GLTF_Importer()
		:VAO(0),
		numVertices(0),
		numBuffers(0),
		shader(nullptr),
		quadShader(nullptr)
	{
		lightPos = glm::vec3(1.2f, 2.0f, 2.0f);
		m_Scene = new Scene();
	}

	void GLTF_Importer::importScene(std::string& file)
	{
		filePath = file;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		numBuffers = 4;
		m_Buffers.resize(numBuffers);
		glGenBuffers(numBuffers, m_Buffers.data());

		Assimp::Importer importer;

		const aiScene* pScene = importer.ReadFile(file, ASSIMP_LOAD_FLAGS);

		if (pScene)
		{
			loadFromScene(pScene);
		}
		else {
			CORE_LOG_ERROR("\nFailed to load gltf file: \n{0}\n{1}", file, importer.GetErrorString());
		}
		loadShaders();
		populateBuffers();
		glBindVertexArray(0);
	}

	void GLTF_Importer::importComputeDemoScene(std::string& file)
	{
		filePath = file;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		numBuffers = 3;
		m_Buffers.resize(numBuffers);
		glGenBuffers(numBuffers, m_Buffers.data());
		Assimp::Importer importer;

		const aiScene* pScene = importer.ReadFile(file, ASSIMP_LOAD_FLAGS);
		if (pScene)
		{
			loadFromComputeDemoScene(pScene);
		}
		else {
			CORE_LOG_ERROR("\nFailed to load gltf file: \n{0}\n{1}", file, importer.GetErrorString());
		}
		loadComputeDemoShaders();
		populateComputeDemoBuffers();
		glBindVertexArray(0);
	}

	void GLTF_Importer::loadFromComputeDemoScene(const aiScene* pScene)
	{
		unsigned int numNodes = 0;
		m_Meshes.reserve(pScene->mNumMeshes);
		countNodes(pScene->mRootNode, numNodes);
		numMeshesPerNode.resize(numNodes);
		//aiNodes.resize(numNodes);
		storeNodes(pScene->mRootNode);

		for (unsigned int i = 0; i < numNodes; i++) {
			numMeshesPerNode[i] = (aiNodes[i]->mNumMeshes);
		}
		allocateVertexAttributeVectors(pScene);
		allocateIndexVector(pScene);
		m_PBR_Materials.reserve(pScene->mNumMaterials);
		importComputeDemoMaterials(pScene);
		storeComputeDemoVertexData(pScene);
	}

	void GLTF_Importer::importComputeDemoMaterials(const aiScene* pScene)
	{
		aiMaterial* aMaterial;
		for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
			aMaterial = pScene->mMaterials[i];
			Material myMaterial;
			myMaterial.name = aMaterial->GetName().C_Str();

			aiColor4D baseColor;
			if (AI_SUCCESS == aiGetMaterialColor(aMaterial, AI_MATKEY_BASE_COLOR, &baseColor)) {
				myMaterial.baseColorFactor = glm::dvec4(baseColor.r, baseColor.g, baseColor.b, baseColor.a);
			}
			float roughness, metallic;
			if (AI_SUCCESS == aiGetMaterialFloat(aMaterial, AI_MATKEY_METALLIC_FACTOR, &metallic)) {
				myMaterial.metallicFactor = metallic;
			}
			if (AI_SUCCESS == aiGetMaterialFloat(aMaterial, AI_MATKEY_ROUGHNESS_FACTOR, &roughness)) {
				myMaterial.roughnessFactor = roughness;
			}

			//specular factors
			if (myMaterial.name == "Material")
			{
				myMaterial.KHR_materials_specular = glm::dvec3(2.072167571642637,
					2.072167571642637,
					2.072167571642637);
			}
			if (myMaterial.name == "Material.002")
			{
				myMaterial.KHR_materials_specular = glm::dvec3(2.371357614441398,
					2.371357614441398,
					2.371357614441398);
			}
			if (myMaterial.name == "Material.003")
			{
			
			}
			if (myMaterial.name == "metal_sphere")
			{

			}
			if (myMaterial.name == "Material.005")
			{
				myMaterial.KHR_materials_specular = glm::dvec3(0.8089210540725896,
					0.8089210540725896,
					0.8089210540725896);
			}
			if (myMaterial.name == "Material.009")
			{

			}
			if (myMaterial.name == "Material.008")
			{

			}
			if (myMaterial.name == "Material.010")
			{
				myMaterial.KHR_materials_specular = glm::dvec3(2.2772561980856847,
					2.2772561980856847,
					2.2772561980856847);
			}
			if (myMaterial.name == "Material.007")
			{
				
			}
			if (myMaterial.name == "Material.006")
			{

			}

			float ior;
			if (AI_SUCCESS == aiGetMaterialFloat(aMaterial, AI_MATKEY_REFRACTI, &ior)) {
				myMaterial.KHR_materials_ior = ior;
			}

			float transmission;
			if (AI_SUCCESS == aiGetMaterialFloat(aMaterial, AI_MATKEY_TRANSMISSION_FACTOR, &transmission)) {
				myMaterial.KHR_materials_transmission = transmission;
			}

			aiColor4D emissive;
			if (AI_SUCCESS == aiGetMaterialColor(aMaterial, AI_MATKEY_COLOR_EMISSIVE, &emissive)) {
				myMaterial.emissiveFactor = glm::dvec3(emissive.r, emissive.g, emissive.b);
				if(myMaterial.emissiveFactor.r == 1 && myMaterial.emissiveFactor.g == 1 &&
					myMaterial.emissiveFactor.b == 1)
					myMaterial.isEmmisive = true;
			}

			
			if (myMaterial.isEmmisive) {
				if(myMaterial.name == "Material.006")
					myMaterial.KHR_materials_emissive_strength = 5.999999523162842;
				if(myMaterial.name == "Material.007")
					myMaterial.KHR_materials_emissive_strength = 9;
			}

			std::cout << "\n" << myMaterial.name;
			std::cout <<":  " << myMaterial.isEmmisive<<"  "<<myMaterial.emissiveFactor.r
				<<"  "<< myMaterial.emissiveFactor.g<<"  "<< myMaterial.emissiveFactor.b;
			m_PBR_Materials.push_back(myMaterial);
		}
	}

	void GLTF_Importer::storeComputeDemoVertexData(const aiScene* pScene)
	{
		unsigned int baseIndex = 0;
		unsigned int baseVertex = 0;
		glm::mat4 worldMatrix = glm::mat4(1.0f);
		for (int i = 0; i < aiNodes.size(); i++)
		{
			if (numMeshesPerNode[i] > 0)
			{
				calculateWorldMat(aiNodes[i], worldMatrix);
				for (unsigned int j = 0; j < aiNodes[i]->mNumMeshes; j++)
				{
					aiMesh* aMesh = pScene->mMeshes[aiNodes[i]->mMeshes[j]];

					BasicMesh mMesh;
					mMesh.m_NumIndices = 3 * (aMesh->mNumFaces);
					mMesh.m_BaseIndex = baseIndex;
					mMesh.m_BaseVertex = baseVertex;
					mMesh.m_MaterialIndex = aMesh->mMaterialIndex;
					mMesh.m_WorldMat = worldMatrix;
					m_Meshes.push_back(mMesh);

					for (unsigned int v = 0; v < aMesh->mNumVertices; v++)
					{
						if (((3 * baseVertex) + (3 * v)) > pos.size())
							CORE_LOG_ERROR("\nPOSITION VECTOR INDEX OUT OF BOUND");
						//pos[((3 * baseVertex) + (3 * v))] = (aMesh->mVertices[v].x);
						pos.push_back(aMesh->mVertices[v].x);
						if ((((3 * baseVertex) + (3 * v)) + 1) > pos.size())
							CORE_LOG_ERROR("\nPOSITION VECTOR INDEX OUT OF BOUND");
						//pos[((3 * baseVertex) + (3 * v)) + 1] = (aMesh->mVertices[v].y);
						pos.push_back(aMesh->mVertices[v].y);
						if ((((3 * baseVertex) + (3 * v)) + 2) > pos.size())
							CORE_LOG_ERROR("\nPOSITION VECTOR INDEX OUT OF BOUND");
						//pos[((3 * baseVertex) + (3 * v)) + 2] = (aMesh->mVertices[v].z);
						pos.push_back(aMesh->mVertices[v].z);

						if (aMesh->HasNormals())
						{
							if (((3 * baseVertex) + (3 * v)) > pos.size())
								CORE_LOG_ERROR("\NORMALS VECTOR INDEX OUT OF BOUND");
							//normals[((3 * baseVertex) + (3 * v))] = (aMesh->mNormals[v].x);
							normals.push_back(aMesh->mNormals[v].x);
							if (((3 * baseVertex) + (3 * v) + 1) > pos.size())
								CORE_LOG_ERROR("\nNORMALS VECTOR INDEX OUT OF BOUND");
							//normals[((3 * baseVertex) + (3 * v)) + 1] = (aMesh->mNormals[v].y);
							normals.push_back(aMesh->mNormals[v].y);
							if (((3 * baseVertex) + (3 * v) + 2) > pos.size())
								CORE_LOG_ERROR("\nNORMALS VECTOR INDEX OUT OF BOUND");
							//normals[((3 * baseVertex) + (3 * v)) + 2] = (aMesh->mNormals[v].z);
							normals.push_back(aMesh->mNormals[v].z);
						}
						if (0)
						{
							if ((((2 * baseVertex) + (2 * v)) + 0) > texCoord_0.size())
								CORE_LOG_ERROR("\nTEXTURE VECTOR INDEX OUT OF BOUND");
							//texCoord_0[((2 * baseVertex) + (2 * v))] = (aMesh->mTextureCoords[0][v].x);
							texCoord_0.push_back(aMesh->mTextureCoords[0][v].x);
							if ((((2 * baseVertex) + (2 * v)) + 1) > texCoord_0.size())
								CORE_LOG_ERROR("\nTEXTURE VECTOR INDEX OUT OF BOUND");
							//texCoord_0[((2 * baseVertex) + (2 * v)) + 1] = (aMesh->mTextureCoords[0][v].y);
							texCoord_0.push_back(aMesh->mTextureCoords[0][v].y);
						}
					}
					for (int v_i = 0; v_i < (aMesh->mNumFaces); v_i++) {
						if ((((3 * baseIndex) + (3 * v_i)) + 0) > indexes.size())
							CORE_LOG_ERROR("\nINDEX VECTOR INDEX OUT OF BOUND");
						//indexes[((3 * baseIndex) + (3 * v_i))] = aMesh->mFaces[v_i].mIndices[0];
						indexes.push_back(aMesh->mFaces[v_i].mIndices[0]);
						if ((((3 * baseIndex) + (3 * v_i)) + 1) > indexes.size())
							CORE_LOG_ERROR("\nINDEX VECTOR INDEX OUT OF BOUND");
						//indexes[((3 * baseIndex) + (3 * v_i) + 1)] = aMesh->mFaces[v_i].mIndices[1];
						indexes.push_back(aMesh->mFaces[v_i].mIndices[1]);
						if ((((3 * baseIndex) + (3 * v_i)) + 2) > indexes.size())
							CORE_LOG_ERROR("\nINDEX VECTOR INDEX OUT OF BOUND");
						//indexes[((3 * baseIndex) + (3 * v_i) + 2)] = aMesh->mFaces[v_i].mIndices[2];
						indexes.push_back(aMesh->mFaces[v_i].mIndices[2]);
					}

					baseIndex += (aMesh->mNumFaces);
					baseVertex += (aMesh->mNumVertices);
				}
			}
		}
	}

	void GLTF_Importer::loadComputeDemoShaders()
	{
		shader = new openglShader(
			VISTA_SHADER_NAME("demoScene.vert"),
			VISTA_SHADER_NAME("demoScene.frag"));
		shader->use();
	}

	void GLTF_Importer::populateComputeDemoBuffers()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POSITION_VB]);
		glBufferData(GL_ARRAY_BUFFER, (sizeof(pos[0]) * pos.size()), &pos[0], GL_STATIC_DRAW);
		glVertexAttribPointer(POS_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(POS_LOCATION);

		glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
		glBufferData(GL_ARRAY_BUFFER, (sizeof(normals[0]) * normals.size()), &normals[0], GL_STATIC_DRAW);
		glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(NORMAL_LOCATION);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (sizeof(indexes[0]) * indexes.size()), &indexes[0], GL_STATIC_DRAW);
	}
	void GLTF_Importer::setupComputeQuad()
	{
		glGenVertexArrays(1, &quadVAO);
		glBindVertexArray(quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
			(void*)0);
		glEnableVertexAttribArray(0);
	}
	void GLTF_Importer::drawComputeDemo(GLFWwindow* window)
	{
		// Check if "Ctrl+R" is pressed for switching to ray tracing mode
		bool ctrlPressed = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;
		bool rPressed = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;
		static bool useRayTracing = false; // Toggle state
		if (ctrlPressed && rPressed) {
			useRayTracing = !useRayTracing; // Toggle ray tracing mode
		}

		if (useRayTracing) 
		{
			glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

		}
		else
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			shader->use();
			m_Scene->cam->processInput(window);
			glBindVertexArray(VAO);

			shader->use();
			shader->setVec3("light.position", lightPos);
			shader->setVec3("viewPos", m_Scene->cam->cameraPos);

			// light properties
			shader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
			/*shader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);*/
			shader->setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);

			// material properties
			/*shader->setFloat("material.shininess", 64.0f);*/

			for (int i = 0; i < m_Meshes.size(); i++)
			{
				
				unsigned int u_model = glGetUniformLocation(shader->shaderProgramID, "model");
				unsigned int u_view = glGetUniformLocation(shader->shaderProgramID, "view");
				unsigned int u_projection = glGetUniformLocation(shader->shaderProgramID, "projection");
				glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(m_Meshes[i].m_WorldMat));
				glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(m_Scene->cam->getViewMat()));
				glUniformMatrix4fv(u_projection, 1, GL_FALSE, glm::value_ptr(m_Scene->cam->getProjMat()));

				unsigned int materialIndex = m_Meshes[i].m_MaterialIndex;
				shader->setVec4("objectColor", m_PBR_Materials[materialIndex].baseColorFactor);
				/*
				if (m_Materials[materialIndex].hasTexture_BaseColor) {
					m_Materials[materialIndex].baseColorTex_0.bindTex_0();
					glUniform1i(glGetUniformLocation(shader->shaderProgramID, "material.diffuse"), 0);
				}
				if (m_Materials[materialIndex].hasTexture_Specular) {
					m_Materials[materialIndex].specularTex_1.bindTex_1();
					glUniform1i(glGetUniformLocation(shader->shaderProgramID, "material.specular"), 1);
				}*/
				/*if (false && m_Materials[materialIndex].hasTexture_Normal) {
					m_Materials[materialIndex].normalTex_2.bindTex_2();
					glUniform1i(glGetUniformLocation(shader->shaderProgramID, "normal_2"), 2);
				}*/
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL),
					glDrawElementsBaseVertex(
						GL_TRIANGLES,
						m_Meshes[i].m_NumIndices,
						GL_UNSIGNED_INT,
						(void*)(sizeof(unsigned int) * 3 * m_Meshes[i].m_BaseIndex),
						m_Meshes[i].m_BaseVertex
					);
			}
			glBindVertexArray(0);
		}
	}

	void GLTF_Importer::loadFromScene(const aiScene* pScene)
	{
		unsigned int numNodes = 0; 
		m_Meshes.reserve(pScene->mNumMeshes);
		countNodes(pScene->mRootNode, numNodes);
		numMeshesPerNode.resize(numNodes);
		//aiNodes.resize(numNodes);
		storeNodes(pScene->mRootNode);

		for (unsigned int i = 0; i < numNodes; i++) {
			numMeshesPerNode[i] = (aiNodes[i]->mNumMeshes);
		}
		allocateVertexAttributeVectors(pScene);
		allocateIndexVector(pScene);
		m_Materials.resize(pScene->mNumMaterials);
		importMaterials(pScene);
		storeVertexData(pScene);
	}

	void GLTF_Importer::countNodes(aiNode* root, unsigned int& count)
	{
		count = count + 1;

		for (unsigned int i = 0; i < root->mNumChildren; i++)
		{
			countNodes(root->mChildren[i], count);
		}
	}
	void GLTF_Importer::storeNodes(aiNode* root)
	{
		aiNodes.push_back(root);
		for (unsigned int i = 0; i < root->mNumChildren; i++) {
			storeNodes(root->mChildren[i]);
		}
	}
	void GLTF_Importer::allocateVertexAttributeVectors(const aiScene* pScene)
	{
		for (unsigned int i = 0; i < pScene->mNumMeshes; i++) {
			numVertices += pScene->mMeshes[i]->mNumVertices;
		}
		unsigned int numPos = 3 * numVertices;
		unsigned int numNormals = 3 * numVertices;
		unsigned int numUVs = 2 * numVertices;
		pos.reserve(numPos);
		normals.reserve(numNormals);
		texCoord_0.reserve(numUVs);
	}

	void GLTF_Importer::allocateIndexVector(const aiScene* pScene)
	{
		unsigned int numIndices = 0;
		for (unsigned int i = 0; i < pScene->mNumMeshes; i++) {
			numIndices += 3 * (pScene->mMeshes[i]->mNumFaces);
		}
		indexes.reserve(numIndices);
	}

	void GLTF_Importer::storeVertexData(const aiScene* pScene)
	{
		unsigned int baseIndex = 0;
		unsigned int baseVertex = 0;
		glm::mat4 worldMatrix = glm::mat4(1.0f);
		for (int i = 0; i < aiNodes.size(); i++) 
		{
			if (numMeshesPerNode[i] > 0)
			{
				calculateWorldMat(aiNodes[i], worldMatrix);
				for (unsigned int j = 0; j < aiNodes[i]->mNumMeshes; j++)
				{
					aiMesh* aMesh = pScene->mMeshes[aiNodes[i]->mMeshes[j]];
					
					BasicMesh mMesh;
					mMesh.m_NumIndices = 3 * (aMesh->mNumFaces);
					mMesh.m_BaseIndex = baseIndex;
					mMesh.m_BaseVertex = baseVertex;
					mMesh.m_MaterialIndex = aMesh->mMaterialIndex;
					mMesh.m_WorldMat = worldMatrix;
					m_Meshes.push_back(mMesh);

					for (unsigned int v = 0; v < aMesh->mNumVertices; v++)
					{
						if (((3 * baseVertex) + (3 * v)) > pos.size())
							CORE_LOG_ERROR("\nPOSITION VECTOR INDEX OUT OF BOUND");
						//pos[((3 * baseVertex) + (3 * v))] = (aMesh->mVertices[v].x);
						pos.push_back(aMesh->mVertices[v].x);
						if ((((3 * baseVertex) + (3 * v)) + 1) > pos.size())
							CORE_LOG_ERROR("\nPOSITION VECTOR INDEX OUT OF BOUND");
						//pos[((3 * baseVertex) + (3 * v)) + 1] = (aMesh->mVertices[v].y);
						pos.push_back(aMesh->mVertices[v].y);
						if ((((3 * baseVertex) + (3 * v)) + 2) > pos.size())
							CORE_LOG_ERROR("\nPOSITION VECTOR INDEX OUT OF BOUND");
						//pos[((3 * baseVertex) + (3 * v)) + 2] = (aMesh->mVertices[v].z);
						pos.push_back(aMesh->mVertices[v].z);

						if (aMesh->HasNormals())
						{
							if (((3 * baseVertex) + (3 * v)) > pos.size())
								CORE_LOG_ERROR("\NORMALS VECTOR INDEX OUT OF BOUND");
							//normals[((3 * baseVertex) + (3 * v))] = (aMesh->mNormals[v].x);
							normals.push_back(aMesh->mNormals[v].x);
							if (((3 * baseVertex) + (3 * v) + 1) > pos.size())
								CORE_LOG_ERROR("\nNORMALS VECTOR INDEX OUT OF BOUND");
							//normals[((3 * baseVertex) + (3 * v)) + 1] = (aMesh->mNormals[v].y);
							normals.push_back(aMesh->mNormals[v].y);
							if (((3 * baseVertex) + (3 * v) + 2) > pos.size())
								CORE_LOG_ERROR("\nNORMALS VECTOR INDEX OUT OF BOUND");
							//normals[((3 * baseVertex) + (3 * v)) + 2] = (aMesh->mNormals[v].z);
							normals.push_back(aMesh->mNormals[v].z);
						}
						if (aMesh->HasTextureCoords(0))
						{
							if ((((2 * baseVertex) + (2 * v)) + 0) > texCoord_0.size())
								CORE_LOG_ERROR("\nTEXTURE VECTOR INDEX OUT OF BOUND");
							//texCoord_0[((2 * baseVertex) + (2 * v))] = (aMesh->mTextureCoords[0][v].x);
							texCoord_0.push_back(aMesh->mTextureCoords[0][v].x);
							if ((((2 * baseVertex) + (2 * v)) + 1) > texCoord_0.size())
								CORE_LOG_ERROR("\nTEXTURE VECTOR INDEX OUT OF BOUND");
							//texCoord_0[((2 * baseVertex) + (2 * v)) + 1] = (aMesh->mTextureCoords[0][v].y);
							texCoord_0.push_back(aMesh->mTextureCoords[0][v].y);
						}
					}
					for (int v_i = 0; v_i < (aMesh->mNumFaces); v_i++) {
						if ((((3 * baseIndex) + (3 * v_i)) + 0) > indexes.size())
							CORE_LOG_ERROR("\nINDEX VECTOR INDEX OUT OF BOUND");
						//indexes[((3 * baseIndex) + (3 * v_i))] = aMesh->mFaces[v_i].mIndices[0];
						indexes.push_back(aMesh->mFaces[v_i].mIndices[0]);
						if ((((3 * baseIndex) + (3 * v_i)) + 1) > indexes.size())
							CORE_LOG_ERROR("\nINDEX VECTOR INDEX OUT OF BOUND");
						//indexes[((3 * baseIndex) + (3 * v_i) + 1)] = aMesh->mFaces[v_i].mIndices[1];
						indexes.push_back(aMesh->mFaces[v_i].mIndices[1]);
						if ((((3 * baseIndex) + (3 * v_i)) + 2) > indexes.size())
							CORE_LOG_ERROR("\nINDEX VECTOR INDEX OUT OF BOUND");
						//indexes[((3 * baseIndex) + (3 * v_i) + 2)] = aMesh->mFaces[v_i].mIndices[2];
						indexes.push_back(aMesh->mFaces[v_i].mIndices[2]);
					}

					baseIndex += (aMesh->mNumFaces);
					baseVertex += (aMesh->mNumVertices);
				}
			}
		}
	}

	void GLTF_Importer::calculateWorldMat(aiNode* node, glm::mat4& wMat)
	{
		aiMatrix4x4 ai_wMat;
		while (node != nullptr)
		{
			ai_wMat = node->mTransformation * ai_wMat;
			node = node->mParent;
		}
		wMat[0][0] = ai_wMat.a1; wMat[1][0] = ai_wMat.a2; wMat[2][0] = ai_wMat.a3; wMat[3][0] = ai_wMat.a4;
		wMat[0][1] = ai_wMat.b1; wMat[1][1] = ai_wMat.b2; wMat[2][1] = ai_wMat.b3; wMat[3][1] = ai_wMat.b4;
		wMat[0][2] = ai_wMat.c1; wMat[1][2] = ai_wMat.c2; wMat[2][2] = ai_wMat.c3; wMat[3][2] = ai_wMat.c4;
		wMat[0][3] = ai_wMat.d1; wMat[1][3] = ai_wMat.d2; wMat[2][3] = ai_wMat.d3; wMat[3][3] = ai_wMat.d4;
	}

	void GLTF_Importer::importMaterials(const aiScene* pScene)
	{
		
		aiMaterial* aMaterial;
		for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
			aMaterial = pScene->mMaterials[i];
			BasicMaterial mMaterial;
			if (aMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				unsigned int diffuse_uvindex;
				aiString path;
				if (aMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, &diffuse_uvindex, NULL, NULL, NULL) == AI_SUCCESS) {
					mMaterial.hasTexture_BaseColor = true;
					mMaterial.baseColorTex_0.path = path.C_Str();
					mMaterial.baseColorTex_0.path = textureAbsolutePath(mMaterial.baseColorTex_0.path);
					loadTexture(mMaterial.baseColorTex_0, mMaterial.hasTexture_BaseColor, false, false);
					CORE_LOG_INFO("\ntexture path: {0}", mMaterial.baseColorTex_0.path);
					CORE_LOG_INFO("\ndiffuse_uvindex: {0}", diffuse_uvindex);
				}
			}
			if (aMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0)
			{
				aiString path;
				unsigned int specular_uvindex;
				if (aMaterial->GetTexture(aiTextureType_SPECULAR, 0, &path, NULL, &specular_uvindex, NULL, NULL, NULL) == AI_SUCCESS) {
					mMaterial.hasTexture_Specular = true;
					mMaterial.specularTex_1.path = path.C_Str();
					mMaterial.specularTex_1.path = textureAbsolutePath(mMaterial.specularTex_1.path);
					loadTexture(mMaterial.specularTex_1,false, mMaterial.hasTexture_Specular,false);
					CORE_LOG_INFO("\ntexture path: {0}", mMaterial.specularTex_1.path);
					CORE_LOG_INFO("\nspecular_uvindex: {0}", specular_uvindex);
				}
			}
			if (aMaterial->GetTextureCount(aiTextureType_NORMALS) > 0)
			{
				unsigned int normal_uvindex;
				aiString path;
				if (aMaterial->GetTexture(aiTextureType_NORMALS, 0, &path, NULL, &normal_uvindex, NULL, NULL, NULL) == AI_SUCCESS) {
					mMaterial.hasTexture_Normal = true;
					mMaterial.normalTex_2.path = path.C_Str();
					mMaterial.normalTex_2.path = textureAbsolutePath(mMaterial.normalTex_2.path);
					loadTexture(mMaterial.normalTex_2,false,false,mMaterial.hasTexture_Normal);
					CORE_LOG_INFO("\ntexture path: {0}", mMaterial.normalTex_2.path);
					CORE_LOG_INFO("\nnormal_uvindex: {0}", normal_uvindex);
				}
			}
			m_Materials[i] = mMaterial;
		}
	}

	std::string GLTF_Importer::textureAbsolutePath(std::string& relativePath)
	{
		std::string tmpStr = filePath;
		int strFindIndex = 0;
		strFindIndex = filePath.find_last_of('\\');
		filePath = filePath.replace((strFindIndex + 1), 200, std::string(""));

		strFindIndex = relativePath.find("/");
		if (strFindIndex != relativePath.npos) {
			relativePath = relativePath.replace(strFindIndex, 1, std::string("\\"));
		}
		std::string absPath = filePath + relativePath;
		filePath = tmpStr;
		return absPath;
	}

	void GLTF_Importer::loadTexture(BasicTexture& texture, bool diffuse, bool specular, bool normal)
	{
		int width = 0, height = 0, numChannels = 0;
		//stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load((texture.path.c_str()), &width, &height,
			&numChannels, 0);
		if (data) {
			texture.data = data;
			texture.width = width;
			texture.height = height;
			texture.numChannels = numChannels;
			initOpenglTextures(texture, diffuse, specular, normal);
			stbi_image_free(data);
		}
		else {
			CORE_LOG_ERROR("\nFailed to load texture: {0}", texture.path);
		}
	}

	void GLTF_Importer::loadShaders()
	{
		shader = new openglShader(
			VISTA_SHADER_NAME("scene.vert"),
			VISTA_SHADER_NAME("scene.frag"));
		shader->use();
	}

	void GLTF_Importer::initOpenglTextures(BasicTexture& texture, bool diffuse, bool specular, bool normal)
	{
		if (diffuse)
		{
			glGenTextures(1, &(texture.texture_0));
			glBindTexture(GL_TEXTURE_2D, (texture.texture_0));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			if (texture.numChannels == 1) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, texture.width, texture.height, 0, GL_LUMINANCE,
					GL_UNSIGNED_BYTE, texture.data);
			}
			if (texture.numChannels == 2) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, texture.width, texture.height, 0, GL_LUMINANCE_ALPHA,
					GL_UNSIGNED_BYTE, texture.data);
			}
			if (texture.numChannels == 3) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB,
					GL_UNSIGNED_BYTE, texture.data);
			}
			if (texture.numChannels == 4) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA,
					GL_UNSIGNED_BYTE, texture.data);
			}

			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		
		if (specular)
		{
			glGenTextures(1, &(texture.texture_1));
			glBindTexture(GL_TEXTURE_2D, (texture.texture_1));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			if (texture.numChannels == 1) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, texture.width, texture.height, 0, GL_LUMINANCE,
					GL_UNSIGNED_BYTE, texture.data);
			}
			if (texture.numChannels == 2) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, texture.width, texture.height, 0, GL_LUMINANCE_ALPHA,
					GL_UNSIGNED_BYTE, texture.data);
			}
			if (texture.numChannels == 3) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB,
					GL_UNSIGNED_BYTE, texture.data);
			}
			if (texture.numChannels == 4) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA,
					GL_UNSIGNED_BYTE, texture.data);
			}

			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		
		if (normal)
		{
			glGenTextures(1, &(texture.texture_2));
			glBindTexture(GL_TEXTURE_2D, (texture.texture_2));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			if (texture.numChannels == 1) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, texture.width, texture.height, 0, GL_LUMINANCE,
					GL_UNSIGNED_BYTE, texture.data);
			}
			if (texture.numChannels == 2) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, texture.width, texture.height, 0, GL_LUMINANCE_ALPHA,
					GL_UNSIGNED_BYTE, texture.data);
			}
			if (texture.numChannels == 3) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB,
					GL_UNSIGNED_BYTE, texture.data);
			}
			if (texture.numChannels == 4) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA,
					GL_UNSIGNED_BYTE, texture.data);
			}

			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		
	}

	void GLTF_Importer::populateBuffers()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POSITION_VB]);
		glBufferData(GL_ARRAY_BUFFER, (sizeof(pos[0]) * pos.size()), &pos[0], GL_STATIC_DRAW);
		glVertexAttribPointer(POS_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(POS_LOCATION);

		glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
		glBufferData(GL_ARRAY_BUFFER, (sizeof(normals[0]) * normals.size()), &normals[0], GL_STATIC_DRAW);
		glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(NORMAL_LOCATION);

		glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEX_COORDS_VB]);
		glBufferData(GL_ARRAY_BUFFER, (sizeof(texCoord_0[0]) * texCoord_0.size()), &texCoord_0[0], GL_STATIC_DRAW);
		glVertexAttribPointer(TEX_COORD_0_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(TEX_COORD_0_LOCATION);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (sizeof(indexes[0]) * indexes.size()), &indexes[0], GL_STATIC_DRAW);
	}

	void GLTF_Importer::draw(GLFWwindow* window)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		shader->use();
		m_Scene->cam->processInput(window);
		glBindVertexArray(VAO);

		shader->use();
		shader->setVec3("light.position", lightPos);
		shader->setVec3("viewPos", m_Scene->cam->cameraPos);

		// light properties
		shader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		shader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		// material properties
		shader->setFloat("material.shininess", 64.0f);

		for (int i = 0; i < m_Meshes.size(); i++)
		{
			unsigned int u_model = glGetUniformLocation(shader->shaderProgramID, "model");
			unsigned int u_view = glGetUniformLocation(shader->shaderProgramID, "view");
			unsigned int u_projection = glGetUniformLocation(shader->shaderProgramID, "projection");
			glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(m_Meshes[i].m_WorldMat));
			glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(m_Scene->cam->getViewMat()));
			glUniformMatrix4fv(u_projection, 1, GL_FALSE, glm::value_ptr(m_Scene->cam->getProjMat()));

			unsigned int materialIndex = m_Meshes[i].m_MaterialIndex;
			if (m_Materials[materialIndex].hasTexture_BaseColor) {
				m_Materials[materialIndex].baseColorTex_0.bindTex_0();
				glUniform1i(glGetUniformLocation(shader->shaderProgramID, "material.diffuse"), 0);
			}
			if (m_Materials[materialIndex].hasTexture_Specular) {
				m_Materials[materialIndex].specularTex_1.bindTex_1();
				glUniform1i(glGetUniformLocation(shader->shaderProgramID, "material.specular"), 1);
			}
			/*if (false && m_Materials[materialIndex].hasTexture_Normal) {
				m_Materials[materialIndex].normalTex_2.bindTex_2();
				glUniform1i(glGetUniformLocation(shader->shaderProgramID, "normal_2"), 2);
			}*/
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL),
			glDrawElementsBaseVertex(
				GL_TRIANGLES,
				m_Meshes[i].m_NumIndices,
				GL_UNSIGNED_INT,
				(void*)(sizeof(unsigned int) * 3 * m_Meshes[i].m_BaseIndex),
				m_Meshes[i].m_BaseVertex
			);
		}
		glBindVertexArray(0);
	}
}