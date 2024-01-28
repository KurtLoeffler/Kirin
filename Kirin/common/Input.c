#include "common/Input.h"

#include "common/CString.h"

GamePadEntry gamePadEntries[Input_MaxGamePads];

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
		state->keyPressedRepeatStates[i] = 0;
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
		GamePadEntry* gamePadEntry = &gamePadEntries[i];
		if (gamePadEntry->isValid)
		{
			GamePadState* gamePadState = &state->gamePadStates[i];
			for (int32 i = 0; i < GamePadInput_Count; i++)
			{
				gamePadState->pressed[i] = 0;
				gamePadState->released[i] = 0;
			}
		}
	}
}

void Input_ClearFrameStatesAndDependents(InputState* state)
{
	while (state)
	{
		Input_ClearFrameStates(state);
		state = state->dependent;
	}
}

static DependentHandleKeyboardInput(InputState* state, InputState* dependentState, Keycode keycode, bool value)
{
	dependentState->keyStates[(int32)keycode] = state->keyStates[(int32)keycode];
	if (value)
	{
		dependentState->keyPressedStates[(int32)keycode] = MaxI(dependentState->keyPressedStates[(int32)keycode], state->keyPressedStates[(int32)keycode]);
		dependentState->keyPressedRepeatStates[(int32)keycode] = MaxI(dependentState->keyPressedRepeatStates[(int32)keycode], state->keyPressedRepeatStates[(int32)keycode]);
	}
	if (!value)
	{
		dependentState->keyReleasedStates[(int32)keycode] = MaxI(dependentState->keyReleasedStates[(int32)keycode], state->keyReleasedStates[(int32)keycode]);
	}
}

void Input_HandleKeyboardEvent(InputState* state, Keycode keycode, bool value, bool repeat)
{
	state->keyStates[(int32)keycode] = (int32)value;
	if (value)
	{
		if (repeat)
		{
			state->keyPressedRepeatStates[(int32)keycode] = 1;
		}
		else
		{
			state->keyPressedStates[(int32)keycode] = 1;
		}
	}
	if (!value)
	{
		state->keyReleasedStates[(int32)keycode] = 1;
	}

	if (state->dependent)
	{
		DependentHandleKeyboardInput(state, state->dependent, keycode, value);
	}

	if (state->onKeyboardEventHandled)
	{
		state->onKeyboardEventHandled(keycode, value, repeat);
	}
}

bool Input_GetKeyPressed(InputState* state, Keycode keycode)
{
	return state->keyPressedStates[(int32)keycode] != 0;
}

bool Input_GetKeyPressedRepeat(InputState* state, Keycode keycode)
{
	return state->keyPressedRepeatStates[(int32)keycode] != 0;
}

bool Input_GetKeyReleased(InputState* state, Keycode keycode)
{
	return state->keyReleasedStates[(int32)keycode] != 0;
}

bool Input_GetKey(InputState* state, Keycode keycode)
{
	return state->keyStates[(int32)keycode] != 0;
}

bool Input_GetMouseButton(InputState* state, int32 button)
{
	if (button < 0 || button >= Input_MaxMouseButtons)
	{
		return false;
	}

	return state->mouseButtonStates[button] != 0;
}

bool Input_GetMouseButtonPressed(InputState* state, int32 button)
{
	if (button < 0 || button >= Input_MaxMouseButtons)
	{
		return false;
	}

	return state->mouseButtonPressedStates[button] == 1;
}

bool Input_GetMouseButtonReleased(InputState* state, int32 button)
{
	if (button < 0 || button >= Input_MaxMouseButtons)
	{
		return false;
	}

	return state->mouseButtonReleasedStates[button] != 0;
}

float Input_GetMouseX(InputState* state)
{
	return state->mouseX;
}

float Input_GetMouseY(InputState* state)
{
	return state->mouseY;
}

float Input_GetMouseMotionX(InputState* state)
{
	return state->mouseDeltaX;
}

float Input_GetMouseMotionY(InputState* state)
{
	return state->mouseDeltaY;
}

float Input_GetMouseWheelX(InputState* state)
{
	return state->mouseWheelX;
}

float Input_GetMouseWheelY(InputState* state)
{
	return state->mouseWheelY;
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

static DependentHandleMouseButtonEvent(InputState* state, InputState* dependentState, int32 button, bool value)
{
	dependentState->mouseButtonStates[button] = state->mouseButtonStates[button];
	if (value)
	{
		dependentState->mouseButtonPressedStates[button] = MaxI(dependentState->mouseButtonPressedStates[button], state->mouseButtonPressedStates[button]);
	}
	if (!value)
	{
		dependentState->mouseButtonReleasedStates[button] = MaxI(dependentState->mouseButtonReleasedStates[button], state->mouseButtonReleasedStates[button]);
	}
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
	}
	if (!value)
	{
		state->mouseButtonStates[button] = 0;
		state->mouseButtonReleasedStates[button] = 1;
	}

	if (state->dependent)
	{
		DependentHandleMouseButtonEvent(state, state->dependent, button, value);
	}
}

void Input_HandleMouseMotionEvent(InputState* state, float x, float y, float deltaX, float deltaY)
{
	state->mouseX = x;
	state->mouseY = y;
	state->mouseDeltaX += deltaX;
	state->mouseDeltaY += deltaY;

	if (state->dependent)
	{
		state->dependent->mouseX = state->mouseX;
		state->dependent->mouseY = state->mouseY;
		state->dependent->mouseDeltaX += deltaX;
		state->dependent->mouseDeltaY += deltaY;
	}
}

