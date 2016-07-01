#pragma once

#include <vector>
#include <string>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include "Mesh.h"
#include "..\Utils\LoadImage.h"

class Model
{
public:
	Model(GLchar* path);
	
	void draw(const Shader& shader);

	std::vector<Texture> textures_loaded;
private:
	void loadModel(std::string path);
	void processNode(aiNode& node, const aiScene& scene);
	Mesh processMesh(aiMesh& mesh, const aiScene& scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial& mat, aiTextureType type, std::string typeName);
	GLuint TextureFromFile(const char* path, std::string directory);

public:
	std::vector<Mesh> m_Meshes;
	std::string m_Directory;

};