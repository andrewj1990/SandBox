#pragma once

#include "..\..\Graphics\renderable.h"
#include "..\..\Entity\Entity.h"

class Tile : public Renderable
{
public:
	Tile();

	void init(float x, float y, const glm::vec4& colour, bool solid, bool treeTile) override;

	void submit(Renderer& renderer) const override;

	bool isTreeTile() const override { return m_TreeTile && !m_Solid; }

	std::shared_ptr<Entity> getTree() override
	{
		return m_Tree;
	}

private:
	bool m_TreeTile;

	std::shared_ptr<Entity> m_Tree;
};