#include "Model.h"

Model::Model(GLchar* path)
{
	loadModel(path);
}

void Model::draw(const Shader& shader)
{
	for (int i = 0; i < m_Meshes.size(); i++)
	{
		m_Meshes[i].draw(shader);
	}
}

void Model::loadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << "\n";
		return;
	}
	m_Directory = path.substr(0, path.find_last_of('/'));
	processNode(*scene->mRootNode, *scene);
}

void Model::processNode(aiNode& node, const aiScene& scene)
{
	for (GLuint i = 0; i < node.mNumMeshes; i++)
	{
		aiMesh* mesh = scene.mMeshes[node.mMeshes[i]];
		m_Meshes.push_back(processMesh(*mesh, scene));
	}

	for (GLuint i = 0; i < node.mNumChildren; i++)
	{
		processNode(*node.mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh& mesh, const aiScene& scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	for (GLuint i = 0; i < mesh.mNumVertices; i++)
	{
		Vertex vertex;

		glm::vec3 vector;
		vector.x = mesh.mVertices[i].x;
		vector.y = mesh.mVertices[i].y;
		vector.z = mesh.mVertices[i].z;
		vertex.postion = vector;

		vector.x = mesh.mNormals[i].x;
		vector.y = mesh.mNormals[i].y;
		vector.z = mesh.mNormals[i].z;
		vertex.normal = vector;

		glm::vec2 vec(0.0f, 0.0f);
		if (mesh.mTextureCoords[0])
		{
			vec.x = mesh.mTextureCoords[0][i].x;
			vec.y = mesh.mTextureCoords[0][i].y;
		}
			
		vertex.texCoords = vec;

		vertices.push_back(vertex);
	}

	for (GLuint i = 0; i < mesh.mNumFaces; i++)
	{
		aiFace face = mesh.mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh.mMaterialIndex >= 0)
	{
		aiMaterial* material = scene.mMaterials[mesh.mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(*material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(*material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial& mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;

	for (GLuint i = 0; i < mat.GetTextureCount(type); i++)
	{
		aiString str;
		mat.GetTexture(type, i, &str);
		GLboolean skip = false;
		for (GLuint j = 0; j < textures_loaded.size(); j++)
		{
			if (textures_loaded[j].path == str)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		
		if (!skip)
		{
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), m_Directory);
			texture.type = typeName;
			texture.path = str;
			textures_loaded.push_back(texture);
		}
	}

	return textures;
}

GLuint Model::TextureFromFile(const char* path, std::string directory)
{
	//Generate texture ID and load texture data 
	std::string filename = path;
	filename = directory + '/' + filename;
	GLuint textureID;
	glGenTextures(1, &textureID);
	unsigned int width, height;
	unsigned char* image = loadImage(filename.c_str(), width, height);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	delete image;

	return textureID;
}
