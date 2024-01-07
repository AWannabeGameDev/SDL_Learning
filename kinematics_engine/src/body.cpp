#include "kine/ke_body.h"

using namespace kine;

bool Body::isResolved() const {

	return m_isResolved;

}

Vector2 kine::Body::velocity() const {

	return m_velocity;

}