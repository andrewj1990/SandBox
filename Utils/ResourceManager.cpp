#include "ResourceManager.h"

std::shared_ptr<Shader> ResourceManager::shader(const std::string& name)
{
	auto it = m_ShaderMap.find(name);
	if (it != m_ShaderMap.end())
	{
		return it->second;
	}

	// return default shader
	m_ShaderMap[name] = std::shared_ptr<Shader>(new Shader("Shaders/simple.vs", "Shaders/simple.frag"));
	return m_ShaderMap[name];
}

void ResourceManager::addShader(const std::string& name, const std::string& vertShader, const std::string& fragShader)
{
	m_ShaderMap[name] = std::shared_ptr<Shader>(new Shader(vertShader.c_str(), fragShader.c_str()));
}