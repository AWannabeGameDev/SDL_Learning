#ifndef MY_COLLISION_LISTENER_H
#define MY_COLLISION_LISTENER_H

#include <cle/cle_collision_listener.h>
#include "../entities/player.h"
#include <sdl_util/userdata.h>

class MyCollisionListener : public cle::CollisionListener {

public :

	virtual void beginTouch(cle::Collider* first, cle::Collider* second, cle::Vector2D normal) override;
	virtual void beginOverlap(cle::Collider* first, cle::Collider* second) override;
	virtual void endTouch(cle::Collider* first, cle::Collider* second, cle::Vector2D normal) override;
	virtual void endOverlap(cle::Collider* first, cle::Collider* second) override;

	UserData* f_getUserData(cle::Collider* coll);

private :

	void f_playerBeginTouch(cle::Collider* player, cle::Collider* other, cle::Vector2D normal);
	void f_playerEndTouch(cle::Collider* player, cle::Collider* other, cle::Vector2D normal);

};

#endif