#ifndef PHYSICS_H
#define PHYSICS_H

#include <memory>

#include "Entity.hpp"

struct Intersect {
	bool result;
	Vec2f pos;
};

namespace Physics {
	Vec2f GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
	Vec2f GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
	Vec2f CalcOverlap(const Vec2f& aPos, const Vec2f& aHalfSize,
		const Vec2f& bPos, const Vec2f& bHalfSize);
}

#endif // !PHYSICS_H
