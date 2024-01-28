#include "common/Input.h"

#include "common/CString.h"

const char* Keycode_ToString(Keycode keycode)
{
	switch (keycode)
	{
#define KEYCODE(name, value) case Keycode_##name: return #name;
#include "Keycodes.h"
#undef KEYCODE
	default:
		return "INVALID";
		break;
	}
}

void Input_ClearFrameStates(InputState* state)
{
	for (int32 i = 0; i < (int32)Keycode_Count; i++)
	{
		state->keyPressedStates[i] = 0;
		state->keyReleasedStates[i] = 0;
	}

	for (int32 i = 0; i < Input_MaxMouseButtons; i++)
	{
		state->mouseButtonPressedStates[i] = 0;
		state->mouseButtonReleasedStates[i] = 0;
	}

	state->mouseDeltaX = 0;
	state->mouseDeltaY = 0;
	state->mouseWheelX = 0;
	state->mouseWheelY = 0;

	for (int32 i = 0; i < Input_MaxGamePads; i++)
	{
		GamePadState* gamePadState = &state->gamePadStates[i];
		if (gamePadState->isValid)
		{
			for (int32 j = 0; j < (int32)GamePadInput_Count; j++)
			{
				gamePadState->lastValues[j] = gamePadState->values[j];
			}
		}
	}
}

void Input_ClearFixedFrameStates(InputState* state)
{
	for (int32 i = 0; i < (int32)Keycode_Count; i++)
	{
		state->fixedKeyPressedStates[i] = 0;
		state->fixedKeyReleasedStates[i] = 0;
	}

	for (int32 i = 0; i < Input_MaxMouseButtons; i++)
	{
		state->fixedMouseButtonPressedStates[i] = 0;
		state->fixedMouseButtonReleasedStates[i] = 0;
	}

	state->fixedMouseDeltaX = 0;
	state->fixedMouseDeltaY = 0;
	state->fixedMouseWheelX = 0;
	state->fixedMouseWheelY = 0;

	for (int32 i = 0; i < Input_MaxGamePads; i++)
	{
		GamePadState* gamePadState = &state->gamePadStates[i];
		if (gamePadState->isValid)
		{
			for (int32 j = 0; j < (int32)GamePadInput_Count; j++)
			{
				gamePadState->fixedLastValues[j] = gamePadState->fixedValues[j];

				// align fixedValues with current values.
				gamePadState->fixedValues[j] = gamePadState->values[j];
			}
		}
	}
}

void Input_HandleKeyboardEvent(InputState* state, Keycode keycode, bool value, bool repeat)
{
	if (value)
	{
		int value = repeat ? 2 : 1;
		state->keyStates[(int32)keycode] = value;
		state->keyPressedStates[(int32)keycode] = value;

		state->fixedKeyPressedStates[(int32)keycode] = MaxI(state->fixedKeyPressedStates[(int32)keycode], value);
	}
	if (!value)
	{
		state->keyStates[(int32)keycode] = 0;
		state->keyReleasedStates[(int32)keycode] = 1;

		state->fixedKeyReleasedStates[(int32)keycode] = MaxI(state->fixedKeyReleasedStates[(int32)keycode], 1);
	}

	if (state->onKeyboardEventHandled)
	{
		state->onKeyboardEventHandled(keycode, value, repeat);
	}
}

bool Input_GetKeyPressed(InputState* state, Keycode keycode, bool isFixed)
{
	return (isFixed ? state->fixedKeyPressedStates : state->keyPressedStates)[(int32)keycode] == 1;
}

bool Input_GetKeyPressedRepeat(InputState* state, Keycode keycode, bool isFixed)
{
	return (isFixed ? state->fixedKeyPressedStates : state->keyPressedStates)[(int32)keycode] != 0;
}

bool Input_GetKeyReleased(InputState* state, Keycode keycode, bool isFixed)
{
	return (isFixed ? state->fixedKeyReleasedStates : state->keyReleasedStates)[(int32)keycode] != 0;
}

bool Input_GetKey(InputState* state, Keycode keycode, bool isFixed)
{
	// currently just use keyStates for both fixed and non-fixed key states.
	return state->keyStates[(int32)keycode] != 0;
}

