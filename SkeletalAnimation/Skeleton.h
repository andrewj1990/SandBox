#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "..\Graphics\Renderer.h"
#include "..\Graphics\renderable.h"
#include "..\Graphics\texture_manager.h"

struct KeyFrame
{
	int time;
	float angle;
	float dx;
	float dy;
};

struct Bone
{
	std::string name;
	glm::vec4 params; // (x, y, angle, length)
	glm::vec2 size;

	float offsetAngle;
	float offsetX;
	float offsetY;

	bool absolute;

	int parent;
	std::vector<int> childs;

	std::vector<KeyFrame> keyFrame;

	std::string texture;
};

class Skeleton
{
public:
	Skeleton(const int& x = 0, const int& y = 0);

	int boneIndex(std::string name) const;

	void addChild(const std::string& parentName, const glm::vec4& params, const glm::vec2& size, const std::string& texFile, const std::string& childName, bool absolute = false);

	Bone& getBone(const std::string& name);

	void addAnimationData(const std::string& name, int time, float angle, float dx = 0.0f, float dy = 0.0f);
	void animate(const std::string& name, int time);
	void move(const std::string& name, float x, float y, float angle);
	void printSkeletonInfo(std::string name = "root", int level = 0) const;
	
	void render(Renderer& renderer);
	void render(const std::string& name, Renderer& renderer);

	Bone& getRoot() { return m_Root; }

private:
	Bone m_Root;
	std::vector<Bone> m_Bones;

};