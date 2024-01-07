#ifndef KE_SHAPE_H
#define KE_SHAPE_H

namespace kine {

	struct Vector2 {

		float x;
		float y;

		Vector2 operator+(const Vector2& other) const;
		Vector2 operator-() const;
		Vector2 operator-(const Vector2& other) const;
		void operator+=(const Vector2& other);
		void operator-=(const Vector2& other);
		Vector2 operator*(float scale) const;

		float magnitude() const;
		float magnitudeSquared() const;

	};

	struct AABB2 {

		float x;
		float y;
		float w;
		float h;

		bool overlaps(const AABB2& other) const;
		Vector2 center() const;

	};

}

#endif