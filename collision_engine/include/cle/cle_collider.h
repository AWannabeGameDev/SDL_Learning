#ifndef CLE_COLLIDER_H
#define CLE_COLLIDER_H

#include "cle_shape.h"

namespace cle {

	class Collider {

		friend class CollisionListener;
		friend void resolveCollision(Collider* first, Collider* second);

	public :

		AABB aabb;
		void* userData;

		bool isResolved();
		
	private :

		bool m_isResolved;
		AABB m_prevAABBCollide;

	};

	void resolveCollision(Collider* first, Collider* second);
}

#endif