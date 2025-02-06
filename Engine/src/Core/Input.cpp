#include "Kosmic/Core/Input.hpp"

namespace Kosmic::Input {
	// Store mouse delta
	static int s_MouseDeltaX = 0;
	static int s_MouseDeltaY = 0;

	void ProcessEvent(const SDL_Event& event) {
		// If mouse movement occurs, accumulate deltas
		if (event.type == SDL_MOUSEMOTION) {
			s_MouseDeltaX += event.motion.xrel;
			s_MouseDeltaY += event.motion.yrel;
		}
	}

    // Add input handling using SDL
    bool IsKeyPressed(SDL_Keycode key) {
        const Uint8* state = SDL_GetKeyboardState(nullptr);
        return state[SDL_GetScancodeFromKey(key)];
    }

	void GetMouseDelta(int& deltaX, int& deltaY) {
		deltaX = s_MouseDeltaX;
		deltaY = s_MouseDeltaY;
		// Resets deltas after reading
		s_MouseDeltaX = 0;
		s_MouseDeltaY = 0;
	}

	void Reset() {
		s_MouseDeltaX = 0;
		s_MouseDeltaY = 0;
	}
}
