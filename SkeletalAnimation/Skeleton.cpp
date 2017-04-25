#include "Skeleton.h"

Skeleton::Skeleton(int x, int y)
{
	m_Root.name = "root";
	m_Root.params = glm::vec4(x, y, 0, 0);
	m_Root.size = glm::vec2(0, 0);
	m_Root.parent = -1;
	m_Root.absolute = false;
	m_Root.offsetAngle = 0.0f;
	m_Root.offsetX = 0.0f;
	m_Root.offsetY = 0.0f;
	m_Root.texture = "";
}

int Skeleton::boneIndex(std::string name) const
{
	for (int i = 0; i < m_Bones.size(); i++)
	{
		if (m_Bones[i].name == name) return i;
	}

	return -1;
}

void Skeleton::addChild(const std::string& parentName, const glm::vec4& params, const glm::vec2& size, const std::string& texFile, const std::string& childName, bool absolute)
{
	// find parent bone
	int pIndex = boneIndex(parentName);

	Bone newChild;
	newChild.name = childName;
	newChild.size = size;
	newChild.absolute = absolute;
	newChild.offsetAngle = 0.0f;
	newChild.offsetX = 0.0f;
	newChild.offsetY = 0.0f;
	newChild.params = params;

	newChild.params.w = params.w;
	newChild.parent = pIndex;
	newChild.texture = texFile;

	m_Bones.push_back(newChild);

	// add new child to parents children vector
	int cIndex = boneIndex(childName);
	if (pIndex >= 0)
	{
		Bone& parentBone = m_Bones[pIndex];
		parentBone.childs.push_back(cIndex);
	}
	else
	{
		m_Root.childs.push_back(cIndex);
	}
}

Bone& Skeleton::getBone(const std::string& name)
{
	int bIndex = boneIndex(name);
	return bIndex == -1 ? m_Root : m_Bones[bIndex];
}

void Skeleton::addAnimationData(const std::string& name, int time, float angle, float dx, float dy)
{
	Bone& bone = getBone(name);

	KeyFrame frame;
	frame.angle = angle;
	frame.dx = dx;
	frame.dy = dy;
	frame.time = time;

	bone.keyFrame.push_back(frame);
}

void Skeleton::animate(const std::string& name, int time)
{
	float angle = 0.0f;
	float dx = 0.0f;
	float dy = 0.0f;
	float t;

	Bone& bone = getBone(name);

	for (int i = 0; i < bone.keyFrame.size(); i++)
	{
		if (bone.keyFrame[i].time == time)
		{
			if (i != bone.keyFrame.size() - 1)
			{
				t = bone.keyFrame[i + 1].time - bone.keyFrame[i].time;
				angle = bone.keyFrame[i + 1].angle - bone.keyFrame[i].angle;
				dx = bone.keyFrame[i + 1].dx - bone.keyFrame[i].dx;
				dy = bone.keyFrame[i + 1].dy - bone.keyFrame[i].dy;

				bone.offsetAngle = angle / t;
				bone.offsetX = dx / t;
				bone.offsetY = dy / t;
			}
			else
			{
				bone.offsetAngle = 0.0f;
				bone.offsetX = 0.0f;
				bone.offsetY = 0.0f;
			}
		}
	}

	move(name, bone.offsetX, bone.offsetY, bone.offsetAngle);

	for (int i = 0; i < bone.childs.size(); i++)
	{
		animate(m_Bones[bone.childs[i]].name, time);
	}
}

void Skeleton::move(const std::string& name, float x, float y, float angle)
{
	int index = boneIndex(name);

	Bone& bone = index == -1 ? m_Root : m_Bones[index];
	bone.params.x += x;
	bone.params.y += y;
	bone.params.z += angle;
}

