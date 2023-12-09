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

Input_OnKeyboardEventHandledFunc Input_onKeyboardEventHandled;
int keyStates[(int32)Keycode_Count];
int keyPressedStates[(int32)Keycode_Count];
int keyReleasedStates[(int32)Keycode_Count];

int fixedKeyPressedStates[(int32)Keycode_Count];
int fixedKeyReleasedStates[(int32)Keycode_Count];

#define Input_MaxMouseButtons 5
int mouseButtonStates[Input_MaxMouseButtons];
int mouseButtonPressedStates[Input_MaxMouseButtons];
int mouseButtonReleasedStates[Input_MaxMouseButtons];

int fixedMouseButtonPressedStates[Input_MaxMouseButtons];
int fixedMouseButtonReleasedStates[Input_MaxMouseButtons];

float mouseX;
float mouseY;
float mouseDeltaX;
float mouseDeltaY;
float fixedMouseDeltaX;
float fixedMouseDeltaY;

float mouseWheelX;
float mouseWheelY;
float fixedMouseWheelX;
float fixedMouseWheelY;

GamePadState gamePadStates[Input_MaxGamePads];

void Input_ClearFrameStates()
{
	for (int32 i = 0; i < (int32)Keycode_Count; i++)
	{
		keyPressedStates[i] = 0;
		keyReleasedStates[i] = 0;
	}

	for (int32 i = 0; i < Input_MaxMouseButtons; i++)
	{
		mouseButtonPressedStates[i] = 0;
		mouseButtonReleasedStates[i] = 0;
	}

	mouseDeltaX = 0;
	mouseDeltaY = 0;
	mouseWheelX = 0;
	mouseWheelY = 0;

	for (int32 i = 0; i < Input_MaxGamePads; i++)
	{
		GamePadState* state = &gamePadStates[i];
		if (state->isValid)
		{
			for (int32 j = 0; j < (int32)GamePadInput_Count; j++)
			{
				state->lastValues[j] = state->values[j];
			}
		}
	}
}

void Input_ClearFixedFrameStates()
{
	for (int32 i = 0; i < (int32)Keycode_Count; i++)
	{
		fixedKeyPressedStates[i] = 0;
		fixedKeyReleasedStates[i] = 0;
	}

	for (int32 i = 0; i < Input_MaxMouseButtons; i++)
	{
		fixedMouseButtonPressedStates[i] = 0;
		fixedMouseButtonReleasedStates[i] = 0;
	}

	fixedMouseDeltaX = 0;
	fixedMouseDeltaY = 0;
	fixedMouseWheelX = 0;
	fixedMouseWheelY = 0;

	for (int32 i = 0; i < Input_MaxGamePads; i++)
	{
		GamePadState* state = &gamePadStates[i];
		if (state->isValid)
		{
			for (int32 j = 0; j < (int32)GamePadInput_Count; j++)
			{
				state->fixedLastValues[j] = state->fixedValues[j];

				// align fixedValues with current values.
				state->fixedValues[j] = state->values[j];
			}
		}
	}
}

void Input_HandleKeyboardEvent(Keycode keycode, bool state, bool repeat)
{
	if (state)
	{
		int value = repeat ? 2 : 1;
		keyStates[(int32)keycode] = value;
		keyPressedStates[(int32)keycode] = value;

		fixedKeyPressedStates[(int32)keycode] = MaxI(fixedKeyPressedStates[(int32)keycode], value);
	}
	if (!state)
	{
		keyStates[(int32)keycode] = 0;
		keyReleasedStates[(int32)keycode] = 1;

		fixedKeyReleasedStates[(int32)keycode] = MaxI(fixedKeyReleasedStates[(int32)keycode], 1);
	}

	if (Input_onKeyboardEventHandled)
	{
		Input_onKeyboardEventHandled(keycode, state, repeat);
	}
}

bool Input_GetKeyPressed(Keycode keycode, bool isFixed)
{
	return (isFixed ? fixedKeyPressedStates : keyPressedStates)[(int32)keycode] == 1;
}

