#include <sdl/SDL.h>
#include <chrono>

class Profiler {

private:

	std::chrono::time_point<std::chrono::steady_clock> start;
	const char* message;

public:

	Profiler(const char* msg);

	~Profiler();

};

class Timer {

private :

	Uint64 m_start;

public :

	Timer();

	float elapsed();

	void reset();

};