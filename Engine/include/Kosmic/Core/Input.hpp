#pragma once
#include <SDL2/SDL.h>

namespace Kosmic::Input {
	// Process an individual SDL event
	void ProcessEvent(const SDL_Event& event);
	// Returns the mouse delta (accumulated movement) and then resets the values
	void GetMouseDelta(int& deltaX, int& deltaY);

    // Add input handling using SDL
    bool IsKeyPressed(SDL_Keycode key);

	// Reset mouse delta
	void Reset();
}
