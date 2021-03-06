#include "Utils.h"

float Utils::random()
{
	float r = rand();
	return r;
}

int Utils::random(int lowerBound, int upperBound)
{
	int range = upperBound - lowerBound;
	float r = rand() % range;
	return lowerBound + r;
}

float Utils::random(float lowerBound, float upperBound)
{
	float r = lowerBound + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (upperBound - lowerBound)));
	return r;
}

bool Utils::chance(int percent)
{
	return Utils::random(0, 100) < percent;
}

int Utils::fastFloor(float v)
{
	return v >= 0 ? (int)v : (int)v - 1;
}

// t range [0, 1] -- a < b
float Utils::lerp(float a, float b, float t)
{
	if (t < 0 || t > 1)
	{
		std::cout << "lerp error\n";
	}
	return a + (b - a) * t;
}

float Utils::calcAngleDeg(float fromX, float fromY, float toX, float toY)
{
	float dx = toX - fromX;
	float dy = toY - fromY;

	return glm::degrees(std::atan2f(dy, dx));
}

float Utils::calcAngleRad(float fromX, float fromY, float toX, float toY)
{
	float dx = toX - fromX;
	float dy = toY - fromY;

	return std::atan2f(dy, dx);
}

bool Utils::inRange(float px, float py, float tx, float ty, float radius)
{
	float dx = tx - px;
	float dy = ty - py;
	float dist = dx * dx + dy * dy;
	return (dist < (radius * radius));
}

bool Utils::lineIntersection(const glm::vec4& line1, const glm::vec4& line2)
{
	float s1X = line1.z - line1.x;
	float s2X = line2.z - line2.x;
	float s1Y = line1.w - line1.y;
	float s2Y = line2.w - line2.y;

	float s = (-s1Y * (line1.x - line2.x) + s1X * (line1.y - line2.y)) / (-s2X * s1Y + s1X * s2Y);
	float t = ( s2X * (line1.y - line2.y) - s2Y * (line1.x - line2.x)) / (-s2X * s1Y + s1X * s2Y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		return true;
	}

	return false;
}

glm::vec2 Utils::intersection(const BoundingBox& bbox, const glm::vec4& line)
{
	int t1 = INT_MAX;

	// intersections of bounding box
	int x = bbox.x;
	int y = bbox.y;
	int w = bbox.width;
	int h = bbox.height;

	float endX = 0;
	float endY = 0;

	float dirX = line.z - line.x;
	float dirY = line.w - line.y;

	// find the end point
	intersect(x, y, x + w, y, line, dirX, dirY, t1, endX, endY);
	intersect(x, y, x, y + h, line, dirX, dirY, t1, endX, endY);
	intersect(x, y + h, x + w, y + h, line, dirX, dirY, t1, endX, endY);
	intersect(x + w, y, x + w, y + h, line, dirX, dirY, t1, endX, endY);

	return glm::vec2(endX, endY);
}

void Utils::intersect(float sx, float sy, float ex, float ey, const glm::vec4& line, float dirX, float dirY, int& T1, float& endPointX, float& endPointY)
{
	float dx = ex - sx;
	float dy = ey - sy;

	float t2 = (dirX * (sy - line.y) + dirY * (line.x - sx)) / (dx * dirY - dy * dirX);
	float t1 = (sx + dx * t2 - line.x) / dirX;


	if (t1 > 0 && t2 > 0 && t2 < 1)
	{
		if (t1 < T1)
		{
			T1 = t1;
			endPointX = line.x + dirX * t1;
			endPointY = line.y + dirY * t1;
		}
	}
}

glm::mat4 Utils::calcTransformMat(float x, float y, float w, float h, float angle)
{
	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(x + w / 2.0f, y + h / 2.0f, 0));
	transform = glm::rotate(transform, angle, glm::vec3(0, 0, 1));
	transform = glm::translate(transform, glm::vec3(-x - w / 2.0f, -y - h / 2.0f, 0));
	return transform;
}

bool Utils::quadCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2)
{
	float x11 = std::min(x1, x1 + w1);
	float x12 = std::max(x1, x1 + w1);
	float y11 = std::min(y1, y1 + h1);
	float y12 = std::max(y1, y1 + h1);

	float x21 = std::min(x2, x2 + w2);
	float x22 = std::max(x2, x2 + w2);
	float y21 = std::min(y2, y2 + h2);
	float y22 = std::max(y2, y2 + h2);

	//return (x11 <= x22 && x12 >= x21 && y11 <= y22 && y12 >= y22);
	return (x11 <= x22 && x12 >= x21 && y11 <= y22 && y12 >= y21);

	//return (x1 <= x2 + w2 && x1 + w1 >= x2 && y1 <= y2 + h2 && y1 + h1 >= y2);
}

// credit goes to blackpawn : http://blackpawn.com/texts/pointinpoly/default.html
bool Utils::trianglePointCollision(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& p)
{
	glm::vec2 v0 = c - a;
	glm::vec2 v1 = b - a;
	glm::vec2 v2 = p - a;

	float dot00 = glm::dot(v0, v0);
	float dot01 = glm::dot(v0, v1);
	float dot02 = glm::dot(v0, v2);
	float dot11 = glm::dot(v1, v1);
	float dot12 = glm::dot(v1, v2);

	float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	return (u >= 0) && (v >= 0) && (u + v < 1);
}

std::unique_ptr<Action> Utils::getRandomAction()
{
	ActionType type = static_cast<ActionType>(rand() % static_cast<int>(ActionType::END));
	std::cout << "type : " << (int)type << "\n";

	return getAction(type);
}

std::unique_ptr<Action> Utils::getAction(ActionType actionType)
{
	switch (actionType)
	{
	case ActionType::MOVE:
		return std::unique_ptr<MoveAction>(new MoveAction());
	case ActionType::ATTACK:
		return std::unique_ptr<AttackAction>(new AttackAction());
	case ActionType::AOEATTACK:
		return std::unique_ptr<AttackAction>(new AttackAction());
		//return std::unique_ptr<AOEAttackAction>(new AOEAttackAction());
	default:
		return std::unique_ptr<MoveAction>(new MoveAction());
	}
}

std::vector<std::unique_ptr<Action>> Utils::getRandomActionList(int size)
{
	std::vector<int> v(static_cast<int>(ActionType::END));
	std::iota(v.begin(), v.end(), 0);

	std::random_shuffle(v.begin(), v.end());

	std::vector<std::unique_ptr<Action>> actionVec;

	if (size > v.size()) size = v.size();
	for (int i = 0; i < size; i++)
	{
		actionVec.push_back(getAction(static_cast<ActionType>(v[i])));
	}

	return actionVec;
}
