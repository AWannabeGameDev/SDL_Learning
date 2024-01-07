#include "cle/cle_collision_listener.h"

using namespace cle;

size_t std::hash<std::pair<Collider*, Collider*>>::operator()(const std::pair<Collider*, Collider*>& pair) const noexcept {

	std::hash<Collider*> hasher;

	size_t seed = hasher(pair.first);
	size_t other = hasher(pair.second);
	seed ^= other + 0x9e3779b9 + (seed << 6) + (seed >> 2);

	return seed;

}

CollisionListener::CollisionListener() :

	m_resColliders{10}, m_nonResColliders{10} {}

Collider* CollisionListener::addCollider(bool isResolved) {

	Collider* ret =  (isResolved ? m_resColliders : m_nonResColliders).addInPlace();
	ret->m_isResolved = isResolved;
	return ret;

}

void CollisionListener::removeCollider(Collider* toRemove) {

	m_toBeRemoved.insert(toRemove);

}

Freelist<Collider>::Iterator cle::CollisionListener::resolvedBegin() {

	return m_resColliders.begin();

}

Freelist<Collider>::Iterator cle::CollisionListener::resolvedEnd() {

	return m_resColliders.end();

}

Freelist<Collider>::Iterator cle::CollisionListener::unresolvedBegin()
{
	return m_nonResColliders.begin();
}

Freelist<Collider>::Iterator cle::CollisionListener::unresolvedEnd() {

	return m_nonResColliders.end();

}

void CollisionListener::f_checkCollision(Collider* first, Collider* second) {

	if(m_toBeRemoved.contains(first) || m_toBeRemoved.contains(second)) return;

	CollisionPair collision {first, second};

	if(!m_overlaps.contains(collision)) {

		if(isOverlapping(first->aabb, second->aabb)) {

			m_overlaps.insert(collision);
			beginOverlap(first, second);

		}

	}

	if(!m_touches.contains(collision)) {

		if(isTouching(first->aabb, second->aabb)) {

			m_touches.insert(collision);
			beginTouch(first, second, calculateNormal(first->aabb, second->aabb));

		}

	}

}

void CollisionListener::checkCollisions() {

	// CHECK BEGIN COLLISIONS

	for(auto first = m_resColliders.begin(); first != m_resColliders.end(); first++) {

		for(auto second = first.next(); second != m_resColliders.end(); second++) {

			f_checkCollision(&*first, &*second);

		}

		for(auto second = m_nonResColliders.begin(); second != m_nonResColliders.end(); second++) {

			f_checkCollision(&*first, &*second);

		}

	}

	// CHECK END OVERLAPS

	for(auto it = m_overlaps.begin(); it != m_overlaps.end(); ) {

		bool overlapping = isOverlapping(it->first->aabb, it->second->aabb);
		bool removed = m_toBeRemoved.contains(it->first) || m_toBeRemoved.contains(it->second);

		if(!overlapping || removed) {

			endOverlap(it->first, it->second);
			it = m_overlaps.erase(it);

		} else ++it;

	}

	// CHECK END TOUCHES

	for(auto it = m_touches.begin(); it != m_touches.end(); ) {

		if(!isTouching(it->first->aabb, it->second->aabb)) {

			endTouch(it->first, it->second, 
					 calculateNormal(it->first->m_prevAABBCollide, it->second->m_prevAABBCollide));

			it = m_touches.erase(it);

		} else if(m_toBeRemoved.contains(it->first) || m_toBeRemoved.contains(it->second)) {

			endTouch(it->first, it->second, calculateNormal(it->first->aabb, it->second->aabb));
			it = m_touches.erase(it);

		} else ++it;

	}

	for(auto it = m_touches.begin(); it != m_touches.end(); ++it) {

		it->first->m_prevAABBCollide = it->first->aabb;
		it->second->m_prevAABBCollide = it->second->aabb;

	}

	for(Collider* toRemove : m_toBeRemoved) {

		(toRemove->m_isResolved ? m_resColliders : m_nonResColliders).remove(toRemove);

	}

	m_toBeRemoved.clear();

}