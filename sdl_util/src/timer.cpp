#include "sdl_util/timer.h"
#include <stdio.h>

Profiler::Profiler(const char* msg) :

	start(std::chrono::high_resolution_clock::now()), message(msg) {}

Profiler::~Profiler() {

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = end - start;

	printf("Time taken (%s) : %lli\n", message, duration.count());

}

Timer::Timer() :

	m_start(SDL_GetTicks64()) {}

float Timer::elapsed() {

	return (SDL_GetTicks64() - m_start) / 1000.f;

}

void Timer::reset() {

	m_start = SDL_GetTicks64();

}