#include "platform/SDL2Input.h"

#include "common/Math.h"

Keycode ZXSDL2KeycodeToKey(SDL_Keycode scancode)
{
	switch (scancode)
	{
	case SDLK_TAB: return Keycode_Tab;
	case SDLK_RETURN: return Keycode_Enter;
	case SDLK_RETURN2: return Keycode_Enter;
	case SDLK_ESCAPE: return Keycode_Escape;
	case SDLK_SPACE: return Keycode_Space;
	case SDLK_a: return Keycode_A;
	case SDLK_b: return Keycode_B;
	case SDLK_c: return Keycode_C;
	case SDLK_d: return Keycode_D;
	case SDLK_e: return Keycode_E;
	case SDLK_f: return Keycode_F;
	case SDLK_g: return Keycode_G;
	case SDLK_h: return Keycode_H;
	case SDLK_i: return Keycode_I;
	case SDLK_j: return Keycode_J;
	case SDLK_k: return Keycode_K;
	case SDLK_l: return Keycode_L;
	case SDLK_m: return Keycode_M;
	case SDLK_n: return Keycode_N;
	case SDLK_o: return Keycode_O;
	case SDLK_p: return Keycode_P;
	case SDLK_q: return Keycode_Q;
	case SDLK_r: return Keycode_R;
	case SDLK_s: return Keycode_S;
	case SDLK_t: return Keycode_T;
	case SDLK_u: return Keycode_U;
	case SDLK_v: return Keycode_V;
	case SDLK_w: return Keycode_W;
	case SDLK_x: return Keycode_X;
	case SDLK_y: return Keycode_Y;
	case SDLK_z: return Keycode_Z;
	case SDLK_1: return Keycode_Digit1;
	case SDLK_2: return Keycode_Digit2;
	case SDLK_3: return Keycode_Digit3;
	case SDLK_4: return Keycode_Digit4;
	case SDLK_5: return Keycode_Digit5;
	case SDLK_6: return Keycode_Digit6;
	case SDLK_7: return Keycode_Digit7;
	case SDLK_8: return Keycode_Digit8;
	case SDLK_9: return Keycode_Digit9;
	case SDLK_0: return Keycode_Digit0;
	case SDLK_MINUS: return Keycode_Minus;
	case SDLK_EQUALS: return Keycode_Equals;
	case SDLK_LEFTBRACKET: return Keycode_LeftBracket;
	case SDLK_RIGHTBRACKET: return Keycode_RightBracket;
	case SDLK_BACKSLASH: return Keycode_Backslash;
	case SDLK_HASH: return Keycode_Hash;
	case SDLK_SEMICOLON: return Keycode_Semicolon;
	case SDLK_QUOTE: return Keycode_Quote;
	case SDLK_BACKQUOTE: return Keycode_Backquote;
	case SDLK_COMMA: return Keycode_Comma;
	case SDLK_PERIOD: return Keycode_Period;
	case SDLK_SLASH: return Keycode_Slash;
	case SDLK_BACKSPACE: return Keycode_Backspace;
	case SDLK_UP: return Keycode_UpArrow;
	case SDLK_DOWN: return Keycode_DownArrow;
	case SDLK_LEFT: return Keycode_LeftArrow;
	case SDLK_RIGHT: return Keycode_RightArrow;
	case SDLK_LALT: return Keycode_LAlt;
	case SDLK_RALT: return Keycode_RAlt;
	case SDLK_LCTRL: return Keycode_LCtrl;
	case SDLK_RCTRL: return Keycode_RCtrl;
	case SDLK_LSHIFT: return Keycode_LShift;
	case SDLK_RSHIFT: return Keycode_RShift;
	case SDLK_F1: return Keycode_F1;
	case SDLK_F2: return Keycode_F2;
	case SDLK_F3: return Keycode_F3;
	case SDLK_F4: return Keycode_F4;
	case SDLK_F5: return Keycode_F5;
	case SDLK_F6: return Keycode_F6;
	case SDLK_F7: return Keycode_F7;
	case SDLK_F8: return Keycode_F8;
	case SDLK_F9: return Keycode_F9;
	case SDLK_F10: return Keycode_F10;
	case SDLK_F11: return Keycode_F11;
	case SDLK_F12: return Keycode_F12;
	case SDLK_INSERT: return Keycode_Ins;
	case SDLK_DELETE: return Keycode_Del;
	case SDLK_PAGEDOWN: return Keycode_PageDown;
	case SDLK_PAGEUP: return Keycode_PageUp;
	case SDLK_HOME: return Keycode_Home;
	case SDLK_END: return Keycode_End;
	case SDLK_PAUSE: return Keycode_Pause;
	default: return Keycode_None;
	}
}