bool Input_GetMouseButton(InputState* state, int32 button, bool isFixed)
{
	if (button < 0 || button >= Input_MaxMouseButtons)
	{
		return false;
	}

	// currently just use mouseButtonStates for both fixed and non-fixed mouse states.
	return state->mouseButtonStates[button] != 0;
}

bool Input_GetMouseButtonPressed(InputState* state, int32 button, bool isFixed)
{
	if (button < 0 || button >= Input_MaxMouseButtons)
	{
		return false;
	}

	return (isFixed ? state->fixedMouseButtonPressedStates : state->mouseButtonPressedStates)[button] == 1;
}

bool Input_GetMouseButtonReleased(InputState* state, int32 button, bool isFixed)
{
	if (button < 0 || button >= Input_MaxMouseButtons)
	{
		return false;
	}

	return (isFixed ? state->fixedMouseButtonReleasedStates : state->mouseButtonReleasedStates)[button] != 0;
}

float Input_GetMouseX(InputState* state, bool isFixed)
{
	// currently just use mouseX for both fixed and non-fixed.
	return (isFixed ? state->mouseX : state->mouseX);
}

float Input_GetMouseY(InputState* state, bool isFixed)
{
	// currently just use mouseX for both fixed and non-fixed.
	return (isFixed ? state->mouseY : state->mouseY);
}

float Input_GetMouseMotionX(InputState* state, bool isFixed)
{
	return (isFixed ? state->fixedMouseDeltaX : state->mouseDeltaX);
}

float Input_GetMouseMotionY(InputState* state, bool isFixed)
{
	return (isFixed ? state->fixedMouseDeltaY : state->mouseDeltaY);
}

float Input_GetMouseWheelX(InputState* state, bool isFixed)
{
	return (isFixed ? state->fixedMouseWheelX : state->mouseWheelX);
}

float Input_GetMouseWheelY(InputState* state, bool isFixed)
{
	return (isFixed ? state->fixedMouseWheelY : state->mouseWheelY);
}

float Input_GetMouseWheelTickScalerX()
{
	// hardcoded default value for windows for now.
	return 3;
}

float Input_GetMouseWheelTickScalerY()
{
	// hardcoded default value for windows for now.
	return 3;
}

void Input_HandleMouseButtonEvent(InputState* state, int32 button, bool value)
{
	if (button < 0 || button >= Input_MaxMouseButtons)
	{
		return;
	}

	if (value)
	{
		state->mouseButtonStates[button] = 1;
		state->mouseButtonPressedStates[button] = 1;

		state->fixedMouseButtonPressedStates[button] = MaxI(state->fixedMouseButtonPressedStates[button], 1);
	}
	if (!value)
	{
		state->mouseButtonStates[button] = 0;
		state->mouseButtonReleasedStates[button] = 1;

		state->fixedMouseButtonReleasedStates[button] = MaxI(state->fixedMouseButtonReleasedStates[button], 1);
	}
}

void Input_HandleMouseMotionEvent(InputState* state, float x, float y, float deltaX, float deltaY)
{
	state->mouseX = x;
	state->mouseY = y;
	state->mouseDeltaX += deltaX;
	state->mouseDeltaY += deltaY;
	state->fixedMouseDeltaX += deltaX;
	state->fixedMouseDeltaY += deltaY;
}

void Input_HandleWheelEvent(InputState* state, float x, float y)
{
	state->mouseWheelX += x;
	state->mouseWheelY += y;
	state->fixedMouseWheelX += x;
	state->fixedMouseWheelY += y;
}

int32 Input_GetGamePadStateIndex(InputState* state, int32 id)
{
	for (int32 i = 0; i < Input_MaxGamePads; i++)
	{
		GamePadState* gamePadState = &state->gamePadStates[i];
		if (gamePadState->isValid && gamePadState->id == id)
		{
			return i;
		}
	}
	return -1;
}

GamePadState* Input_GetGamePadState(InputState* state, int32 id)
{
	int32 index = Input_GetGamePadStateIndex(state, id);
	if (index < 0)
	{
		return null;
	}

	return &state->gamePadStates[index];
}

int32 Input_GetGamePadCount(InputState* state)
{
	int32 count = 0;
	for (int32 i = 0; i < Input_MaxGamePads; i++)
	{
		GamePadState* gamePadState = &state->gamePadStates[i];
		if (gamePadState->isValid)
		{
			count++;
		}
	}
	return count;
}

