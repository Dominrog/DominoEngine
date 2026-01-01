#pragma once

struct InputState {
	bool keys[512]{};
	bool mouse_buttons[8]{};

	float mouse_x = 0.0f;
	float mouse_y = 0.0f;
	float mouse_dx = 0.0f;
	float mouse_dy = 0.0f;
	float scroll = 0.0f;

	bool first_mouse = true;
};