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

#define GamePadState_NameDataLength (256)
typedef struct GamePadState
{
	bool isValid;
	int32 id;
	
	char name[GamePadState_NameDataLength];
	int16 values[(int32)GamePadInput_Count];
	int16 lastValues[(int32)GamePadInput_Count];

	int16 fixedValues[(int32)GamePadInput_Count];
	int16 fixedLastValues[(int32)GamePadInput_Count];
} GamePadState;

void Input_ClearFrameStates();
void Input_ClearFixedFrameStates();

typedef void (*Input_OnKeyboardEventHandledFunc)(Keycode keycode, bool state, bool repeat);
extern Input_OnKeyboardEventHandledFunc Input_onKeyboardEventHandled;

void Input_HandleKeyboardEvent(Keycode keycode, bool state, bool repeat);

bool Input_GetKeyPressed(Keycode keycode, bool isFixed);
bool Input_GetKeyPressedRepeat(Keycode keycode, bool isFixed);
bool Input_GetKeyReleased(Keycode keycode, bool isFixed);
bool Input_GetKey(Keycode keycode, bool isFixed);

bool Input_GetMouseButton(int32 button, bool isFixed);
bool Input_GetMouseButtonPressed(int32 button, bool isFixed);
bool Input_GetMouseButtonReleased(int32 button, bool isFixed);
float Input_GetMouseX(bool isFixed);
float Input_GetMouseY(bool isFixed);
float Input_GetMouseMotionX(bool isFixed);
float Input_GetMouseMotionY(bool isFixed);
float Input_GetMouseWheelX(bool isFixed);
float Input_GetMouseWheelY(bool isFixed);
float Input_GetMouseWheelTickScalerX();
float Input_GetMouseWheelTickScalerY();
void Input_HandleMouseButtonEvent(int32 button, bool state);
void Input_HandleMouseMotionEvent(float x, float y, float deltaX, float deltaY);
void Input_HandleWheelEvent(float x, float y);

typedef void (*Input_OnGamePadInputEventHandledFunc)(int32 id, GamePadInput input, int16 value);
extern Input_OnGamePadInputEventHandledFunc Input_onGamePadInputEventHandled;

typedef void (*Input_OnGamePadRegisterEventHandledFunc)(int32 id, bool registered);
extern Input_OnGamePadRegisterEventHandledFunc Input_onGamePadRegisterEventHandled;

#define Input_GamePadActivationThreshold (Int16Max/2)
#define Input_MaxGamePads (8)
int32 Input_GetGamePadStateIndex(int32 id);
GamePadState* Input_GetGamePadState(int32 id);
int32 Input_GetGamePadCount();
int32 Input_RegisterGamePad(int32 id, const char* name);
void Input_UnregisterGamePad(int32 id);
void Input_HandleGamepadInputEvent(int32 id, GamePadInput input, int16 value);

int16 Input_GetGamePadInput(int32 id, GamePadInput input, bool isFixed);
bool Input_GetGamePadInputActive(int32 id, GamePadInput input, bool isFixed);
bool Input_GetGamePadInputPressed(int32 id, GamePadInput input, bool isFixed);
bool Input_GetGamePadInputReleased(int32 id, GamePadInput input, bool isFixed);
