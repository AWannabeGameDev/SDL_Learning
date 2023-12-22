#ifndef PLAYER_H
#define PLAYER_H

#include <sdl_util/sdloo.h>
#include <sdl_util/keyboard_input.h>
#include <sdl_util/userdata.h>
#include <cle/cle_include.h>

class Player {

	friend class MyCollisionListener;

private :

	typedef KeyboardInput key;

	cle::Collider* m_collider;
	SDLOO::Texture m_tex;
	bool m_facingRight;

	const float m_width, m_height;
	const float m_renderOffsetX, m_renderOffsetY;

	float m_xvel;
	const float m_maxXspeed;

	float m_yvel;
	const float m_jumpImpulse;
	const float m_gravityChangeThreshold, m_downVelCap;
	const float m_upGravity, m_downGravity;

	bool m_grounded;
	unsigned int m_groundCount;

	UserData m_userData;

	float f_clamp(float f, float min, float max);

public :
	
	Player(float x, float y, cle::CollisionListener& collisionListener, SDLOO::Renderer& renderer);

	void update(float dt);

	void render(SDLOO::Renderer& renderer);

	void centerCamera(SDLOO::Renderer& renderer, const SDL_FRect& border);

};

#endif