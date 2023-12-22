#include "my_collision_listener.h"
#include <sdl_util/userdata.h>

void MyCollisionListener::beginTouch(cle::Collider* first, cle::Collider* second, cle::Vector2D normal) {

	if(f_getUserData(first)) {

		if(f_getUserData(first)->text == "player") {

			f_playerBeginTouch(first, second, normal);

		}

	} else if(f_getUserData(second)) {

		if(f_getUserData(second)->text == "player") {

			normal.x *= -1;
			normal.y *= -1;
			f_playerBeginTouch(second, first, normal);

		}

	}

}

void MyCollisionListener::beginOverlap(cle::Collider* first, cle::Collider* second) {

	cle::resolveCollision(first, second);

}

void MyCollisionListener::endTouch(cle::Collider* first, cle::Collider* second, cle::Vector2D normal) {

	if(f_getUserData(first)) {

		if(f_getUserData(first)->text == "player") {

			f_playerEndTouch(first, second, normal);

		}

	} else if(f_getUserData(second)) {

		if(f_getUserData(second)->text == "player") {

			normal.x *= -1;
			normal.y *= -1;
			f_playerEndTouch(second, first, normal);

		}

	}

}

void MyCollisionListener::endOverlap(cle::Collider* first, cle::Collider* second) {



}

UserData* MyCollisionListener::f_getUserData(cle::Collider* coll) {

	return (UserData*)coll->userData;
	
}

void MyCollisionListener::f_playerBeginTouch(cle::Collider* playerCol, cle::Collider* other, cle::Vector2D normal) {

	Player* player = (Player*)f_getUserData(playerCol)->ptr;

	if(normal.y > 0) {

		player->m_grounded = true;
		player->m_groundCount++;

	}

}

void MyCollisionListener::f_playerEndTouch(cle::Collider* playerCol, cle::Collider* other, cle::Vector2D normal) {

	Player* player = (Player*)f_getUserData(playerCol)->ptr;

	if((normal.y > 0) && player->m_groundCount > 0) {

		if(--player->m_groundCount == 0) player->m_grounded = false;

	}

}