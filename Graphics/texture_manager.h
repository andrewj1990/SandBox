#pragma once

#include <vector>
#include <unordered_map>
#include "texture.h"

class Texture;

class TextureManager
{
public:
	static void add(Texture* texture);
	static Texture* get(const std::string& name);
	static Texture* get(const std::string& name, int row, int col);
	static void clean();

private:
	TextureManager();

private:
	static std::vector<Texture*> m_Textures;
	static std::unordered_map<std::string, Texture*> m_TextureMap;
};