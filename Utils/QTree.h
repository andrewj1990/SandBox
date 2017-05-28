#pragma once

// referenced https://gamedevelopment.tutsplus.com/tutorials/quick-tip-use-quadtrees-to-detect-likely-collisions-in-2d-space--gamedev-374

#include <vector>
#include <iterator>

#include "BoundingBox.h"

template <class T>
class QTree
{
	int max_objects_;
	int max_levels_;
	int level_;

	std::vector<T*> objects_;

	BoundingBox bound_;
	std::unique_ptr<QTree<T>> ne;
	std::unique_ptr<QTree<T>> se;
	std::unique_ptr<QTree<T>> sw;
	std::unique_ptr<QTree<T>> nw;

public:
	QTree(int level, const BoundingBox& bounds);

	void split();

	int getIndex(T* data);
	void insert(T* data);

	void retrieve(std::vector<T*>& data, const BoundingBox& bbox);

	void getBounds(std::vector<BoundingBox>& bounds);
};

template<class T>
inline QTree<T>::QTree(int level, const BoundingBox& bounds)
	: level_(level), bound_(bounds), ne(nullptr), se(nullptr), sw(nullptr), nw(nullptr)
{
	max_objects_ = 50;
	max_levels_ = 10;
}

template<class T>
inline void QTree<T>::split()
{
	int subWidth = bound_.width / 2;
	int subHeight = bound_.height / 2;
	int x = bound_.x;
	int y = bound_.y;

	nw = std::make_unique<QTree<T>>(level_ + 1, BoundingBox(x, y, subWidth, subHeight));
	ne = std::make_unique<QTree<T>>(level_ + 1, BoundingBox(x + subWidth, y, subWidth, subHeight));
	sw = std::make_unique<QTree<T>>(level_ + 1, BoundingBox(x, y + subHeight, subWidth, subHeight));
	se = std::make_unique<QTree<T>>(level_ + 1, BoundingBox(x + subWidth, y + subHeight, subWidth, subHeight));
}

template<class T>
inline int QTree<T>::getIndex(T* data)
{
	int index = -1;

	double vmid = bound_.x + (bound_.width / 2);
	double hmid = bound_.y + (bound_.height / 2);

	bool topQuad = data->getPosition().y + data->getSize().y < hmid;
	bool botQuad = data->getPosition().y > hmid;

	if (data->getPosition().x + data->getSize().x < vmid) {
		if (topQuad) index = 1;
		else if (botQuad) index = 2;
	} else if (data->getPosition().x > vmid) {
		if (topQuad) index = 0;
		else if (botQuad) index = 3;
	}

	return index;
}

template<class T>
inline void QTree<T>::insert(T* data)
{
	// if there are child nodes then find the child node that we insert into
	if (nw != nullptr) {
		int index = getIndex(data);

		if (index != -1) {
			if (index == 0)	ne->insert(data);
			else if (index == 1) nw->insert(data);
			else if (index == 2) sw->insert(data);
			else if (index == 3) se->insert(data);

			return;
		}
	}

	objects_.push_back(data);

	if (objects_.size() > max_objects_ && level_ < max_levels_) {
		if (nw == nullptr) split();

		// remove objects from current bounds and add to leaf nodes
		for (auto i = objects_.begin(); i != objects_.end(); ) {
			int index = getIndex(*i);
			if (index != -1) {
				if (index == 0)	ne->insert(*i);
				else if (index == 1) nw->insert(*i);
				else if (index == 2) sw->insert(*i);
				else if (index == 3) se->insert(*i);
				i = objects_.erase(i);
			} else {
				++i;
			}
		}
	}

}

template<class T>
inline void QTree<T>::retrieve(std::vector<T*>& data, const BoundingBox& bbox)
{
	if (bound_.intersects(bbox, true)) {
		if (ne != nullptr) ne->retrieve(data, bbox);
		if (nw != nullptr) nw->retrieve(data, bbox);
		if (sw != nullptr) sw->retrieve(data, bbox);
		if (se != nullptr) se->retrieve(data, bbox);

		std::copy(objects_.begin(), objects_.end(), std::back_inserter(data));
	}

}

template<class T>
inline void QTree<T>::getBounds(std::vector<BoundingBox>& bounds)
{
	bounds.push_back(bound_);

	if (nw == nullptr) return;

	nw->getBounds(bounds);
	ne->getBounds(bounds);
	sw->getBounds(bounds);
	se->getBounds(bounds);
}
