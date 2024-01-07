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

	static void randomMotionTest(MyKineEngine& engine) {

		float borderThickness = 20;

		kine::Body* borderBox = engine.createBody(false);
		borderBox->aabb = {0, 0, 1280, borderThickness};
		((BodyData*)borderBox->userData)->isStatic = true;

		borderBox = engine.createBody(false);
		borderBox->aabb = {0, 720 - borderThickness, 1280, borderThickness};
		((BodyData*)borderBox->userData)->isStatic = true;

		borderBox = engine.createBody(false);
		borderBox->aabb = {0, 0, borderThickness, 720};
		((BodyData*)borderBox->userData)->isStatic = true;

		borderBox = engine.createBody(false);
		borderBox->aabb = {1280 - borderThickness, 0, borderThickness, 720};
		((BodyData*)borderBox->userData)->isStatic = true;

		srand(time(nullptr));

		float boxW = 20;
		float boxH = 20;

		for(int i = 1; i <= 20; i++) {

			kine::Body* box = engine.createBody(true);
			((BodyData*)box->userData)->isStatic = false;
			box->aabb = {(float)(rand() % (int)(1280 - (2 * borderThickness) - boxW)) + borderThickness,
						 (float)(rand() % (int)(720 - (2 * borderThickness) - boxH)) + borderThickness,
						 boxW, boxH};
			box->speed = {3 * ((float)((rand() % 400) - 200)), 3 * ((float)((rand() % 400) - 200))};

		}

	}

	static void stackTest(MyKineEngine& engine) {

		float borderThickness = 20;

		kine::Body* borderBox = engine.createBody(false);
		borderBox->aabb = {0, 0, 1280, borderThickness};
		((BodyData*)borderBox->userData)->isStatic = true;

		borderBox = engine.createBody(false);
		borderBox->aabb = {0, 720 - borderThickness, 1280, borderThickness};
		((BodyData*)borderBox->userData)->isStatic = true;

		borderBox = engine.createBody(false);
		borderBox->aabb = {0, 0, borderThickness, 720};
		((BodyData*)borderBox->userData)->isStatic = true;

		borderBox = engine.createBody(false);
		borderBox->aabb = {1280 - borderThickness, 0, borderThickness, 720};
		((BodyData*)borderBox->userData)->isStatic = true;

		float boxW = 20;
		float boxH = 20;

		for(float x = borderThickness; x <= (1280 - borderThickness - boxW); x += (2 * boxW)) {

			kine::Body* box = engine.createBody(true);
			box->aabb = {x, 600, boxW, boxH};
			box->speed = {60, 0};
			((BodyData*)box->userData)->isStatic = false;

		}

	}

	static void run() {

		SDLOO::Window window {"First", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
			1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE};
		SDLOO::Renderer renderer {window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC, 
								  1600, 900};
		renderer.setCameraPosition(-(1600-1280)/2, -(900-720)/2);

		Timer timer;
		MyKineEngine kineEngine;

		bool mouseDragging = false;

		bool paused = true;

		stackTest(kineEngine);

		while(running) {

			float deltaTime = timer.elapsed();

			SDL_Event ev;
			while(SDL_PollEvent(&ev)) {

				switch(ev.type) {

					case SDL_WINDOWEVENT :

						if(ev.window.event == SDL_WINDOWEVENT_RESIZED) renderer.updateView();
						break;

					case SDL_QUIT :

						setRunning(false);
						break;

					case SDL_MOUSEWHEEL :

						renderer.scaleCameraBy(expf(-(float)ev.wheel.y / 10));
						break;

					case SDL_MOUSEBUTTONDOWN :

						mouseDragging = true;
						break;

					case SDL_MOUSEBUTTONUP :

						mouseDragging = false;
						break;

					case SDL_MOUSEMOTION :

						if(mouseDragging)
							renderer.moveCamera(-(float)ev.motion.xrel, -(float)ev.motion.yrel);
						break;

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