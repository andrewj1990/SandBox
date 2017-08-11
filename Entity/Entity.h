#pragma once

#include "..\Graphics\Sprite.h"
#include "..\Utils\quadTree.h"
#include "..\Utils\ResourceManager.h"
#include "Particles\ParticleManager.h"

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
	float getAngle() const override { return m_Angle; }
	float calcDistanceSqrd(const Entity& entity) const;
	virtual void damage(int amount) {/* m_Destroy = true; */}
	virtual void updateLight() { }

	float m_Dx;
	float m_Dy;

private:
	void init();

protected:

	float m_CumulativeTime;
};