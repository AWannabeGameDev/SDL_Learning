#ifndef MY_KINE_ENGINE_H
#define MY_KINE_ENGINE_H

#include <kine/ke_kine_engine.h>
#include "body_data.h"

class MyKineEngine : public kine::KineEngine {

private :

	kine::UnrolledList<BodyData> m_bodyData;

public :

	MyKineEngine();

	kine::Body* createBody(bool isResolved);

	void beginCollide(kine::Body* first, kine::Body* second) override;
	void endCollide(kine::Body* first, kine::Body* second) override;
	void onResolve(kine::Body* first, kine::Body* second, 
				   kine::Vector2 normal, float timeStep, float timeOfImpact) override;

};

#endif