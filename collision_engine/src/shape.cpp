#include "cle/cle_shape.h"
#include <cmath>

using namespace cle;

bool cle::isOverlapping(const AABB& first, const AABB& second) {

	return (first.x < (second.x + second.w)) && ((first.x + first.w) > second.x) &&
		   (first.y < (second.y + second.h)) && ((first.y + first.h) > second.y);

}

bool cle::isTouching(const AABB& first, const AABB& second) {

	return pointOnAABB(first.x + (first.w / 2), first.y + (first.h / 2), thicken(second, first));

}

AABB cle::thicken(const AABB& main, const AABB& medium) {

	return {main.x - (medium.w / 2), main.y - (medium.h / 2), main.w + medium.w, main.h + medium.h};

}

Vector2D cle::calculateNormal(const AABB& first, const AABB& second) {

	AABB thick = thicken(first, second);
	float x = second.x + (second.w / 2);
	float y = second.y + (second.h / 2);
	float thickXcenter = thick.x + (thick.w / 2);
	float thickYcenter = thick.y + (thick.h / 2);

	bool xoverlapping = (x > thick.x) && (x < (thick.x + thick.w));
	bool yoverlapping = (y > thick.y) && (y < (thick.y + thick.h));

	if(xoverlapping) {

		if(yoverlapping) {

			float xdist1 = fabsf(x - thick.x);
			float xdist2 = fabsf(x - (thick.x + thick.w));
			float xdist = xdist1 < xdist2 ? xdist1 : xdist2;

			float ydist1 = fabsf(y - thick.y);
			float ydist2 = fabsf(y - (thick.y + thick.h));
			float ydist = ydist1 < ydist2 ? ydist1 : ydist2;

			Vector2D normal{};

			if(xdist < ydist) normal.x = (x > thickXcenter ? 1.f : -1.f);
			else if(xdist > ydist) normal.y = (y > thickYcenter ? 1.f : -1.f);
			else {

				normal = {x > thickXcenter ? xdist : -xdist, y > thickYcenter ? ydist : -ydist};
				normalizeVector(normal);

			}

			return normal;

		} else return (y > thickYcenter ? Vector2D{0, 1.f} : Vector2D{0, -1.f});

	} else {

		if(yoverlapping) return (x > thickXcenter ? Vector2D{1.f, 0} : Vector2D{-1.f, 0});

		else {

			float xdist1 = x - thick.x;
			float xdist2 = x - (thick.x + thick.w);
			float xdist = fabsf(xdist1) < fabsf(xdist2) ? xdist1 : xdist2;

			float ydist1 = y - thick.y;
			float ydist2 = y - (thick.y + thick.h);
			float ydist = fabsf(ydist1) < fabsf(ydist2) ? ydist1 : ydist2;

			Vector2D normal{xdist, ydist};
			normalizeVector(normal);
			return normal;

		}

	}
	
}

bool cle::pointOnAABB(float x, float y, const AABB& aabb) {

	if((x == aabb.x) || (x == (aabb.x + aabb.w))) return (y >= aabb.y) && (y <= (aabb.y + aabb.h));
	else if((y == aabb.y) || (y == (aabb.y + aabb.h))) return (x >= aabb.x) && (x <= (aabb.x + aabb.w));
	else return false;
	
}

void cle::normalizeVector(Vector2D& vec) {

	float magnitude = sqrtf((vec.x * vec.x) + (vec.y * vec.y));
	vec.x /= magnitude;
	vec.y /= magnitude;

}
