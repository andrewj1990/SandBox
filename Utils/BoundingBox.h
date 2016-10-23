#pragma once

#include "..\Graphics\renderable.h"

struct BoundingBox
{
	int x;
	int y;
	int width;
	int height;

	BoundingBox(int x, int y, int width, int height)
		: x(x), y(y), width(width), height(height)
	{ }

	bool contains(const Renderable& data)
	{
		bool containsX = (x <= data.getPosition().x && data.getPosition().x <= x + width);
		bool containsY = (y <= data.getPosition().y && data.getPosition().y <= y + height);

		return (containsX && containsY);
	}

	bool intersects(const Renderable& box)
	{
		return (x <= box.getPosition().x + box.getSize().x && x + width >= box.getPosition().x && y <= box.getPosition().y + box.getSize().y && y + height >= box.getPosition().y);
	}

	bool intersects(const BoundingBox& box)
	{
		return (x <= box.x + box.width && x + width >= box.x && y <= box.y + box.height && y + height >= box.y);
	}
};