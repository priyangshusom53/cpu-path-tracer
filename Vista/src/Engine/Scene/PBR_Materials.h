#pragma once
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

#include"Engine/Log.h"
#include"Engine/Renderer/OpenGLShader.h"
#include"Engine/Scene/Camera.h"

class Material
{
 public:
	Material()
	{
		name = "undefined";

		doubleSided = true;

		baseColorFactor = glm::dvec4(1.0);
		roughnessFactor = 0.0;
		metallicFactor = 0.0;

		KHR_materials_specular = glm::vec3(0.0);

		KHR_materials_ior = 0.0;
		KHR_materials_transmission = 0.0;

		isEmmisive = false;
		emissiveFactor = glm::dvec3(1.0);
		KHR_materials_emissive_strength = 4.0;
	}

	std::string name;

	bool doubleSided;

	glm::dvec4 baseColorFactor;
	double roughnessFactor;
	double metallicFactor;

	glm::dvec3 KHR_materials_specular;

	double KHR_materials_ior;
	double KHR_materials_transmission;

	bool isEmmisive;
	glm::dvec3 emissiveFactor;
	double KHR_materials_emissive_strength;


};