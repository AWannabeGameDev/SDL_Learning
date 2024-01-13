#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <random>
#include <sdl/SDL.h>
#include <sdl_util/sdloo.h>
#include <tilemap/tilemap_include.h>
#include <sdl_util/timer.h>
#include "helper/constants.h"
//#include "entities/player.h"
#include <sdl_util/keyboard_input.h>
//#include "helper/my_collision_listener.h"
#include "helper/my_kine_engine.h"
#include "helper/body_data.h"

//FIX THE TILE RENDER GLITCH LINES

class GameLoop {

private:

	inline static bool running;

public:

	static void setRunning(bool run) {

		running = run;

	}

	static void init() {

		SDL_Init(SDL_INIT_VIDEO);
		IMG_Init(IMG_INIT_PNG);

		KeyboardInput::addKeybind("SIM_PAUSE_RESUME", SDL_SCANCODE_SPACE);
		KeyboardInput::addKeybind("SIM_STEP", SDL_SCANCODE_RIGHT);

		running = true;

	}

	// works quite well with bouncing, some boxes get pushed put when no bouncing
	static void randomMotionTest(MyKineEngine& engine) {

		float borderThickness = 2;
		float borderW = 128;
		float borderH = 72;

		kine::Body* borderBox = engine.createBody(false);
		borderBox->aabb = {0, 0, borderW, borderThickness};
		((BodyData*)borderBox->userData)->isStatic = true;

		borderBox = engine.createBody(false);
		borderBox->aabb = {0, borderH - borderThickness, borderW, borderThickness};
		((BodyData*)borderBox->userData)->isStatic = true;

		borderBox = engine.createBody(false);
		borderBox->aabb = {0, 0, borderThickness, borderH};
		((BodyData*)borderBox->userData)->isStatic = true;

		borderBox = engine.createBody(false);
		borderBox->aabb = {borderW - borderThickness, 0, borderThickness, borderH};
		((BodyData*)borderBox->userData)->isStatic = true;

		srand(time(nullptr));

		float boxW = 1;
		float boxH = 1;
		float boxMaxSpeed = 70;
		float numBoxes = 80;

		for(int i = 1; i <= numBoxes; i++) {

			kine::Body* box = engine.createBody(true);
			((BodyData*)box->userData)->isStatic = false;
			box->aabb = {(float)(rand() % (int)(borderW - (2 * borderThickness) - boxW)) + borderThickness,
						 (float)(rand() % (int)(borderH - (2 * borderThickness) - boxH)) + borderThickness,
						 boxW, boxH};
			box->speed = {3 * ((float)((rand() % (int)((2.f/3.f) * boxMaxSpeed)) - (boxMaxSpeed / 3))), 
						  3 * ((float)((rand() % (int)((2.f/3.f) * boxMaxSpeed)) - (boxMaxSpeed / 3)))};

		}

	}

	// doesnt work at all
	static void stackTest(MyKineEngine& engine) {

		float borderThickness = 2;
		float borderW = 128;
		float borderH = 72;

		kine::Body* borderBox = engine.createBody(false);
		borderBox->aabb = {0, 0, borderW, borderThickness};
		((BodyData*)borderBox->userData)->isStatic = true;

		borderBox = engine.createBody(false);
		borderBox->aabb = {0, borderH - borderThickness, borderW, borderThickness};
		((BodyData*)borderBox->userData)->isStatic = true;

		borderBox = engine.createBody(false);
		borderBox->aabb = {0, 0, borderThickness, borderH};
		((BodyData*)borderBox->userData)->isStatic = true;

		borderBox = engine.createBody(false);
		borderBox->aabb = {borderW - borderThickness, 0, borderThickness, borderH};
		((BodyData*)borderBox->userData)->isStatic = true;

		float boxW = 2;
		float boxH = 2;

		for(float x = borderThickness; x <= (borderW - borderThickness - boxW); x += (2 * boxW)) {

			kine::Body* box = engine.createBody(true);
			box->aabb = {x, 60, boxW, boxH};
			box->speed = {6, 0};
			((BodyData*)box->userData)->isStatic = false;

		}

	}

