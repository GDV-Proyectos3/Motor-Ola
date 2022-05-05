#pragma once

#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif

#include <iostream>
#include <SDL.h>
#include <array>

#include "utils/Singleton.h"
#include "OgreManager.h"

// Instead of a Singleton class, we could make it part of
// SDLUtils as well.

MOTOR_API class InputManager : public Singleton<InputManager> {

	friend Singleton<InputManager>;

public:
	MOTOR_API enum MOUSEBUTTON : uint8_t {
		LEFT = 0, MIDDLE = 1, RIGHT = 2
	};

	virtual ~InputManager() {
	}

	// clear the state
	MOTOR_API inline void clearState() {
		_isCloseWindoEvent = false;
		_isKeyDownEvent = false;
		_isKeyUpEvent = false;
		_isMouseButtonEvent = false;
		_isMouseMotionEvent = false;
	}



	// update the state with a new event
	MOTOR_API inline void update(const SDL_Event& event) {
		switch (event.type) {
		case SDL_KEYDOWN:
			onKeyDown(event);
			break;
		case SDL_KEYUP:
			onKeyUp(event);
			break;
		case SDL_MOUSEMOTION:
			onMouseMotion(event);
			break;
		case SDL_MOUSEBUTTONDOWN:
			onMouseButtonChange(event, true);
			break;
		case SDL_MOUSEBUTTONUP:
			onMouseButtonChange(event, false);
			break;
		case SDL_WINDOWEVENT:
			handleWindowEvent(event);
			break;
		default:
			break;
		}
	}

	// refresh
	MOTOR_API inline void refresh() {
		SDL_Event event;

		clearState();
		while (SDL_PollEvent(&event))
			update(event);
	}

	// close window event
	MOTOR_API inline bool closeWindowEvent() {
		return _isCloseWindoEvent;
	}

	// keyboard
	MOTOR_API inline bool keyDownEvent() {
		return _isKeyDownEvent;
	}

	MOTOR_API inline bool keyUpEvent() {
		return _isKeyUpEvent;
	}

	MOTOR_API inline bool isKeyDown(SDL_Scancode key) {
		return keyDownEvent() && _kbState[key] == 1;
	}

	MOTOR_API inline bool isKeyDown(SDL_Keycode key) {
		return isKeyDown(SDL_GetScancodeFromKey(key));
	}

	MOTOR_API inline bool isKeyUp(SDL_Scancode key) {
		return keyUpEvent() && _kbState[key] == 0;
	}

	MOTOR_API inline bool isKeyUp(SDL_Keycode key) {
		return isKeyUp(SDL_GetScancodeFromKey(key));
	}

	// mouse
	MOTOR_API inline bool mouseMotionEvent() {
		return _isMouseMotionEvent;
	}

	MOTOR_API inline bool mouseButtonEvent() {
		return _isMouseButtonEvent;
	}

	MOTOR_API inline const std::pair<Sint32, Sint32>& getMousePos() {
		return _mousePos;
	}

	MOTOR_API inline int getMouseButtonState(MOUSEBUTTON b) {
		return _mbState[b];
	}

	MOTOR_API inline std::pair<Sint32, Sint32> getMousePosInGame() {
		std::pair<Sint32, Sint32>p;
		p.first = _mousePos.first -(Singleton<OgreManager>::instance()->getWindowWidth() / 2);
		p.second = _mousePos.second -(Singleton<OgreManager>::instance()->getWindowHeight() / 2);
		return p;

	}

private:
	InputManager() {
		_kbState = SDL_GetKeyboardState(0);
		clearState();
	}

	inline void onKeyDown(const SDL_Event&) {
		_isKeyDownEvent = true;
	}

	inline void onKeyUp(const SDL_Event&) {
		_isKeyUpEvent = true;
	}

	inline void onMouseMotion(const SDL_Event& event) {
		_isMouseMotionEvent = true;
		_mousePos.first = event.motion.x;
		_mousePos.second = event.motion.y;

	}

	inline void onMouseButtonChange(const SDL_Event& event, bool isDown) {
		_isMouseButtonEvent = true;
		switch (event.button.button) {
		case SDL_BUTTON_LEFT:
			_mbState[LEFT] = isDown;
			break;
		case SDL_BUTTON_MIDDLE:
			_mbState[MIDDLE] = isDown;
			break;
		case SDL_BUTTON_RIGHT:
			_mbState[RIGHT] = isDown;
			break;
		default:
			break;
		}
	}

	inline void handleWindowEvent(const SDL_Event& event) {
		switch (event.window.event) {
		case SDL_WINDOWEVENT_CLOSE:
			_isCloseWindoEvent = true;
			break;
		default:
			break;
		}
	}

	bool _isCloseWindoEvent;
	bool _isKeyUpEvent;
	bool _isKeyDownEvent;
	bool _isMouseMotionEvent;
	bool _isMouseButtonEvent;
	std::pair<Sint32, Sint32> _mousePos;
	std::array<bool, 3> _mbState;
	const Uint8* _kbState;
}
;

// This macro defines a compact way for using the singleton InputHandler, instead of
// writing InputHandler::instance()->method() we write ih().method()
//
MOTOR_API inline InputManager& ih() {
	return *InputManager::instance();
}

