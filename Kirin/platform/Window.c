#include "platform/Window.h"

#include "platform/SDL2Input.h"

// currently SDL specific but should be abstracted later.
#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_syswm.h"

Window gWindow;

void Window_Init(Window* self, WindowBackend* backend, const char* title, int32 width, int32 height)
{
	*self = (Window){ 0 };

	self->backend = backend;
	self->windowWidth = width;
	self->windowHeight = height;
	self->vSyncMode = VSyncMode_AdaptiveVSync;
	self->windowMode = WindowMode_Window;
	self->cursorVisible = true;

	// HACK: this fixes a weird issue trigger event values are invalid when pushing both triggers.
	SDL_SetHint(SDL_HINT_JOYSTICK_RAWINPUT, "0");

	// required for proper high dpi support for windows.
	SDL_SetHint(SDL_HINT_WINDOWS_DPI_SCALING, "1");

	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_GAMECONTROLLER);

	backend->init(self);

#if 0
	{
		int modesCount = SDL_GetNumDisplayModes(0);
		if (modesCount < 1) {
			return;
		}
		for (int i = 0; i < modesCount; ++i)
		{
			SDL_DisplayMode mode;
			if (SDL_GetDisplayMode(0, i, &mode) != 0)
			{
				return;
			}
			PrintF("mode %d: %dx%d @%dhz\n", i, mode.w, mode.h, mode.refresh_rate);
		}
	}
#endif

	uint32 windowFlags = SDL_WINDOW_RESIZABLE|SDL_WINDOW_ALLOW_HIGHDPI;

	backend->updateWindowFlags(self, &windowFlags);

	int32 posX = SDL_WINDOWPOS_CENTERED;
	int32 posY = SDL_WINDOWPOS_CENTERED;
	if (false)
	{
		posX = 1200;
	}
	SDL_Window* sdlWindow = SDL_CreateWindow(title, posX, posY, width, height, windowFlags);

	self->internalHandle = sdlWindow;
	{
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(sdlWindow, &wmInfo);
		self->nativeHandle = (HWND)wmInfo.info.win.window;
	}

	backend->createDevice(self);

	Window_SetVSyncMode(self, Window_GetVSyncMode(self));
}

void Window_Free(Window* self)
{
	SDL_Window* sdlWindow = (SDL_Window*)self->internalHandle;

	SDL_DestroyWindow(sdlWindow);
}

void Window_Present(Window* self)
{
	self->backend->present(self);
}

void Window_ProcessEvents(Window* self, InputState* inputState)
{
	SDL_Window* sdlWindow = (SDL_Window*)self->internalHandle;

	Input_ClearFrameStates(inputState);

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		SDL2Input_HandleEvent(inputState, &event);
		switch (event.type)
		{
		case SDL_QUIT:
			self->closeRequested = true;
			break;
		case SDL_MOUSEMOTION:
		{
#if 0
			// example of how to get dpi aware pixel mouse position.
			float x = event.motion.x*GetDPIScaleFactor();
			float y = event.motion.y*GetDPIScaleFactor();
			ZXPrintF("Mouse moved to (%f, %f)\n", x, y);
#endif
			break;
		}
		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_SIZE_CHANGED:
			{
				IVec2 pixelSize = Window_GetSizeInPixels(self);
				PrintF("window size changed: %dx%d (%dx%d)\n", event.window.data1, event.window.data2, pixelSize.x, pixelSize.y);
				if (Window_GetWindowMode(self) == WindowMode_Window)
				{
					IVec2 windowSize = Window_GetSizeInScreenUnits(self);
					self->windowWidth = windowSize.x;
					self->windowHeight = windowSize.y;
				}
				break;
			}
			default:
				break;
			}
			break;
		}
	}

	if ((Input_GetKey(inputState, Keycode_LAlt, false) || Input_GetKey(inputState, Keycode_RAlt, false)) && Input_GetKeyPressed(inputState, Keycode_Enter, false))
	{
		if (Window_GetWindowMode(self) == WindowMode_Window)
		{
			Window_SetWindowMode(self, WindowMode_DesktopFullscreen);
		}
		else
		{
			Window_SetWindowMode(self, WindowMode_Window);
		}

		Input_ClearFrameStates(inputState);
		Input_ClearFixedFrameStates(inputState);
	}
}

