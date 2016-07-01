#include "label.h"

Label::Label(const std::string& text, float x, float y, Font* font, glm::vec4 colour)
	: Renderable(), text(text), position(m_Position), m_Font(font)
{
	m_Position = glm::vec3(x, y, 0.0f);
	m_Colour = colour;
}

Label::Label(const std::string& text, float x, float y, const std::string& font, glm::vec4 colour)
	: Renderable(), text(text), position(m_Position), m_Font(FontManager::get(font))
{
	m_Position = glm::vec3(x, y, 0.0f);
	m_Colour = colour;
}

void Label::submit(Renderer& renderer) const
{
	renderer.drawString(*m_Font, text, m_Position, m_Colour);
}	