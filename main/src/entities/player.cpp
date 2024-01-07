#include "player.h"
#include "../helper/constants.h"
#include <sdl_util/userdata.h>

Player::Player(float x, float y, cle::CollisionListener& collisionListener, SDLOO::Renderer& renderer) :

	m_collider(collisionListener.addCollider(true)), m_tex{"player.png", renderer}, m_facingRight(false),
	m_width(16), m_height(16),
	m_renderOffsetX(0), m_renderOffsetY(0),
	m_xvel(0.f), m_maxXspeed(100.f), 
	m_yvel(0.f), m_jumpImpulse(300.f), m_gravityChangeThreshold(m_jumpImpulse * 1/2), m_downVelCap(m_jumpImpulse * 3/2),
	m_upGravity(720.f), m_downGravity(m_upGravity * 2), 
	m_grounded(false), m_groundCount(0),
	m_userData{"player", this}

{

	m_collider->aabb = {x, y, m_width, m_height};
	m_collider->userData = &m_userData;

}

void Player::centerCamera(SDLOO::Renderer& renderer, const SDL_FRect& border) {

	const SDL_FRect& cam = renderer.camera();
	float x = m_collider->aabb.x - ((cam.w - m_width) / 2);
	float y = m_collider->aabb.y - ((cam.h - m_height) / 2);

	if(border.w < cam.w)						 x = border.x - ((cam.w - border.w) / 2);
	else if(x < border.x)						 x = border.x;
	else if((x + cam.w) > (border.x + border.w)) x = border.x + border.w - cam.w;

	if(border.h < cam.h)						 y = border.y - ((cam.h - border.h) / 2);
	else if(y < border.y)						 y = border.y;
	else if((y + cam.h) > (border.y + border.h)) y = border.y + border.h - cam.h;

	renderer.setCameraPosition(x, y);

}

float Player::f_clamp(float f, float min, float max) {

	if(min > f) return min;
	else if(max < f) return max;
	else return f;

}

void Player::update(float dt) {

	float dx = 0.f, dy = 0.f;

	if(key::isPressed("KEY_RIGHT") && !key::isPressed("KEY_LEFT")) {

		m_xvel = m_maxXspeed;
		m_facingRight = true;

	} else if(!key::isPressed("KEY_RIGHT") && key::isPressed("KEY_LEFT")) {

		m_xvel = -m_maxXspeed;
		m_facingRight = false;

	} else m_xvel = 0;

	if(m_grounded) {

		if(key::isJustPressed("KEY_JUMP")) {

			m_yvel = -m_jumpImpulse;

		} else {

			m_yvel = 0.f;

		}

	} else {

		if(m_yvel < -m_gravityChangeThreshold) m_yvel += m_upGravity * dt;
		else if(m_yvel < m_downVelCap) m_yvel += m_downGravity * dt;
		else m_yvel = m_downVelCap;

	}

	dx = m_xvel * dt;
	dy = m_yvel * dt;

	m_collider->aabb.x += dx;
	m_collider->aabb.y += dy;

	//std::cout << "Player grounded : " << m_grounded << '\n';

}

void Player::render(SDLOO::Renderer& renderer) {

	SDL_FRect rect {m_collider->aabb.x + (!m_facingRight? m_renderOffsetX : m_width - m_tex.width() - m_renderOffsetX),
					m_collider->aabb.y + m_renderOffsetY, 
					(float)m_tex.width(), (float)m_tex.height()};

	renderer.renderCopyEx(m_tex, rect, 0.f, {}, m_facingRight? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);

	//renderer.drawRect({m_body->GetPosition().x, m_body->GetPosition().y, m_width, m_height});

}