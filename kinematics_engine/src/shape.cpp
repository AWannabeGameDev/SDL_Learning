#include "kine/ke_shape.h"
#include <cmath>

using namespace kine;

bool AABB2::overlaps(const AABB2& other) const {

	return (x < (other.x + other.w)) && ((x + w) > other.x) &&
		   (y < (other.y + other.h)) && ((y + h) > other.y);

}

Vector2 AABB2::center() const {

	return {x - (w / 2), y - (h / 2)};

}

Vector2 Vector2::operator+(const Vector2& other)  const{

	return {x + other.x, y + other.y};

}

Vector2 Vector2::operator-() const {

	return {-x, -y};

}

Vector2 Vector2::operator-(const Vector2& other) const {

	return {x - other.x, y - other.y};

}

void Vector2::operator+=(const Vector2& other) {

	x += other.x;
	y += other.y;

}

void Vector2::operator-=(const Vector2& other) {

	x -= other.x;
	y -= other.y;

}

Vector2 Vector2::operator*(float scale) const {

	return {x * scale, y * scale};

}

float kine::Vector2::magnitude() const {

	return sqrtf((x * x) + (y * y));

}

float kine::Vector2::magnitudeSquared() const {

	return (x * x) + (y * y);

}