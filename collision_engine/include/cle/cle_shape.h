#ifndef CLE_AABB_H
#define CLE_AABB_H

namespace cle {

	struct AABB {

		float x, y;
		float w, h;

	};

	struct Vector2D {

		float x, y;

	};

	bool isOverlapping(const AABB& first, const AABB& second);
	bool isTouching(const AABB& first, const AABB& second);
	Vector2D calculateNormal(const AABB& first, const AABB& second);

	AABB thicken(const AABB& main, const AABB& medium);
	bool pointOnAABB(float x, float y, const AABB& aabb);
	void normalizeVector(Vector2D& vec);

}

#endif