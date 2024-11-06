#include "BasicMaterial.h"

namespace vista
{
	BasicMaterial::BasicMaterial(aiMaterial* _matrial)
	{

        /*aiTextureType textureTypes[] = {
            aiTextureType_BASE_COLOR,
            aiTextureType_DIFFUSE,
            aiTextureType_SPECULAR,
            aiTextureType_NORMALS
        };
        aiString aiPath;
        aiTextureMapping mapping;
        aiTextureMapMode mapMode;
        for (auto textureType : textureTypes) {
            unsigned int textureCount = _matrial->GetTextureCount(textureType);
            if (textureCount > 0) {
                for (unsigned int i = 0; i < textureCount; i++) {
                    aiReturn result = _matrial->GetTexture(
                        textureType,
                        i,
                        &aiPath,
                        &mapping,
                        &uvIndex,
                        NULL,
                        NULL,
                        &mapMode
                    );

                    if (result == aiReturn_SUCCESS) {
                        CORE_LOG_INFO("Texture Type: {0}, Path: {1}", (int)textureType, aiPath.C_Str());
                    }
                }
            }
            else {
                CORE_LOG_WARN("No textures of type {0} found.", (int)textureType);
            }
        }*/
        //aiString aiPath;
        //aiReturn result;

        //// Example: Query the base color texture path
        //result = _matrial->Get(AI_MATKEY_TEXTURE(aiTextureType_BASE_COLOR, 0), aiPath);

        //if (result == aiReturn_SUCCESS) {
        //    std::string path = aiPath.C_Str();
        //    CORE_LOG_INFO("Base Color Texture Path: {0}", path);
        //}
        //else {
        //    CORE_LOG_WARN("Failed to retrieve base color texture path.");
        //}
        aiTextureType textureTypes[] = {
            aiTextureType_BASE_COLOR,
            aiTextureType_DIFFUSE,
            aiTextureType_SPECULAR,
            aiTextureType_NORMALS,
            aiTextureType_METALNESS
        };
        for (auto textureType : textureTypes) {
            unsigned int textureCount = _matrial->GetTextureCount(textureType);
            CORE_LOG_INFO("count of Texture: {0} of type: {1}\n", textureCount, int(textureType));
        }
	}
}