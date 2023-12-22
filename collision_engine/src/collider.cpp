#include "cle/cle_collider.h"

using namespace cle;

bool Collider::isResolved() {

	return m_isResolved;

}

void cle::resolveCollision(Collider* first, Collider* second) {

	static auto calculateResolution = [](const AABB& thick, float xcenter, float ycenter, float& xres, float& yres) {

		float thickXcenter = thick.x + (thick.w / 2);
		float thickYcenter = thick.y + (thick.h / 2);

		if(xcenter > thickXcenter) {

			if(ycenter > thickYcenter) {

				if((thick.x + thick.w - xcenter) < (thick.y + thick.h - ycenter)) {

					xres = thick.x + thick.w;
					yres = ycenter;

				} else {

					xres = xcenter;
					yres = thick.y + thick.h;

				}

			} else {

				if((thick.x + thick.w - xcenter) < (ycenter - thick.y)) {

					xres = thick.x + thick.w;
					yres = ycenter;

				} else {

					xres = xcenter;
					yres = thick.y;

				}

			}

		} else {

			if(ycenter > thickYcenter) {

				if((xcenter - thick.x) < (thick.y + thick.h - ycenter)) {

					xres = thick.x;
					yres = ycenter;

				} else {

					xres = xcenter;
					yres = thick.y + thick.h;

				}

			} else {

				if((xcenter - thick.x) < (ycenter - thick.y)) {

					xres = thick.x;
					yres = ycenter;

				} else {

					xres = xcenter;
					yres = thick.y;

				}

			}

		}

	};

	if(first->m_isResolved && !second->m_isResolved) {

		float xcenter = first->aabb.x + (first->aabb.w / 2);
		float ycenter = first->aabb.y + (first->aabb.h / 2);

		float xres, yres;
		calculateResolution(thicken(second->aabb, first->aabb), xcenter, ycenter, xres, yres);

		first->aabb.x = xres - (first->aabb.w / 2);
		first->aabb.y = yres - (first->aabb.h / 2);

	} else if(!first->m_isResolved && second->m_isResolved) {

		float xcenter = second->aabb.x + (second->aabb.w / 2);
		float ycenter = second->aabb.y + (second->aabb.h / 2);

		float xres, yres;
		calculateResolution(thicken(first->aabb, second->aabb), xcenter, ycenter, xres, yres);

		second->aabb.x = xres - (second->aabb.w / 2);
		second->aabb.y = yres - (second->aabb.h / 2);

	} else if(first->m_isResolved && second->m_isResolved) {

		float xcenter1 = first->aabb.x + (first->aabb.w / 2);
		float ycenter1 = first->aabb.y + (first->aabb.h / 2);

		float xres1, yres1;

		float xcenter2 = second->aabb.x + (second->aabb.w / 2);
		float ycenter2 = second->aabb.y + (second->aabb.h / 2);

		float xres2, yres2;

		calculateResolution(thicken(second->aabb, first->aabb), xcenter1, ycenter1, xres1, yres1);
		calculateResolution(thicken(first->aabb, second->aabb), xcenter2, ycenter2, xres2, yres2);
		xres1 = (xres1 + xcenter1) / 2;
		yres1 = (yres1 + ycenter1) / 2;
		xres2 = (xres2 + xcenter2) / 2;
		yres2 = (yres2 + ycenter2) / 2;

		first->aabb.x = xres1 - (first->aabb.w / 2);
		first->aabb.y = yres1 - (first->aabb.h / 2);
		second->aabb.x = xres2 - (second->aabb.w / 2);
		second->aabb.y = yres2 - (second->aabb.h / 2);

	}

}