	// works perfectly
	static void headOnCollisionTest(MyKineEngine& engine) {

		float boxW = 4;
		float boxH = 4;
		float boxVel = 10;
		float boxSep = 40;

		kine::Body* box = engine.createBody(false);
		box->aabb = {-boxSep / 2, 0, boxW, boxH};
		box->speed = {boxVel / 2, 0};

		box = engine.createBody(true);
		box->aabb = {boxSep / 2, 0, boxW, boxH};
		box->speed = {-boxVel, 0};

	}

	// works good with bouncing, slightly unstable without bouncing but overall is pseudo-stable
	static void multiBodyCollisionTest(MyKineEngine& engine) {

		float boxW = 4;
		float boxH = 4;
		float boxVel = 12;
		
		kine::Body* box = engine.createBody(false);
		box->aabb = {-boxW / 2, -20 - boxH, boxW, boxH};
		box->speed = {0, boxVel};

		box = engine.createBody(true);
		box->aabb = {16, 12, boxW, boxH};
		box->speed = {-boxVel * 4.f/5.f, -boxVel * 3.f/5.f};

		box = engine.createBody(true);
		box->aabb = {-16 - boxW, 12, boxW, boxH};
		box->speed = {boxVel * 4.f/5.f, -boxVel* 3.f/5.f};

	}

	static void run() {

		int windowW = 1280;
		int windowH = 720;
		float cameraW = 160;
		float cameraH = 90;

		SDLOO::Window window {"First", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
			windowW, windowH, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE};
		SDLOO::Renderer renderer {window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC, 
								  160, 90};
		renderer.setCameraPosition(0, 0);

		Timer timer;
		MyKineEngine kineEngine;

		bool mouseDragging = false;

		bool paused = true;

		multiBodyCollisionTest(kineEngine);

		while(running) {

			float deltaTime = timer.elapsed();

			SDL_Event ev;
			while(SDL_PollEvent(&ev)) {

				switch(ev.type) {

					case SDL_WINDOWEVENT: {

						if(ev.window.event == SDL_WINDOWEVENT_RESIZED) renderer.updateView();
						break;

					} case SDL_QUIT: {

						setRunning(false);
						break;

					} case SDL_MOUSEWHEEL: {

						const SDL_FRect& cam = renderer.camera();
						kine::Vector2 camCenter{cam.x + (cam.w / 2), cam.y + (cam.h / 2)};

						renderer.scaleCameraBy(expf(-(float)ev.wheel.y / 10));
						renderer.setCameraPosition(camCenter.x - (cam.w / 2), camCenter.y - (cam.h / 2));

						break;

					} case SDL_MOUSEBUTTONDOWN: {

						mouseDragging = true;
						break;

					} case SDL_MOUSEBUTTONUP: {

						mouseDragging = false;
						break;

					} case SDL_MOUSEMOTION: {

						if(mouseDragging)
							renderer.moveCamera(-(float)ev.motion.xrel * (cameraW / windowW),
												-(float)ev.motion.yrel * (cameraH / windowH));
						break;

					}

				}

			}

			KeyboardInput::updateState();

			if(KeyboardInput::isJustPressed("SIM_PAUSE_RESUME"))
				paused = !paused;

			if(paused) {

				if(KeyboardInput::isJustPressed("SIM_STEP"))
					kineEngine.step(deltaTime);

			} else kineEngine.step(deltaTime);

			// RENDER -----------------

			timer.reset();
			renderer.clear();

			renderer.setDrawColor(255, 0, 0, 255);
			for(auto& body : kineEngine) {

				SDL_FRect rect {body.aabb.x, body.aabb.y, body.aabb.w, body.aabb.h};
				renderer.drawRect(rect);

			}

			renderer.renderPresent();

		}

	}

	static void close() {

		SDL_Quit();
		IMG_Quit();

	}

};

#endif