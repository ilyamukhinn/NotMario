#include "Physics.hpp"
#include "Components.hpp"

Vec2f Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
	if (a->has<CTransform>() &&
		b->has<CTransform>() &&
		a->has<CBoundingBox>() &&
		b->has<CBoundingBox>()) {
		return CalcOverlap(a->get<CTransform>().m_pos,
			a->get<CBoundingBox>().m_halfSize,
			b->get<CTransform>().m_pos,
			b->get<CBoundingBox>().m_halfSize);
	}

	return Vec2f(0.0f, 0.0f);
}

Vec2f Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
	if (a->has<CTransform>() &&
		b->has<CTransform>() &&
		a->has<CBoundingBox>() &&
		b->has<CBoundingBox>()) {
		return CalcOverlap(a->get<CTransform>().m_prevPos,
			a->get<CBoundingBox>().m_halfSize,
			b->get<CTransform>().m_prevPos,
			b->get<CBoundingBox>().m_halfSize);
	}

	return Vec2f(0.0f, 0.0f);
}

Vec2f Physics::CalcOverlap(const Vec2f& aPos, const Vec2f& aHalfSize,
	const Vec2f& bPos, const Vec2f& bHalfSize) {
	Vec2f delta = Vec2f(abs(aPos.x - bPos.x), abs(aPos.y - bPos.y));
	float oX = aHalfSize.x + bHalfSize.x - delta.x;
	float oY = aHalfSize.y + bHalfSize.y - delta.y;
	return Vec2f(oX, oY);
}