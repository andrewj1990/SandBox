#pragma once

#include <unordered_map>
#include "Shader.h"
#include <memory>

class ResourceManager
{
public:
	static ResourceManager& getInstance()
	{
		static ResourceManager instance;
		return instance;
	}

	ResourceManager(ResourceManager const&) = delete;
	void operator=(ResourceManager const&) = delete;

	std::shared_ptr<Shader> shader(const std::string& name);
	void addShader(const std::string& name, const std::string& vertShader, const std::string& fragShader);

private:
	ResourceManager() {}

private:
	std::unordered_map<std::string, std::shared_ptr<Shader>> m_ShaderMap;
};