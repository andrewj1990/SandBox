#include "light.h"

Light::Light(Layer& shadowLayer, const std::vector<Renderable*>& blocks, int x, int y, glm::vec4 color)
	: m_Layer(shadowLayer), m_Blocks(blocks), m_Color(color), m_X(x), m_Y(y)
{
	calculateShadow();
}

void Light::update(const int& x, const int& y)
{
	m_X = x;
	m_Y = y;
	calculateShadow();
}

void Light::setRenderables(const std::vector<Renderable*>& blocks)
{

}

void Light::calculateShadow()
{
	m_Layer.clear();
	m_Shadows.clear();

	// for each block create a quad for the shadow behind it
	for (Renderable* block : m_Blocks)
	{
		// find middle points of each block
		std::vector<glm::vec3> midPoint = midPoints(block);

		// end points
		glm::vec3 midpoint1 = midPoint[1];
		glm::vec3 midpoint2 = midPoint[2];

		float dx1 = midpoint1.x - m_X;
		float dy1 = midpoint1.y - m_Y;

		float point1x = midpoint1.x + dx1 * INT16_MAX;
		float point1y = midpoint1.y + dy1 * INT16_MAX;

		float dx2 = midpoint2.x - m_X;
		float dy2 = midpoint2.y - m_Y;

		float point2x = midpoint2.x + dx2 * INT16_MAX;
		float point2y = midpoint2.y + dy2 * INT16_MAX;

		//m_Shadows.push_back(std::unique_ptr<Renderable>(new Renderable(0, 0, 100, 0, 100, 100, 100, 0)));
		m_Shadows.push_back(std::unique_ptr<Renderable>(new Renderable(midpoint1.x, midpoint1.y, point1x, point1y, point2x, point2y, midpoint2.x, midpoint2.y, m_Color)));
	}

	for (auto& shadow_block : m_Shadows)
	{
		m_Layer.add(shadow_block.get());
	}

}

std::vector<glm::vec3> Light::midPoints(Renderable* block)
{
	std::vector<glm::vec2> result;
	std::vector<glm::vec3> block_pos = block->getPositions();

	//std::cout << "before sort" << "\n";
	//std::cout << "vert1 : " << block_pos[0].x << ", " << block_pos[0].y << "\n";
	//std::cout << "vert2 : " << block_pos[1].x << ", " << block_pos[1].y << "\n";
	//std::cout << "vert3 : " << block_pos[2].x << ", " << block_pos[2].y << "\n";
	//std::cout << "vert4 : " << block_pos[3].x << ", " << block_pos[3].y << "\n";

	std::sort(block_pos.begin(), block_pos.end(), 
		[this](const glm::vec3& a, const glm::vec3& b) 
		{
			int ax = m_X - a.x;
			int ay = m_Y - a.y;
			int bx = m_X - b.x;
			int by = m_Y - b.y;

			int adist = ax * ax + ay * ay;
			int bdist = bx * bx + by * by;
			return adist < bdist;
		} );


	//std::cout << "after sort" << "\n";
	//std::cout << "vert1 : " << block_pos[0].x << ", " << block_pos[0].y << "\n";
	//std::cout << "vert2 : " << block_pos[1].x << ", " << block_pos[1].y << "\n";
	//std::cout << "vert3 : " << block_pos[2].x << ", " << block_pos[2].y << "\n";
	//std::cout << "vert4 : " << block_pos[3].x << ", " << block_pos[3].y << "\n";

	//std::vector<int> distances;
	//for (glm::vec3 pos : block_pos)
	//{
	//	int dx = m_X - pos.x;
	//	int dy = m_Y - pos.y;

	//	int dist = dx * dx + dy * dy;
	//	distances.push_back(dist);
	//}

	//std::sort(distances.begin(), distances.end());

	return block_pos;
}

