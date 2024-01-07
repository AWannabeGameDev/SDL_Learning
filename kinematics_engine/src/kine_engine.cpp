#include "kine/ke_kine_engine.h"
#include <stdio.h>
#include <mutex>

using namespace kine;

KineEngine::KineEngine() :
	
	m_timeOfImpactSlop(0.98f), m_bodies{10}, m_collisions{} {}

Body* KineEngine::createBody(bool isResolved) {

	auto ret = m_bodies.addInPlace();
	ret->m_isResolved = isResolved;
	ret->m_engine = this;

	return &*ret;

}

void kine::KineEngine::removeBody(Body* body) {

	m_bodies.remove(m_bodies.getIteratorTo(body));

}

UnrolledList<Body>::Iterator kine::KineEngine::begin() {

	return m_bodies.begin();

}

UnrolledList<Body>::Iterator kine::KineEngine::end() {

	return m_bodies.end();

}

void KineEngine::step(float deltaTime) {

	m_collisions.reserve(m_bodies.size());

	for(Body& body : m_bodies) {

		body.prevPosition = {body.aabb.x, body.aabb.y};
		body.aabb.x += body.speed.x * deltaTime;
		body.aabb.y += body.speed.y * deltaTime;
		
	}

	for(auto body = m_bodies.begin(); body != m_bodies.end(); ++body) {

		if(body->isResolved() == false) continue;

		m_CollisionInfo newInfo {nullptr, std::numeric_limits<float>::max()};

		for(auto other = m_bodies.begin(); other != m_bodies.end(); ++other) {

			if(body == other) continue;

			if(body->aabb.overlaps(other->aabb)) {

				float toi = f_findCollisionTime(&*body, &*other, deltaTime);

				if(toi < newInfo.timeOfImpact) {

					newInfo.other = &*other;
					newInfo.timeOfImpact = toi;

				}

			}

		}

		if(newInfo.other != nullptr) m_collisions[&*body] = newInfo;

	}

	for(auto it = m_collisions.begin(); it != m_collisions.end(); ++it) {

		Body* body = it->first;
		m_CollisionInfo& info = it->second;

		body->aabb.x -= body->speed.x * (deltaTime - info.timeOfImpact);
		body->aabb.y -= body->speed.y * (deltaTime - info.timeOfImpact);

		if(info.other->isResolved() == true) {

			info.other->aabb.x -= info.other->speed.x * (deltaTime - info.timeOfImpact);
			info.other->aabb.y -= info.other->speed.y * (deltaTime - info.timeOfImpact);

			if(m_collisions.contains(info.other)) {

				m_CollisionInfo& otherInfo = m_collisions.at(info.other);
				if(otherInfo.other == body) m_collisions.erase(info.other);

			}

		}

		Vector2 overlap = f_overlappingAxes(body->aabb, info.other->aabb);
		Vector2 normal {f_sgn(info.other->aabb.x - body->aabb.x) * overlap.y, 
						f_sgn(info.other->aabb.y - body->aabb.y) * overlap.x};

		onResolve(body, info.other, normal, deltaTime, info.timeOfImpact);

	}

	for(Body& body : m_bodies) 
		body.m_velocity = Vector2{body.aabb.x - body.prevPosition.x, body.aabb.y - body.prevPosition.y} * (1 / deltaTime);

	m_collisions.clear();

}

float kine::KineEngine::f_sgn(float num) {

	if(num > 0) return 1.f;
	else if(num == 0) return 0.f;
	else return -1.f;

}

Vector2 KineEngine::f_overlappingAxes(const AABB2& first, const AABB2& second) {

	return {(float)((first.x < (second.x + second.w)) && ((first.x + first.w) > second.x)),
			(float)((first.y < (second.y + second.h)) && ((first.y + first.h) > second.y))};

}

void kine::KineEngine::f_thickenAABB(AABB2& main, const AABB2& medium) {

	main.x -= medium.w / 2;
	main.y -= medium.h / 2;
	main.w += medium.w;
	main.h += medium.h;

}

float KineEngine::f_findCollisionTime(Body* first, Body* second, float deltaTime) {

	AABB2 aabb;
	Vector2 point;
	Vector2 currentPoint;
	Vector2 pointVelocity;

	if(first->isResolved() == false) {

		aabb = first->aabb;
		f_thickenAABB(aabb, second->aabb);

		point = {second->prevPosition.x + (second->aabb.w / 2), second->prevPosition.y + (second->aabb.h / 2)};
		currentPoint = second->aabb.center();

		pointVelocity = second->speed - first->speed;
		if(pointVelocity.magnitudeSquared() == 0) pointVelocity = second->m_velocity - first->m_velocity;

	} else if(second->isResolved() == false) {

		aabb = second->aabb;
		f_thickenAABB(aabb, first->aabb);

		point = {first->prevPosition.x + (first->aabb.w / 2), first->prevPosition.y + (first->aabb.h / 2)};
		currentPoint = first->aabb.center();

		pointVelocity = first->speed - second->speed;
		if(pointVelocity.magnitudeSquared() == 0) pointVelocity = first->m_velocity - second->m_velocity;

	} else {
		
		aabb = {first->prevPosition.x, first->prevPosition.y, first->aabb.w, first->aabb.h};
		f_thickenAABB(aabb, second->aabb);

		point = {second->prevPosition.x + (second->aabb.w / 2), second->prevPosition.y + (second->aabb.h / 2)};
		currentPoint = second->aabb.center();

		pointVelocity = second->speed - first->speed;
		if(pointVelocity.magnitudeSquared() == 0) pointVelocity = second->m_velocity - first->m_velocity;

	}

	Vector2 candidateLines {-f_sgn(pointVelocity.x), -f_sgn(pointVelocity.y)};
	Vector2 xpoi, ypoi, poi;

	if(candidateLines.x == 1)
		xpoi = {aabb.x + aabb.w, ((pointVelocity.y / pointVelocity.x) * (aabb.x + aabb.w - point.x)) + point.y};

	else if(candidateLines.x == -1)
		xpoi = {aabb.x, ((pointVelocity.y / pointVelocity.x) * (aabb.x - point.x)) + point.y};

	else
		xpoi = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};

	if(candidateLines.y == 1)
		ypoi = {((pointVelocity.x / pointVelocity.y) * (aabb.y + aabb.h - point.y)) + point.x, aabb.y + aabb.h};

	else if(candidateLines.y == -1)
		ypoi = {((pointVelocity.x / pointVelocity.y) * (aabb.y - point.y)) + point.x, aabb.y};

	else
		ypoi = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};

	if((xpoi.y > (aabb.y + aabb.h)) || (xpoi.y < aabb.y)) poi = ypoi;
	else poi = xpoi;

	float origDistSq = (currentPoint - point).magnitudeSquared();
	float collisionDistSq = (point - poi).magnitudeSquared();
	float toi = deltaTime * sqrtf(collisionDistSq / origDistSq);

	return toi * m_timeOfImpactSlop;

}