void Input_HandleWheelEvent(InputState* state, float x, float y)
{
	state->mouseWheelX += x;
	state->mouseWheelY += y;

	if (state->dependent)
	{
		state->dependent->mouseWheelX += x;
		state->dependent->mouseWheelY += y;
	}
}

int32 Input_GetGamePadIndex(int32 id)
{
	for (int32 i = 0; i < Input_MaxGamePads; i++)
	{
		GamePadEntry* gamePadEntry = &gamePadEntries[i];
		if (gamePadEntry->isValid && gamePadEntry->id == id)
		{
			return i;
		}
	}
	return -1;
}

GamePadEntry* Input_GetGamePadEntry(int32 id)
{
	int32 index = Input_GetGamePadIndex(id);
	if (index < 0)
	{
		return null;
	}

	return &gamePadEntries[index];
}

GamePadState* Input_GetGamePadState(InputState* state, int32 id)
{
	int32 index = Input_GetGamePadIndex(id);
	if (index < 0)
	{
		return null;
	}

	return &state->gamePadStates[index];
}

int32 Input_GetGamePadCount()
{
	int32 count = 0;
	for (int32 i = 0; i < Input_MaxGamePads; i++)
	{
		GamePadEntry* gamePadEntry = &gamePadEntries[i];
		if (gamePadEntry->isValid)
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

	int32 index = Input_GetGamePadIndex(id);
	if (index >= 0)
	{
		ErrorF("gamepad with id %d already exists.", id);
	}

	for (int32 i = 0; i < Input_MaxGamePads; i++)
	{
		GamePadEntry* gamePadEntry = &gamePadEntries[i];
		if (!gamePadEntry->isValid)
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

	GamePadEntry* gamePadEntry = &gamePadEntries[index];
	gamePadEntry->isValid = true;
	gamePadEntry->id = id;
	gamePadEntry->index = index;
	StrCpy(gamePadEntry->name, name, MaxPathLength);

	PrintF("registered gamepad with id %d \"%s\"\n", id, name);

	return index;
}

void Input_UnregisterGamePad(int32 id)
{
	if (id < 0)
	{
		Error("id must be > 0.");
	}

	GamePadEntry* gamePadEntry = Input_GetGamePadEntry(id);
	
	if (!gamePadEntry)
	{
		Error("gamepad with id %d does not exist.");
	}

	PrintF("unregistered gamepad with id %d \"%s\"\n", id, gamePadEntry->name);
	MemSet(gamePadEntry, 0, sizeof(GamePadEntry));
}

static bool ValueIsActive(int16 value)
{
	return AbsI(value) >= Input_GamePadActivationThreshold;
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
		gamePadState->prevValues[(int32)input] = gamePadState->values[(int32)input];
		gamePadState->values[(int32)input] = value;
		bool isActive = ValueIsActive(gamePadState->values[(int32)input]);
		bool wasActive = ValueIsActive(gamePadState->prevValues[(int32)input]);
		if (isActive && wasActive)
		{
			// special case if an analog stick immediately changed sign but stayed active.
			if (SignI(gamePadState->prevValues[(int32)input]) != SignI(value))
			{
				gamePadState->pressed[(int32)input] = SignI(value);
				gamePadState->released[(int32)input] = SignI(gamePadState->prevValues[(int32)input]);
			}
		}
		else if (isActive && !wasActive)
		{
			gamePadState->pressed[(int32)input] = SignI(value);
		}
		else if (wasActive && !isActive)
		{
			gamePadState->released[(int32)input] = SignI(gamePadState->prevValues[(int32)input]);
		}

#if 1
		if (state->dependent)
		{
			Input_HandleGamepadInputEvent(state->dependent, id, input, value);
		}
#else
		// allow dependent values to increase value, or change axis direction with each non dependent input event.
		if (state->dependent)
		{
			GamePadState* dependentGamePadState = Input_GetGamePadState(state->dependent, id);
			if (dependentGamePadState)
			{
				if (SignI(dependentGamePadState->values[(int32)input]) != SignI(value) || AbsI(value) > AbsI(dependentGamePadState->values[(int32)input]))
				{
					dependentGamePadState->values[(int32)input] = value;
				}
			}
		}
#endif
	}
}

int16 Input_GetGamePadInput(InputState* state, int32 id, GamePadInput input)
{
	GamePadState* gamePadState = Input_GetGamePadState(state, id);
	if (!gamePadState)
	{
		return 0;
	}
	return gamePadState->values[(int32)input];
}

bool Input_GetGamePadInputActive(InputState* state, int32 id, GamePadInput input)
{
	return ValueIsActive(Input_GetGamePadInput(state, id, input));
}

bool Input_GetGamePadInputPressed(InputState* state, int32 id, GamePadInput input)
{
	GamePadState* gamePadState = Input_GetGamePadState(state, id);
	if (!gamePadState)
	{
		return false;
	}
	return !!gamePadState->pressed[(int32)input];
}

bool Input_GetGamePadInputReleased(InputState* state, int32 id, GamePadInput input)
{
	GamePadState* gamePadState = Input_GetGamePadState(state, id);
	if (!gamePadState)
	{
		return false;
	}

	return !!gamePadState->released[(int32)input];
}
