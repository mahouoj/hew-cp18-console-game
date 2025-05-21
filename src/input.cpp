#include "input.h"

#define CONIOEX
#include "conioex.h"

static InputType g_input_type;
static KeyState g_key_states[InputKey_Num];

enum InputBtnController {
	// button
	InputBtnController_Jump = 2,
	InputBtnController_Confirm = 4,
	InputBtnController_Retry = 8,
	InputBtnController_RotateLeft = 16,
	InputBtnController_RotateRight = 32,
	// pov
	InputBtnController_Up = 0,
	InputBtnController_UpRight = 4500,
	InputBtnController_Right = 9000,
	InputBtnController_RightDown = 1350,
	InputBtnController_Down = 18000,
	InputBtnController_DownLeft = 22500,
	InputBtnController_Left = 27000,
	InputBtnController_LeftUp = 31500
};

enum InputBtnKeyboard {
	InputBtnKeyboard_Up = PK_UP,
	InputBtnKeyboard_Right = PK_RIGHT,
	InputBtnKeyboard_Down = PK_DOWN,
	InputBtnKeyboard_Left = PK_LEFT,
	InputBtnKeyboard_Jump = PK_SP,
	InputBtnKeyboard_RotateLeft = PK_A,
	InputBtnKeyboard_RotateRight = PK_D,
	InputBtnKeyboard_Confirm = PK_ENTER,
	InputBtnKeyboard_Retry = PK_R
};

void input_set_key_press(InputKey key, bool press);
void input_update_keyboard();
void input_update_controller();
void input_update_controller_keyboard();

void input_init() {
	g_input_type = InputType_Controller;
}

void input_update() {
	switch (g_input_type) {
	case InputType_Controller: {
		// input_update_controller();
		input_update_controller_keyboard();
		// input_update_keyboard();
		break;
	}
	case InputType_Keyboard: {
		input_update_keyboard();
		break;
	}
	}
}

void input_update_keyboard() {
	input_set_key_press(InputKey_Up, inport(PK_UP));
	input_set_key_press(InputKey_Right, inport(PK_RIGHT));
	input_set_key_press(InputKey_Down, inport(PK_DOWN));
	input_set_key_press(InputKey_Left, inport(PK_LEFT));
	input_set_key_press(InputKey_RotateLeft, inport(PK_A));
	input_set_key_press(InputKey_RotateRight, inport(PK_D));
	input_set_key_press(InputKey_Jump, inport(PK_SP));
	input_set_key_press(InputKey_Confirm, inport(PK_ENTER));
	input_set_key_press(InputKey_Retry, inport(PK_R));
}

// keyboard or controller
void input_update_controller_keyboard() {
	// get input
	JOYINFOEX joyInfo;
	joyInfo.dwSize = sizeof(JOYINFOEX);
	joyInfo.dwFlags = JOY_RETURNALL;

	UINT joystickID = 1; // TODO

	if (joyGetPosEx(joystickID, &joyInfo) != JOYERR_NOERROR) {
		input_update_keyboard();
		return; // TODO
	}
	int btn = joyInfo.dwButtons;
	input_set_key_press(InputKey_Jump, (btn & InputBtnController_Jump) || inport(InputBtnKeyboard_Jump));
	input_set_key_press(InputKey_Confirm, (btn & InputBtnController_Confirm) || inport(InputBtnKeyboard_Confirm));
	input_set_key_press(InputKey_Retry, (btn & InputBtnController_Retry) || inport(InputBtnKeyboard_Retry));
	input_set_key_press(InputKey_RotateLeft, (btn & InputBtnController_RotateLeft) || inport(InputBtnKeyboard_RotateLeft));
	input_set_key_press(InputKey_RotateRight, (btn & InputBtnController_RotateRight) || inport(InputBtnKeyboard_RotateRight));
	// move
	int xpos = joyInfo.dwXpos;
	int ypos = joyInfo.dwYpos;
	int pov = joyInfo.dwPOV;
	bool up = inport(InputBtnKeyboard_Up);
	bool right = inport(InputBtnKeyboard_Right);
	bool down = inport(InputBtnKeyboard_Down);
	bool left = inport(InputBtnKeyboard_Left);
	switch (pov) {
	case InputBtnController_Up: {
		up = true;
		break;
	}
	case InputBtnController_UpRight: {
		up = true;
		right = true;
		break;
	}
	case InputBtnController_Right: {
		right = true;
		break;
	}
	case InputBtnController_RightDown: {
		right = true;
		down = true;
		break;
	}
	case InputBtnController_Down: {
		down = true;
		break;
	}
	case InputBtnController_DownLeft: {
		down = true;
		left = true;
		break;
	}
	case InputBtnController_Left: {
		left = true;
		break;
	}
	case InputBtnController_LeftUp: {
		left = true;
		up = true;
		break;
	}
	default: {
		float x = (float)xpos / 65535.0f;
		float y = (float)ypos / 65535.0f;
		if (x <= 0.3f) {
			left = true;
		}
		else if (x >= 0.7f) {
			right = true;
		}
		if (y <= 0.3f) {
			up = true;
		}
		else if (y >= 0.7f) {
			down = true;
		}
		break;
	}
	}
	input_set_key_press(InputKey_Up, up);
	input_set_key_press(InputKey_Right, right);
	input_set_key_press(InputKey_Down, down);
	input_set_key_press(InputKey_Left, left);
}

