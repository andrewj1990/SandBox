#pragma once

#include "..\Graphics\Sprite.h"
#include "..\Utils\quadTree.h"
#include "..\Utils\ResourceManager.h"

class Entity : public Sprite {
public:
	Entity();
	Entity(float x, float y);
	Entity(const glm::vec3& pos, const glm::vec2& size, Texture* texture);
	Entity(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& colour);
	virtual ~Entity();

	void init(const glm::vec3& pos, const glm::vec2& size, Texture* texture);

	virtual void update(float timeElapsed);
	virtual void submit(Renderer& renderer);
	virtual void render(Renderer& renderer);

	virtual void renderLight(Renderer& renderer) {}

	virtual bool collide(const Entity& entity) const;
	virtual bool collide(const Renderable& renderable) const;
	virtual bool collide(const BoundingBox& bbox) const;

	virtual void shoot(float angle) {}
	float getAngle(const Entity& entity);
	virtual float getAngle() const { return m_Angle; }
	virtual void damage(int amount) {}

private:
	void init();

protected:
	float m_Dx;
	float m_Dy;

	float m_CumulativeTime;
};