#ifndef CLE_COLLISION_LISTENER_H
#define CLE_COLLISION_LISTENER_H

#include "cle_collider.h"
#include "cle_freelist.h"
#include <unordered_set>

template<>
struct std::hash<std::pair<cle::Collider*, cle::Collider*>> {

	size_t operator()(const std::pair<cle::Collider*, cle::Collider*>& pair) const noexcept;

};

namespace cle {

	class CollisionListener {

	private:

		typedef std::pair<Collider*, Collider*> CollisionPair;

		Freelist<Collider> m_resColliders, m_nonResColliders;
		std::unordered_set<CollisionPair> m_touches, m_overlaps;
		std::unordered_set<Collider*> m_toBeRemoved;

		void f_checkCollision(Collider* first, Collider* second);

	public :

		CollisionListener();

		void checkCollisions();

		Collider* addCollider(bool isResolved);
		void removeCollider(Collider* toRemove); 

		virtual void beginTouch(Collider* first, Collider* second, Vector2D normal) = 0;
		virtual void beginOverlap(Collider* first, Collider* second) = 0;
		virtual void endTouch(Collider* first, Collider* second, Vector2D normal) = 0; 
		virtual void endOverlap(Collider* first, Collider* second) = 0;

		Freelist<Collider>::Iterator resolvedBegin();
		Freelist<Collider>::Iterator resolvedEnd();
		Freelist<Collider>::Iterator unresolvedBegin();
		Freelist<Collider>::Iterator unresolvedEnd();

	};

}

#endif