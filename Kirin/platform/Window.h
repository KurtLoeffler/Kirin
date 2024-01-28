#pragma once

#include "common/Standard.h"
#include "common/Space.h"
#include "common/Input.h"
#include "platform/WindowBackend.h"

typedef enum WindowMode
{
	WindowMode_None,
	WindowMode_Window,
	WindowMode_DesktopFullscreen,
	WindowMode_Fullscreen,
	WindowMode_Count
} WindowMode;

static const char* WindowMode_ToString(WindowMode value)
{
	switch (value) {
	case WindowMode_None: return "WindowMode_None"; break;
	case WindowMode_Window: return "WindowMode_Window"; break;
	case WindowMode_DesktopFullscreen: return "WindowMode_DesktopFullscreen"; break;
	case WindowMode_Fullscreen: return "WindowMode_Fullscreen"; break;
	default: return "INVALID"; break;
	}
	static_assert(WindowMode_Count == 4, "enum has changed.");
}

typedef enum VSyncMode
{
	VSyncMode_Off,
	VSyncMode_VSync,
	VSyncMode_AdaptiveVSync,
	VSyncMode_Count
} VSyncMode;

static const char* VSyncMode_ToString(VSyncMode value)
{
	switch (value) {
	case VSyncMode_Off: return "VSyncMode_Off"; break;
	case VSyncMode_VSync: return "VSyncMode_VSync"; break;
	case VSyncMode_AdaptiveVSync: return "VSyncMode_AdaptiveVSync"; break;
	default: return "INVALID"; break;
	}
	static_assert(VSyncMode_Count == 3, "enum has changed.");
}

typedef struct Window
{
	WindowBackend* backend;
	int32 windowWidth;
	int32 windowHeight;
	int32 exclusiveFullscreenWidth;
	int32 exclusiveFullscreenHeight;
	bool closeRequested;
	VSyncMode vSyncMode;
	WindowMode windowMode;
	bool cursorVisible;
	void* internalHandle;
	void* nativeHandle;
	void* nativeDevice;
} Window;
extern Window gWindow;

void Window_Init(Window* self, WindowBackend* backend, const char* title, int32 width, int32 height);
void Window_Free(Window* self);
void Window_Present(Window* self);
void Window_ProcessEvents(Window* self, InputState* inputState);
VSyncMode Window_GetVSyncMode(Window* self);
void Window_SetVSyncMode(Window* self, VSyncMode value);
WindowMode Window_GetWindowMode(Window* self);
void Window_SetWindowMode(Window* self, WindowMode windowMode);
IVec2 Window_GetSizeInPixels(Window* self);
IVec2 Window_GetSizeInScreenUnits(Window* self);
float Window_GetDPIScaleFactor(Window* self);
bool Window_GetMouseCursorVisible(Window* self);
void Window_SetMouseCursorVisible(Window* self, bool value);
bool Window_GetRelativeMouseMode(Window* self);
void Window_SetRelativeMouseMode(Window* self, bool value);