VSyncMode Window_GetVSyncMode(Window* self)
{
	return self->vSyncMode;
}

void Window_SetVSyncMode(Window* self, VSyncMode value)
{
	self->vSyncMode = value ? 1 : 0;
	self->backend->vsyncModeChanged(self);
}

WindowMode Window_GetWindowMode(Window* self)
{
	return self->windowMode;
}

static SDL_DisplayMode GetDesktopDisplayMode()
{
	SDL_DisplayMode desktopMode;
	if (SDL_GetDesktopDisplayMode(0, &desktopMode) != 0)
	{
		// In case of error...
		ErrorF("failed to get desktop display mode: %s", SDL_GetError());
	}
	return desktopMode;
}

static void EnterExclusiveFullscreenMode(Window* window)
{
	SDL_Window* sdlWindow = (SDL_Window*)window->internalHandle;

	SDL_DisplayMode desktopMode = GetDesktopDisplayMode();
	if (window->exclusiveFullscreenWidth > 0 && window->exclusiveFullscreenHeight > 0)
	{
		desktopMode.w = window->exclusiveFullscreenWidth;
		desktopMode.h = window->exclusiveFullscreenHeight;
	}

	if (SDL_SetWindowDisplayMode(sdlWindow, &desktopMode) != 0)
	{
		ErrorF("failed to set fullscreen display mode: %s", SDL_GetError());
	}

	if (SDL_SetWindowFullscreen(sdlWindow, SDL_WINDOW_FULLSCREEN) != 0)
	{
		ErrorF("failed to switch to fullscreen: %s", SDL_GetError());
	}
}

void Window_SetWindowMode(Window* self, WindowMode windowMode)
{
	if (Window_GetWindowMode(self) == windowMode)
	{
		return;
	}

	SDL_Window* sdlWindow = (SDL_Window*)self->internalHandle;

	self->windowMode = windowMode;

	switch (windowMode)
	{
	case WindowMode_Window:
		SDL_SetWindowFullscreen(sdlWindow, 0);
		break;
	case WindowMode_DesktopFullscreen:
		SDL_SetWindowFullscreen(sdlWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
		break;
	case WindowMode_Fullscreen:
		EnterExclusiveFullscreenMode(self);
		break;
	default:
		break;
	}
}

IVec2 Window_GetSizeInPixels(Window* self)
{
	SDL_Window* sdlWindow = (SDL_Window*)self->internalHandle;

	IVec2 result;
	SDL_GetWindowSizeInPixels(sdlWindow, &result.x, &result.y);
	return result;
}

IVec2 Window_GetSizeInScreenUnits(Window* self)
{
	SDL_Window* sdlWindow = (SDL_Window*)self->internalHandle;

	IVec2 result;
	SDL_GetWindowSize(sdlWindow, &result.x, &result.y);
	return result;
}

float Window_GetDPIScaleFactor(Window* self)
{
	IVec2 pixelSize = Window_GetSizeInPixels(self);
	IVec2 screenSize = Window_GetSizeInScreenUnits(self);
	
	return (float)pixelSize.y/(float)screenSize.y;
}

bool Window_GetMouseCursorVisible(Window* self)
{
	return self->cursorVisible;
}

void Window_SetMouseCursorVisible(Window* self, bool value)
{
	self->cursorVisible = value;
	SDL_ShowCursor(value ? SDL_ENABLE : SDL_DISABLE);
}

bool Window_GetRelativeMouseMode(Window* self)
{
	return SDL_GetRelativeMouseMode();
}

void Window_SetRelativeMouseMode(Window* self, bool value)
{
	SDL_SetRelativeMouseMode(value);
}
