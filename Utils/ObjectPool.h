#pragma once

#include <memory>
#include <stack>
#include <stdexcept>

template <class T, class D = std::default_delete<T>>
class ObjectPool
{
private:
	struct ReturnToPool_Deleter {
		explicit ReturnToPool_Deleter(std::weak_ptr<ObjectPool<T, D>* > pool)
			: pool_(pool) {}

		void operator()(T* ptr) {
			if (auto pool_ptr = pool_.lock())
				(*pool_ptr.get())->add(std::unique_ptr<T, D>{ptr});
			else
				D{}(ptr);
		}
	private:
		std::weak_ptr<ObjectPool<T, D>* > pool_;
	};

public:
	using ptr_type = std::unique_ptr<T, ReturnToPool_Deleter >;

	ObjectPool() : this_ptr_(new ObjectPool<T, D>*(this)) {}
	virtual ~ObjectPool() {}

	void add(std::unique_ptr<T, D> t) {
		pool_.push(std::move(t));
	}

	ptr_type acquire() {
		if (pool_.empty())
			throw std::out_of_range("Cannot acquire object from an empty pool.");

		ptr_type tmp(pool_.top().release(),
			ReturnToPool_Deleter{
			std::weak_ptr<SmartObjectPool<T, D>*>{this_ptr_} });
		pool_.pop();
		return std::move(tmp);
	}

	bool empty() const {
		return pool_.empty();
	}

	size_t size() const {
		return pool_.size();
	}

private:
	std::shared_ptr<ObjectPool<T, D>* > this_ptr_;
	std::stack<std::unique_ptr<T, D> > pool_;
};

//#include <algorithm>
//
//#include "..\Graphics\Renderer.h"
//
//
//class ObjectPool
//{
//public:
//	ObjectPool() 
//	{
//		//for (int i = 0; i < POOL_SIZE; i++)
//		//{
//		//	m_Pool[i].setDestroy(true);
//		//}
//
//		m_PoolVec = std::vector<Entity>(POOL_SIZE);
//		for (Entity& item : m_PoolVec)
//		{
//			item.setDestroy(true);
//		}
//	}
//
//	~ObjectPool() {}
//
//	Entity* getNextObject() 
//	{
//		for (int i = 0; i < POOL_SIZE; i++)
//		{
//			if (m_Pool[i].shouldDestroy())
//			{
//				return &m_Pool[i];
//			}
//		}
//
//	}
//
//	void create(const glm::vec3& pos, const glm::vec2& size, Texture* texture)
//	{
//		//for (int i = 0; i < POOL_SIZE; i++)
//		//{
//		//	if (m_Pool[i].shouldDestroy())
//		//	{
//		//		m_Pool[i].init(pos, size, texture);
//		//		return;
//		//	}
//		//}
//
//		for (Entity& item : m_PoolVec)
//		{
//			if (item.shouldDestroy())
//			{
//				item.init(pos, size, texture);
//				return;
//			}
//		}
//	}
//
//	void add(Entity& entity)
//	{
//
//		for (Entity& item : m_PoolVec)
//		{
//			if (item.shouldDestroy())
//			{
//				item = entity;
//				return;
//			}
//		}
//	}
//
//	void update(const Terrain& terrain, const std::unique_ptr<QuadTree>& quadTree, float timeElapsed)
//	{
//		//for (int i = 0; i < POOL_SIZE; i++)
//		//{
//		//	if (!m_Pool[i].shouldDestroy())
//		//	{
//		//		m_Pool[i].update(timeElapsed);
//		//	}
//		//}
//
//		for (Entity& item : m_PoolVec)
//		{
//			if (!item.shouldDestroy())
//			{
//				item.update(terrain, quadTree, timeElapsed);
//			}
//		}
//
//		std::sort(m_PoolVec.begin(), m_PoolVec.end(), [](const auto& a, const auto& b) { return a.getY() > b.getY(); });
//
//	}
//
//	void render(Renderer& renderer)
//	{
//		//for (int i = 0; i < POOL_SIZE; i++)
//		//{
//		//	if (!m_Pool[i].shouldDestroy())
//		//	{
//		//		ResourceManager::getInstance().shader("basic_shader")->use();
//		//		m_Pool[i].render(renderer);
//		//		ResourceManager::getInstance().shader("outline_shader")->use();
//		//		m_Pool[i].render(renderer);
//		//	}
//		//}
//
//		for (Entity& item : m_PoolVec)
//		{
//			if (!item.shouldDestroy())
//			{
//				item.render(renderer);
//			}
//		}
//	}
//	
//private:
//	static const unsigned int POOL_SIZE = 200;
//	Entity m_Pool[POOL_SIZE];
//	std::vector<Entity> m_PoolVec;
//};