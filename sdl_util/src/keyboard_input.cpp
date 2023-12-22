#include "sdl_util/keyboard_input.h"

void KeyboardInput::addKeybind(const char* name, Uint8 keycode) {

	m_keymap.insert({name, keycode});
	m_state.insert({keycode, {false, false}});

}

void KeyboardInput::updateState() {

	for(auto& [_, keycode] : m_keymap) {

		const Uint8* keyStates = SDL_GetKeyboardState(nullptr);
		KeyPressedStatus& status = m_state[keycode];

		status.prev = status.current;
		status.current = keyStates[keycode];

	}

}

bool KeyboardInput::isPressed(const char* key) {

	return m_state[m_keymap[key]].current;

}

bool KeyboardInput::isJustPressed(const char* key) {

	KeyPressedStatus& keyStatus = m_state[m_keymap[key]];
	return !keyStatus.prev && keyStatus.current;

}

bool KeyboardInput::isReleased(const char* key) {

	return !m_state[m_keymap[key]].current;

}

bool KeyboardInput::isJustReleased(const char* key) {

	KeyPressedStatus& keyStatus = m_state[m_keymap[key]];
	return keyStatus.prev && !keyStatus.current;

}