void input_update_controller() {
	// get input
	JOYINFOEX joyInfo;
	joyInfo.dwSize = sizeof(JOYINFOEX);
	joyInfo.dwFlags = JOY_RETURNALL;

	UINT joystickID = 1; // TODO

	if (joyGetPosEx(joystickID, &joyInfo) != JOYERR_NOERROR) {
		return; // TODO
	}
	int btn = joyInfo.dwButtons;
	input_set_key_press(InputKey_Jump, btn & InputBtnController_Jump);
	input_set_key_press(InputKey_Confirm, btn & InputBtnController_Confirm);
	input_set_key_press(InputKey_Retry, btn & InputBtnController_Retry);
	input_set_key_press(InputKey_RotateLeft, btn & InputBtnController_RotateLeft);
	input_set_key_press(InputKey_RotateRight, btn & InputBtnController_RotateRight);
	// move
	int xpos = joyInfo.dwXpos;
	int ypos = joyInfo.dwYpos;
	int pov = joyInfo.dwPOV;
	bool up = false, right = false, down = false, left = false;
	switch (pov) {
	case InputBtnController_Up: {
		up = true;
		break;
	}
	case InputBtnController_UpRight: {
		up = true;
		right = true;
		break;
	}
	case InputBtnController_Right: {
		right = true;
		break;
	}
	case InputBtnController_RightDown: {
		right = true;
		down = true;
		break;
	}
	case InputBtnController_Down: {
		down = true;
		break;
	}
	case InputBtnController_DownLeft: {
		down = true;
		left = true;
		break;
	}
	case InputBtnController_Left: {
		left = true;
		break;
	}
	case InputBtnController_LeftUp: {
		left = true;
		up = true;
		break;
	}
	default: {
		float x = (float)xpos / 65535.0f;
		float y = (float)ypos / 65535.0f;
		if (x <= 0.3f) {
			left = true;
		}
		else if (x >= 0.7f) {
			right = true;
		}
		if (y <= 0.3f) {
			up = true;
		}
		else if (y >= 0.7f) {
			down = true;
		}
		break;
	}
	}
	input_set_key_press(InputKey_Up, up);
	input_set_key_press(InputKey_Right, right);
	input_set_key_press(InputKey_Down, down);
	input_set_key_press(InputKey_Left, left);
}

KeyState input_get_key_state(InputKey key) {
	return g_key_states[key];
}

bool input_if_key_pressed(InputKey key) {
	return  g_key_states[key].press;
}

bool input_if_key_triggered(InputKey key) {
	return  g_key_states[key].trigger;
}

void input_set_key_press(InputKey key, bool press) {
	KeyState* state = g_key_states + key;
	bool press_old = state->press;
	state->press = press;
	state->trigger = !press_old && press;
	state->release = press_old && !press;
	state->press_old = press_old;
}