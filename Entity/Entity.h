#pragma once

#include "..\Graphics\Sprite.h"
#include "..\Utils\quadTree.h"
#include "..\Utils\ResourceManager.h"

class Entity {
public:
	Entity();
	Entity(float x, float y);
	Entity(const glm::vec3& pos, const glm::vec2& size, Texture* texture);
	Entity(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& colour);
	virtual ~Entity();

	void init(const glm::vec3& pos, const glm::vec2& size, Texture* texture);

	virtual void update(const Terrain& terrain, float timeElapsed) {}
	virtual void update(const Terrain& terrain, const std::unique_ptr<QuadTree>& quadTree, float timeElapsed) {}
	virtual void update(float timeElapsed);
	virtual void submit(Renderer& renderer);
	virtual void render(Renderer& renderer);

	virtual void renderLight(Renderer& renderer) {}

	virtual bool collide(const Entity& entity) const;
	virtual bool collide(const Renderable& renderable) const;
	virtual bool collide(const BoundingBox& bbox) const;

	virtual void setDestroy(bool flag) { m_Destroy = flag; }
	virtual bool shouldDestroy() const { return m_Destroy; }

	virtual void shoot(float angle) {}
	float getAngle(const Entity& entity);
	virtual float getAngle() const { return 0; }
	virtual void damage(int amount) {}

	float getX() const { return m_Sprite.getPosition().x; }
	float getY() const { return m_Sprite.getPosition().y; }
	float getWidth() const { return m_Sprite.getSize().x; }
	float getHeight() const { return m_Sprite.getSize().y; }
	float getCenterX() const { return getX() + m_Sprite.getSize().x / 2.0f; }
	float getCenterY() const { return getY() + m_Sprite.getSize().y / 2.0f; }

	Sprite& getSprite() { return m_Sprite; }
	const Sprite& getSprite() const { return m_Sprite; }
	std::shared_ptr<BoundingBox> getCollisionBox() { return m_CollisionBox; }

private:
	void init();

protected:
	Sprite m_Sprite;
	std::shared_ptr<BoundingBox> m_CollisionBox;

	float m_X;
	float m_Y;
	float m_Dx;
	float m_Dy;
	float m_Angle;

	bool m_Destroy;

	float m_CumulativeTime;

};