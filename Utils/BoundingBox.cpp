#include "BoundingBox.h"
#include "..\Graphics\renderable.h"

BoundingBox::BoundingBox()
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;
}

BoundingBox::BoundingBox(float x, float y, float width, float height)
	: x(x), y(y), width(width), height(height)
{
}

bool BoundingBox::contains(const Renderable& data)
{
	bool containsX = (x <= data.getPosition().x && data.getPosition().x <= x + width);
	bool containsY = (y <= data.getPosition().y && data.getPosition().y <= y + height);

	return (containsX && containsY);
}

bool BoundingBox::intersects(const Renderable& box)
{
	return (x <= box.getPosition().x + box.getSize().x && x + width >= box.getPosition().x && y <= box.getPosition().y + box.getSize().y && y + height >= box.getPosition().y);
}

// intersects with offset
bool BoundingBox::intersects(const BoundingBox& box, bool offset)
{
	if (offset)
	{
		int xx = box.x - 16;
		int yy = box.y - 16;
		int ww = box.width + 32;
		int hh = box.height + 32;

		return (x <= xx + ww && x + width >= xx && y <= yy + hh && y + height >= yy);
	}

	return (x <= box.x + box.width && x + width >= box.x && y <= box.y + box.height && y + height >= box.y);
}
