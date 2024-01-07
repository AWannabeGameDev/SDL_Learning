#include "my_kine_engine.h"

MyKineEngine::MyKineEngine() :

	m_bodyData{10} {}

kine::Body* MyKineEngine::createBody(bool isResolved) {

	BodyData* data = &*m_bodyData.addInPlace();
	kine::Body* body = KineEngine::createBody(isResolved);
	body->userData = data;

	return body;

}

void MyKineEngine::beginCollide(kine::Body* first, kine::Body* second) {



}

void MyKineEngine::endCollide(kine::Body* first, kine::Body* second) {



}

void MyKineEngine::onResolve(kine::Body* first, kine::Body* second, 
							 kine::Vector2 normal, float timeStep, float timeOfImpact) {

#if 1

	BodyData* firstData = (BodyData*)first->userData;
	BodyData* secondData = (BodyData*)second->userData;

	if(firstData->isStatic == true) {

		if(normal.x) second->speed.x *= -1;
		else second->speed.y *= -1;

	} else if(secondData->isStatic == true) {

		if(normal.x) first->speed.x *= -1;
		else first->speed.y *= -1;

	} else {

		if(normal.x) std::swap(first->speed.x, second->speed.x);
		std::swap(first->speed.y, second->speed.y);

	}

#endif

}