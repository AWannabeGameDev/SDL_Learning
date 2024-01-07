#ifndef KE_BODY_H
#define KE_BODY_H

#include "ke_shape.h"

namespace kine {

	class Body {

		friend class KineEngine;

	public :

		AABB2 aabb;
		Vector2 speed;
		Vector2 prevPosition;

		void* userData;

		bool isResolved() const;
		Vector2 velocity() const;

	private :

		Vector2 m_velocity;

		bool m_isResolved;
		class KineEngine* m_engine;
		
	};

}

#endif