bool Input_GetKeyPressedRepeat(Keycode keycode, bool isFixed)
{
	return (isFixed ? fixedKeyPressedStates : keyPressedStates)[(int32)keycode] != 0;
}

bool Input_GetKeyReleased(Keycode keycode, bool isFixed)
{
	return (isFixed ? fixedKeyReleasedStates : keyReleasedStates)[(int32)keycode] != 0;
}

bool Input_GetKey(Keycode keycode, bool isFixed)
{
	// currently just use keyStates for both fixed and non-fixed key states.
	return keyStates[(int32)keycode] != 0;
}

bool Input_GetMouseButton(int32 button, bool isFixed)
{
	if (button < 0 || button >= Input_MaxMouseButtons)
	{
		return false;
	}

	// currently just use mouseButtonStates for both fixed and non-fixed mouse states.
	return mouseButtonStates[button] != 0;
}

bool Input_GetMouseButtonPressed(int32 button, bool isFixed)
{
	if (button < 0 || button >= Input_MaxMouseButtons)
	{
		return false;
	}

	return (isFixed ? fixedMouseButtonPressedStates : mouseButtonPressedStates)[button] == 1;
}

bool Input_GetMouseButtonReleased(int32 button, bool isFixed)
{
	if (button < 0 || button >= Input_MaxMouseButtons)
	{
		return false;
	}

	return (isFixed ? fixedMouseButtonReleasedStates : mouseButtonReleasedStates)[button] != 0;
}

float Input_GetMouseX(bool isFixed)
{
	// currently just use mouseX for both fixed and non-fixed.
	return (isFixed ? mouseX : mouseX);
}

float Input_GetMouseY(bool isFixed)
{
	// currently just use mouseX for both fixed and non-fixed.
	return (isFixed ? mouseY : mouseY);
}

float Input_GetMouseMotionX(bool isFixed)
{
	return (isFixed ? fixedMouseDeltaX : mouseDeltaX);
}

float Input_GetMouseMotionY(bool isFixed)
{
	return (isFixed ? fixedMouseDeltaY : mouseDeltaY);
}

float Input_GetMouseWheelX(bool isFixed)
{
	return (isFixed ? fixedMouseWheelX : mouseWheelX);
}