int32 Input_RegisterGamePad(InputState* state, int32 id, const char* name)
{
	if (id < 0)
	{
		Error("id must be > 0.");
	}

	int32 index = Input_GetGamePadStateIndex(state, id);
	if (index >= 0)
	{
		ErrorF("gamepad with id %d already exists.", id);
	}

	for (int32 i = 0; i < Input_MaxGamePads; i++)
	{
		GamePadState* gamePadState = &state->gamePadStates[i];
		if (!gamePadState->isValid)
		{
			index = i;
			break;
		}
	}

	if (index < 0)
	{
		return -1;
	}

	name = name ? name : "unknown";

	GamePadState* gamePadState = &state->gamePadStates[index];
	gamePadState->isValid = true;
	gamePadState->id = id;
	StrCpy(gamePadState->name, name, MaxPathLength);

	PrintF("registered gamepad with id %d \"%s\"\n", id, name);

	if (state->onGamePadRegisterEventHandled)
	{
		state->onGamePadRegisterEventHandled(id, true);
	}

	return index;
}

void Input_UnregisterGamePad(InputState* state, int32 id)
{
	if (id < 0)
	{
		Error("id must be > 0.");
	}

	GamePadState* gamePadState = Input_GetGamePadState(state, id);
	if (!gamePadState)
	{
		Error("gamepad with id %d does not exist.");
	}

	if (state->onGamePadRegisterEventHandled)
	{
		state->onGamePadRegisterEventHandled(id, false);
	}

	PrintF("unregistered gamepad with id %d \"%s\"\n", id, gamePadState->name);
	MemSet(gamePadState, 0, sizeof(GamePadState));
}

void Input_HandleGamepadInputEvent(InputState* state, int32 id, GamePadInput input, int16 value)
{
	value = ClampI(value, -Int16Max, Int16Max);
	
	if (input <= GamePadInput_None || input >= GamePadInput_Count)
	{
		ErrorF("GamePadInput out of range \"%d\"", (int32)input);
	}
	GamePadState* gamePadState = Input_GetGamePadState(state, id);
	if (gamePadState)
	{
		gamePadState->values[(int32)input] = value;

		// allow fixed values to increase value, or change axis direction with each non fixed input event.
		if (SignI(gamePadState->fixedValues[(int32)input]) != SignI(value) || AbsI(value) > AbsI(gamePadState->fixedValues[(int32)input]))
		{
			gamePadState->fixedValues[(int32)input] = value;
		}
	}

	if (state->onGamePadInputEventHandled)
	{
		state->onGamePadInputEventHandled(id, input, value);
	}
}

int16 Input_GetGamePadInput(InputState* state, int32 id, GamePadInput input, bool isFixed)
{
	GamePadState* gamePadState = Input_GetGamePadState(state, id);
	if (!gamePadState)
	{
		return 0;
	}
	int16* values = isFixed ? gamePadState->fixedValues : gamePadState->values;
	return values[(int32)input];
}

static bool ValueIsActive(int16 value)
{
	return AbsI(value) >= Input_GamePadActivationThreshold;
}

bool Input_GetGamePadInputActive(InputState* state, int32 id, GamePadInput input, bool isFixed)
{
	return ValueIsActive(Input_GetGamePadInput(state, id, input, isFixed));
}

bool Input_GetGamePadInputPressed(InputState* state, int32 id, GamePadInput input, bool isFixed)
{
	GamePadState* gamePadState = Input_GetGamePadState(state, id);
	if (!gamePadState)
	{
		return false;
	}
	int16* lastValues = isFixed ? gamePadState->fixedLastValues : gamePadState->lastValues;
	uint8 lastActive = ValueIsActive(lastValues[(int32)input]);
	return Input_GetGamePadInputActive(state, id, input, isFixed) && !lastActive;
}

bool Input_GetGamePadInputReleased(InputState* state, int32 id, GamePadInput input, bool isFixed)
{
	GamePadState* gamePadState = Input_GetGamePadState(state, id);
	if (!gamePadState)
	{
		return false;
	}
	int16* lastValues = isFixed ? gamePadState->fixedLastValues : gamePadState->lastValues;
	uint8 lastActive = ValueIsActive(lastValues[(int32)input]);
	return !Input_GetGamePadInputActive(state, id, input, isFixed) && lastActive;
}
