#ifndef KE_KINE_ENGINE_H
#define KE_KINE_ENGINE_H

#include "ke_unrolled_list.h"
#include "ke_body.h"
#include <unordered_map>

namespace kine {

	class KineEngine {

	public :

		KineEngine();

		virtual void beginCollide(Body* first, Body* second) = 0;
		virtual void endCollide(Body* first, Body* second) = 0;
		virtual void onResolve(Body* first, Body* second, 
							   Vector2 normal, float timeStep, float timeOfImpact) = 0;

		void step(float deltaTime);

		Body* createBody(bool isResolved);
		void removeBody(Body* body);

		UnrolledList<Body>::Iterator begin();
		UnrolledList<Body>::Iterator end();

	private :

		struct m_CollisionInfo {

			Body* other;
			float timeOfImpact;

		};

		float f_findCollisionTime(Body* first, Body* second, float deltaTime);
		float f_sgn(float num);
		Vector2 f_overlappingAxes(const AABB2& first, const AABB2& second); 
		void f_thickenAABB(AABB2& main, const AABB2& medium);

		float m_timeOfImpactSlop;

		UnrolledList<Body> m_bodies;
		std::unordered_map<Body*, m_CollisionInfo> m_collisions;
		
	};

}

#endif