float Input_GetMouseWheelY(bool isFixed)
{
	return (isFixed ? fixedMouseWheelY : mouseWheelY);
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

void Input_HandleMouseButtonEvent(int32 button, bool state)
{
	if (button < 0 || button >= Input_MaxMouseButtons)
	{
		return;
	}

	if (state)
	{
		mouseButtonStates[button] = 1;
		mouseButtonPressedStates[button] = 1;

		fixedMouseButtonPressedStates[button] = MaxI(fixedMouseButtonPressedStates[button], 1);
	}
	if (!state)
	{
		mouseButtonStates[button] = 0;
		mouseButtonReleasedStates[button] = 1;

		fixedMouseButtonReleasedStates[button] = MaxI(fixedMouseButtonReleasedStates[button], 1);
	}
}

void Input_HandleMouseMotionEvent(float x, float y, float deltaX, float deltaY)
{
	mouseX = x;
	mouseY = y;
	mouseDeltaX += deltaX;
	mouseDeltaY += deltaY;
	fixedMouseDeltaX += deltaX;
	fixedMouseDeltaY += deltaY;
}

void Input_HandleWheelEvent(float x, float y)
{
	mouseWheelX += x;
	mouseWheelY += y;
	fixedMouseWheelX += x;
	fixedMouseWheelY += y;
}

Input_OnGamePadInputEventHandledFunc Input_onGamePadInputEventHandled;
Input_OnGamePadRegisterEventHandledFunc Input_onGamePadRegisterEventHandled;

int32 Input_GetGamePadStateIndex(int32 id)
{
	for (int32 i = 0; i < Input_MaxGamePads; i++)
	{
		GamePadState* state = &gamePadStates[i];
		if (state->isValid && state->id == id)
		{
			return i;
		}
	}
	return -1;
}

GamePadState* Input_GetGamePadState(int32 id)
{
	int32 index = Input_GetGamePadStateIndex(id);
	if (index < 0)
	{
		return null;
	}

	return &gamePadStates[index];
}

int32 Input_GetGamePadCount()
{
	int32 count = 0;
	for (int32 i = 0; i < Input_MaxGamePads; i++)
	{
		GamePadState* state = &gamePadStates[i];
		if (state->isValid)
		{
			count++;
		}
	}
	return count;
}

int32 Input_RegisterGamePad(int32 id, const char* name)
{
	if (id < 0)
	{
		Error("id must be > 0.");
	}

	int32 index = Input_GetGamePadStateIndex(id);
	if (index >= 0)
	{
		ErrorF("gamepad with id %d already exists.", id);
	}

	for (int32 i = 0; i < Input_MaxGamePads; i++)
	{
		GamePadState* state = &gamePadStates[i];
		if (!state->isValid)
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

	GamePadState* state = &gamePadStates[index];
	state->isValid = true;
	state->id = id;
	StrCpy(state->name, name, GamePadState_NameDataLength);

	PrintF("registered gamepad with id %d \"%s\"\n", id, name);

	if (Input_onGamePadRegisterEventHandled)
	{
		Input_onGamePadRegisterEventHandled(id, true);
	}

	return index;
}

void Input_UnregisterGamePad(int32 id)
{
	if (id < 0)
	{
		Error("id must be > 0.");
	}

	GamePadState* state = Input_GetGamePadState(id);
	if (!state)
	{
		Error("gamepad with id %d does not exist.");
	}

	if (Input_onGamePadRegisterEventHandled)
	{
		Input_onGamePadRegisterEventHandled(id, false);
	}

	PrintF("unregistered gamepad with id %d \"%s\"\n", id, state->name);
	MemSet(state, 0, sizeof(GamePadState));
}

void Input_HandleGamepadInputEvent(int32 id, GamePadInput input, int16 value)
{
	value = ClampI(value, -Int16Max, Int16Max);
	
	if (input <= GamePadInput_None || input >= GamePadInput_Count)
	{
		ErrorF("GamePadInput out of range \"%d\"", (int32)input);
	}
	GamePadState* state = Input_GetGamePadState(id);
	if (state)
	{
		state->values[(int32)input] = value;

		// allow fixed values to increase value, or change axis direction with each non fixed input event.
		if (SignI(state->fixedValues[(int32)input]) != SignI(value) || AbsI(value) > AbsI(state->fixedValues[(int32)input]))
		{
			state->fixedValues[(int32)input] = value;
		}
	}

	if (Input_onGamePadInputEventHandled)
	{
		Input_onGamePadInputEventHandled(id, input, value);
	}
}

int16 Input_GetGamePadInput(int32 id, GamePadInput input, bool isFixed)
{
	GamePadState* state = Input_GetGamePadState(id);
	if (!state)
	{
		return 0;
	}
	int16* values = isFixed ? state->fixedValues : state->values;
	return values[(int32)input];
}

static bool ValueIsActive(int16 value)
{
	return AbsI(value) >= Input_GamePadActivationThreshold;
}

bool Input_GetGamePadInputActive(int32 id, GamePadInput input, bool isFixed)
{
	return ValueIsActive(Input_GetGamePadInput(id, input, isFixed));
}

bool Input_GetGamePadInputPressed(int32 id, GamePadInput input, bool isFixed)
{
	GamePadState* state = Input_GetGamePadState(id);
	if (!state)
	{
		return false;
	}
	int16* lastValues = isFixed ? state->fixedLastValues : state->lastValues;
	uint8 lastActive = ValueIsActive(lastValues[(int32)input]);
	return Input_GetGamePadInputActive(id, input, isFixed) && !lastActive;
}

bool Input_GetGamePadInputReleased(int32 id, GamePadInput input, bool isFixed)
{
	GamePadState* state = Input_GetGamePadState(id);
	if (!state)
	{
		return false;
	}
	int16* lastValues = isFixed ? state->fixedLastValues : state->lastValues;
	uint8 lastActive = ValueIsActive(lastValues[(int32)input]);
	return !Input_GetGamePadInputActive(id, input, isFixed) && lastActive;
}
