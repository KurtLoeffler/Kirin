#pragma once

#include "common/Standard.h"
#include "common/Math.h"

typedef enum Keycode
{
#define KEYCODE(name, value) Keycode_##name = value,
#include "Keycodes.h"
#undef KEYCODE
} Keycode;
const char* Keycode_ToString(Keycode keycode);

typedef enum GamePadInput
{
	GamePadInput_None,
	GamePadInput_DPadUp,
	GamePadInput_DPadDown,
	GamePadInput_DPadLeft,
	GamePadInput_DPadRight,
	GamePadInput_FaceUp,
	GamePadInput_FaceDown,
	GamePadInput_FaceLeft,
	GamePadInput_FaceRight,
	GamePadInput_Back,
	GamePadInput_Home,
	GamePadInput_Start,
	GamePadInput_LeftStickX,
	GamePadInput_LeftStickY,
	GamePadInput_RightStickX,
	GamePadInput_RightStickY,
	GamePadInput_LeftStickClick,
	GamePadInput_RightStickClick,
	GamePadInput_LeftTrigger,
	GamePadInput_RightTrigger,
	GamePadInput_LeftShoulder,
	GamePadInput_RightShoulder,
	GamePadInput_Count
} GamePadInput;

static const char* GamePadInput_ToString(GamePadInput value)
{
	switch (value) {
	case GamePadInput_None: return "GamePadInput_None"; break;
	case GamePadInput_DPadUp: return "GamePadInput_DPadUp"; break;
	case GamePadInput_DPadDown: return "GamePadInput_DPadDown"; break;
	case GamePadInput_DPadLeft: return "GamePadInput_DPadLeft"; break;
	case GamePadInput_DPadRight: return "GamePadInput_DPadRight"; break;
	case GamePadInput_FaceUp: return "GamePadInput_FaceUp"; break;
	case GamePadInput_FaceDown: return "GamePadInput_FaceDown"; break;
	case GamePadInput_FaceLeft: return "GamePadInput_FaceLeft"; break;
	case GamePadInput_FaceRight: return "GamePadInput_FaceRight"; break;
	case GamePadInput_Back: return "GamePadInput_Back"; break;
	case GamePadInput_Home: return "GamePadInput_Home"; break;
	case GamePadInput_Start: return "GamePadInput_Start"; break;
	case GamePadInput_LeftStickX: return "GamePadInput_LeftStickX"; break;
	case GamePadInput_LeftStickY: return "GamePadInput_LeftStickY"; break;
	case GamePadInput_RightStickX: return "GamePadInput_RightStickX"; break;
	case GamePadInput_RightStickY: return "GamePadInput_RightStickY"; break;
	case GamePadInput_LeftStickClick: return "GamePadInput_LeftStickClick"; break;
	case GamePadInput_RightStickClick: return "GamePadInput_RightStickClick"; break;
	case GamePadInput_LeftTrigger: return "GamePadInput_LeftTrigger"; break;
	case GamePadInput_RightTrigger: return "GamePadInput_RightTrigger"; break;
	case GamePadInput_LeftShoulder: return "GamePadInput_LeftShoulder"; break;
	case GamePadInput_RightShoulder: return "GamePadInput_RightShoulder"; break;
	case GamePadInput_Count: return "GamePadInput_Count"; break;
	default: return "INVALID"; break;
	}
	static_assert(GamePadInput_Count == 22, "enum has changed.");
}

#define Input_GamePadActivationThreshold (Int16Max/2)
#define Input_MaxGamePads (8)
typedef struct GamePadEntry
{
	bool isValid;
	int32 id;
	int32 index;
	char name[MaxPathLength];
} GamePadEntry;
extern GamePadEntry gamePadEntries[Input_MaxGamePads];

typedef struct GamePadState
{
	int16 values[(int32)GamePadInput_Count];
	int16 prevValues[(int32)GamePadInput_Count];
	int8 pressed[(int32)GamePadInput_Count];
	int8 released[(int32)GamePadInput_Count];
} GamePadState;

#define Input_MaxMouseButtons 5
typedef struct InputState
{
	struct InputState* dependent;

	void (*onKeyboardEventHandled)(Keycode keycode, bool value, bool repeat);

	int keyStates[(int32)Keycode_Count];
	int keyPressedStates[(int32)Keycode_Count];
	int keyPressedRepeatStates[(int32)Keycode_Count];
	int keyReleasedStates[(int32)Keycode_Count];

	int mouseButtonStates[Input_MaxMouseButtons];
	int mouseButtonPressedStates[Input_MaxMouseButtons];
	int mouseButtonReleasedStates[Input_MaxMouseButtons];

	float mouseX;
	float mouseY;
	float mouseDeltaX;
	float mouseDeltaY;

	float mouseWheelX;
	float mouseWheelY;

	GamePadState gamePadStates[Input_MaxGamePads];
} InputState;

void Input_ClearFrameStates(InputState* state);
void Input_ClearFrameStatesAndDependents(InputState* state);

void Input_HandleKeyboardEvent(InputState* state, Keycode keycode, bool value, bool repeat);
bool Input_GetKeyPressed(InputState* state, Keycode keycode);
bool Input_GetKeyPressedRepeat(InputState* state, Keycode keycode);
bool Input_GetKeyReleased(InputState* state, Keycode keycode);
bool Input_GetKey(InputState* state, Keycode keycode);

bool Input_GetMouseButton(InputState* state, int32 button);
bool Input_GetMouseButtonPressed(InputState* state, int32 button);
bool Input_GetMouseButtonReleased(InputState* state, int32 button);
float Input_GetMouseX(InputState* state);
float Input_GetMouseY(InputState* state);
float Input_GetMouseMotionX(InputState* state);
float Input_GetMouseMotionY(InputState* state);
float Input_GetMouseWheelX(InputState* state);
float Input_GetMouseWheelY(InputState* state);
float Input_GetMouseWheelTickScalerX();
float Input_GetMouseWheelTickScalerY();
void Input_HandleMouseButtonEvent(InputState* state, int32 button, bool value);
void Input_HandleMouseMotionEvent(InputState* state, float x, float y, float deltaX, float deltaY);
void Input_HandleWheelEvent(InputState* state, float x, float y);

int32 Input_GetGamePadIndex(int32 id);
GamePadEntry* Input_GetGamePadEntry(int32 id);
GamePadState* Input_GetGamePadState(InputState* state, int32 id);
int32 Input_GetGamePadCount();
int32 Input_RegisterGamePad(int32 id, const char* name);
void Input_UnregisterGamePad(int32 id);
void Input_HandleGamepadInputEvent(InputState* state, int32 id, GamePadInput input, int16 value);

int16 Input_GetGamePadInput(InputState* state, int32 id, GamePadInput input);
bool Input_GetGamePadInputActive(InputState* state, int32 id, GamePadInput input);
bool Input_GetGamePadInputPressed(InputState* state, int32 id, GamePadInput input);
bool Input_GetGamePadInputReleased(InputState* state, int32 id, GamePadInput input);
