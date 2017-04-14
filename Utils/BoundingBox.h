#pragma once

class Renderable;

class BoundingBox
{
public:
	BoundingBox();

	BoundingBox(float x, float y, float width, float height);

	~BoundingBox() { }

	bool contains(const Renderable& data);
	bool contains(const BoundingBox& data);
	bool contains(int px, int py);
	bool intersects(const Renderable& box);
	bool intersects(const BoundingBox& box, bool offset = false);

public:
	float x;
	float y;
	float width;
	float height;

};