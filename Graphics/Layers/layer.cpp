#include "layer.h"

Layer::Layer(Shader& shader, const glm::mat4 projectionMatrix)
	: m_ProjectionMatrix(projectionMatrix), m_Shader(shader), m_Renderer()
{
	m_Shader.use();
	m_Shader.setUniform("pr_matrix", projectionMatrix);

	GLint texID[32];
	for (int i = 0; i < 32; ++i)
	{
		texID[i] = i;
	}

	//m_Shader.setUniform("textures", 32, texID);
	m_Shader.unbind();
}

void Layer::add(Renderable* renderable)
{
	m_Renderables.push_back(renderable);
}

void Layer::remove(Renderable* ptr)
{
	for (int i = m_Renderables.size() - 1; i >= 0; --i)
	{
		if (m_Renderables[i] == ptr)
		{
			m_Renderables.erase(m_Renderables.begin() + i);
			return;
		}
	}
}

void Layer::clear()
{
	m_Renderables.clear();
}

void Layer::render()
{
	m_Shader.use();

	m_Renderer.begin();
	for (Renderable* renderable : m_Renderables)
	{
		renderable->submit(m_Renderer);
	}

	m_Renderer.end();
	m_Renderer.flush();
}

void Layer::render(std::vector<Renderable*> renderables)
{
	m_Shader.use();

	m_Renderer.begin();
	for (Renderable* renderable : renderables)
	{
		//m_Renderer.submit(*renderable);
		renderable->submit(m_Renderer);
	}
	m_Renderer.end();
	m_Renderer.flush();
}

void Layer::setProjectionMatrix(const glm::mat4 projectionMatrix)
{
	m_Shader.use();
	m_Shader.setUniform("pr_matrix", projectionMatrix);
	m_Shader.unbind();
}

void Layer::setModelViewMatrix(const glm::mat4& modelMatrix)
{
	m_Shader.use();
	m_Shader.setUniform("vw_matrix", modelMatrix);
	m_Shader.unbind();
}