void Skeleton::printSkeletonInfo(std::string name, int level) const
{
	int index = boneIndex(name);
	Bone bone = index == -1 ? m_Root : m_Bones[index];

	std::string numIndents = "";
	for (int i = 0; i < level; i++)
	{
		numIndents += "\t";
	}

	std::cout << numIndents << index << ")" << " name : " << bone.name << "\n";
	std::cout << numIndents << "\tparent : " << bone.parent << "\n";
	std::cout << numIndents << "\tparams : (" << bone.params.x << ", " << bone.params.y << ", " << bone.params.z << ", " << bone.params.w << ")\n";
	std::cout << numIndents << "\tnum childs : " << bone.childs.size() << "\n";

	for (int i = 0; i < bone.childs.size(); i++)
	{
		printSkeletonInfo(m_Bones[bone.childs[i]].name, level + 1);
	}
}

void Skeleton::render(Renderer& renderer)
{
	renderer.begin();
	submit("root", renderer);
	renderer.end();
	renderer.flush();
}

void Skeleton::submit(const std::string& name, Renderer & renderer)
{
	Bone& bone = getBone(name);

	for (int i = 0; i < bone.childs.size(); i++)
	{
		Bone& cBone = m_Bones[bone.childs[i]];
		Renderable renderable(glm::vec3(cBone.params.x, cBone.params.y, 0), glm::vec2(cBone.size.x, cBone.size.y), TextureManager::get(cBone.texture));
		//Renderable renderable(glm::vec3(cBone.params.x, cBone.params.y, 0), glm::vec2(cBone.size.x, cBone.size.y), TextureManager::get("Textures/PlayerSpritesheet10.png"));

		glm::mat4 transformation = glm::mat4();

		if (name == "root")
		{
			transformation = glm::translate(transformation, glm::vec3(bone.params.x, bone.params.y, 0));
			transformation = glm::translate(transformation, glm::vec3(cBone.size / 2.0f, 0.0f));
			transformation = glm::rotate(transformation, glm::radians(bone.params.z), glm::vec3(0, 0, 1));
			transformation = glm::translate(transformation, glm::vec3(-cBone.size / 2.0f, 0.0f));
		}

		transformation = glm::translate(transformation, glm::vec3(cBone.params.x, cBone.params.y, 0));
		transformation = glm::rotate(transformation, glm::radians(cBone.params.z), glm::vec3(0, 0, 1));

		renderer.push(transformation);
		renderer.submit(renderable);
		submit(cBone.name, renderer);
		renderer.pop();
	}

	if (name == "root")
	{
		renderer.pop();
	}

}

void Skeleton::render(const std::string& name, Renderer& renderer)
{
	Bone& bone = getBone(name);

	for (int i = 0; i < bone.childs.size(); i++)
	{
		Bone& cBone = m_Bones[bone.childs[i]];
		Renderable renderable(glm::vec3(cBone.params.x, cBone.params.y, 0), glm::vec2(cBone.size.x, cBone.size.y), TextureManager::get(cBone.texture));
		//Renderable renderable(glm::vec3(cBone.params.x, cBone.params.y, 0), glm::vec2(cBone.size.x, cBone.size.y), TextureManager::get("Textures/PlayerSpritesheet10.png"));

		glm::mat4 transformation = glm::mat4();

		if (name == "root")
		{
			transformation = glm::translate(transformation, glm::vec3(bone.params.x, bone.params.y, 0));
			transformation = glm::translate(transformation, glm::vec3(cBone.size / 2.0f, 0.0f));
			transformation = glm::rotate(transformation, glm::radians(bone.params.z), glm::vec3(0, 0, 1));
			transformation = glm::translate(transformation, glm::vec3(-cBone.size / 2.0f, 0.0f));
		}

		transformation = glm::translate(transformation, glm::vec3(cBone.params.x, cBone.params.y, 0));
		transformation = glm::rotate(transformation, glm::radians(cBone.params.z), glm::vec3(0, 0, 1));

		renderer.push(transformation);
		renderer.render(renderable);
		render(cBone.name, renderer);
		renderer.pop();
	}

	if (name == "root")
	{
		renderer.pop();
	}

}