static void TryAddGamePadDevice(int32 joystickIndex)
{
	if (SDL_IsGameController(joystickIndex))
	{
		SDL_GameController* gameController = SDL_GameControllerOpen(joystickIndex);

		if (gameController)
		{
			SDL_Joystick* joystick = SDL_GameControllerGetJoystick(gameController);
#if 0
			char guid[65];
			SDL_JoystickGetGUIDString(SDL_JoystickGetGUID(joystick), guid, 65);
			ZXPrintF("joystick guid: %s\n", guid);
#endif
			int32 id = SDL_JoystickInstanceID(joystick);
			Input_RegisterGamePad(id, SDL_GameControllerName(gameController));
		}
		else
		{
			WarningF("failed to open GameController %d\n", joystickIndex);
		}
	}
}

static void TryRemoveGamePadDevice(int32 id)
{
	if (Input_GetGamePadIndex(id) >= 0)
	{
		Input_UnregisterGamePad(id);
	}
}

void SDL2Input_HandleEvent(InputState* state, SDL_Event* event)
{
	if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP)
	{
		Keycode keycode = ZXSDL2KeycodeToKey(event->key.keysym.sym);
		if (keycode != Keycode_None)
		{
			Input_HandleKeyboardEvent(state, keycode, event->type == SDL_KEYDOWN, event->key.repeat);
		}
	}
	else if (event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP)
	{
		int32 mouseButton = -1;
		switch (event->button.button)
		{
		case SDL_BUTTON_LEFT:
			mouseButton = 0;
			break;
		case SDL_BUTTON_RIGHT:
			mouseButton = 1;
			break;
		case SDL_BUTTON_MIDDLE:
			mouseButton = 2;
			break;
		case SDL_BUTTON_X1:
			mouseButton = 3;
			break;
		case SDL_BUTTON_X2:
			mouseButton = 4;
			break;
		default:
			break;
		}
		if (mouseButton >= 0)
		{
			Input_HandleMouseButtonEvent(state, mouseButton, event->type == SDL_MOUSEBUTTONDOWN);
		}
	}
	else if (event->type == SDL_MOUSEMOTION)
	{
		float scaleFactor = 1;
		SDL_Window* sdlWindow = SDL_GetWindowFromID(event->motion.windowID);
		if (sdlWindow)
		{
			int32 pixelWidth;
			int32 pixelHeight;
			SDL_GetWindowSizeInPixels(sdlWindow, &pixelWidth, &pixelHeight);

			int32 windowWidth;
			int32 windowHeight;
			SDL_GetWindowSize(sdlWindow, &windowWidth, &windowHeight);
			scaleFactor = (float)pixelHeight/(float)windowHeight;
		}

		float x = (float)event->motion.x/scaleFactor;
		float y = (float)event->motion.y/scaleFactor;
		float relX = (float)event->motion.xrel/scaleFactor;
		float relY = (float)event->motion.yrel/scaleFactor;

		Input_HandleMouseMotionEvent(state, x, y, relX, relY);
	}
	else if (event->type == SDL_MOUSEWHEEL)
	{
		Input_HandleWheelEvent(state, (float)event->wheel.preciseX, (float)event->wheel.preciseY);
	}
	else if (event->type == SDL_CONTROLLERDEVICEADDED)
	{
		TryAddGamePadDevice(event->cdevice.which);
	}
	else if (event->type == SDL_CONTROLLERDEVICEREMOVED)
	{
		TryRemoveGamePadDevice(event->cdevice.which);
	}
	else if (event->type == SDL_CONTROLLERAXISMOTION || event->type == SDL_CONTROLLERBUTTONUP || event->type == SDL_CONTROLLERBUTTONDOWN)
	{
		int32 id = 0;
		int16 value = 0;
		GamePadInput input = GamePadInput_None;
		
		if (event->type == SDL_CONTROLLERAXISMOTION)
		{
			id = event->caxis.which;
			value = event->caxis.value;
			switch (event->caxis.axis)
			{
			case SDL_CONTROLLER_AXIS_LEFTX:
				input = GamePadInput_LeftStickX;
				break;
			case SDL_CONTROLLER_AXIS_LEFTY:
				input = GamePadInput_LeftStickY;
				break;
			case SDL_CONTROLLER_AXIS_RIGHTX:
				input = GamePadInput_RightStickX;
				break;
			case SDL_CONTROLLER_AXIS_RIGHTY:
				input = GamePadInput_RightStickY;
				break;
			case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
				input = GamePadInput_LeftTrigger;
				break;
			case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
				input = GamePadInput_RightTrigger;
				break;
			default:
				Warning("unexpected axis.");
				break;
			}
		}
		else
		{
			id = event->cbutton.which;
			value = event->type == SDL_CONTROLLERBUTTONDOWN ? Int16Max : 0;
			switch (event->cbutton.button)
			{
				case SDL_CONTROLLER_BUTTON_A:
					input = GamePadInput_FaceDown;
					break;
				case SDL_CONTROLLER_BUTTON_B:
					input = GamePadInput_FaceRight;
					break;
				case SDL_CONTROLLER_BUTTON_X:
					input = GamePadInput_FaceLeft;
					break;
				case SDL_CONTROLLER_BUTTON_Y:
					input = GamePadInput_FaceUp;
					break;
				case SDL_CONTROLLER_BUTTON_BACK:
					input = GamePadInput_Back;
					break;
				case SDL_CONTROLLER_BUTTON_GUIDE:
					input = GamePadInput_Home;
					break;
				case SDL_CONTROLLER_BUTTON_START:
					input = GamePadInput_Start;
					break;
				case SDL_CONTROLLER_BUTTON_LEFTSTICK:
					input = GamePadInput_LeftStickClick;
					break;
				case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
					input = GamePadInput_RightStickClick;
					break;
				case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
					input = GamePadInput_LeftShoulder;
					break;
				case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
					input = GamePadInput_RightShoulder;
					break;
				case SDL_CONTROLLER_BUTTON_DPAD_UP:
					input = GamePadInput_DPadUp;
					break;
				case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
					input = GamePadInput_DPadDown;
					break;
				case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
					input = GamePadInput_DPadLeft;
					break;
				case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
					input = GamePadInput_DPadRight;
					break;
				case SDL_CONTROLLER_BUTTON_MISC1:
				case SDL_CONTROLLER_BUTTON_PADDLE1:
				case SDL_CONTROLLER_BUTTON_PADDLE2:
				case SDL_CONTROLLER_BUTTON_PADDLE3:
				case SDL_CONTROLLER_BUTTON_PADDLE4:
				case SDL_CONTROLLER_BUTTON_TOUCHPAD:
					break;
			default:
				Warning("unexpected button.");
				break;
			}
		}

		if (input > GamePadInput_None && input < GamePadInput_Count)
		{
			if (Input_GetGamePadIndex(id) >= 0)
			{
				Input_HandleGamepadInputEvent(state, id, input, value);
			}
		}
	}
}
