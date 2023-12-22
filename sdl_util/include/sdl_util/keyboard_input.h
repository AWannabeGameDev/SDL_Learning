#ifndef KEYBOARD_INPUT_H
#define KEYBOARD_INPUT_H

#include <sdl/SDL.h>
#include <unordered_map>

class KeyboardInput {

private :

	struct KeyPressedStatus {

		bool prev;
		bool current;

	};

	inline static std::unordered_map<const char*, Uint8> m_keymap;
	inline static std::unordered_map<Uint8, KeyPressedStatus> m_state;

public :

	static void addKeybind(const char* name, Uint8 keycode);
	static void updateState();

	static bool isPressed(const char* key);
	static bool isJustPressed(const char* key);
	static bool isReleased(const char* key);
	static bool isJustReleased(const char* key);

};

#endif