#include "platform/gl/WindowBackendGL.h"

#include "common/CString.h"
#include "platform/Window.h"

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_syswm.h"

#if PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dwmapi.h>
// DwmFlush
#pragma comment(lib, "dwmapi")
#endif

#include "thirdparty/glad/glad.h"


static int32 openGLVersionMajor = 0;
static int32 openGLVersionMinor = 0;
static char openGLSLVersionString[8];

const char* WindowBackendGL_GetGLSLVersionString()
{
	return openGLSLVersionString;
}

static void Init(Window* window)
{
	SDL_GL_LoadLibrary(NULL);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
}

static void UpdateWindowFlags(Window* window, uint32* flags)
{
	*flags |= SDL_WINDOW_OPENGL;
}

static void CreateDevice(Window* window)
{
	SDL_Window* sdlWindow = (SDL_Window*)window->internalHandle;

	int32 versions[][2] = {
		{4, 6},
		{4, 5},
		{4, 4},
		{4, 3},
	};

	SDL_GLContext context = null;
	for (int32 i = 0; i < 4; i++)
	{
		int32 major = versions[i][0];
		int32 minor = versions[i][1];
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		context = SDL_GL_CreateContext(sdlWindow);
		if (context)
		{
			openGLVersionMajor = major;
			openGLVersionMinor = minor;
			SPrintF(openGLSLVersionString, 16, "%d%d", major, minor);
			if (StrLen(openGLSLVersionString) < 3)
			{
				StrCat(openGLSLVersionString+StrLen(openGLSLVersionString), "0", 16);
			}
			PrintF("using OpenGL Core %d.%d\n", major, minor);
			break;
		}
	}

	if (!context)
	{
		Error("Failed to create OpenGL context.");
	}

	gladLoadGLLoader(SDL_GL_GetProcAddress);

	PrintF("GL_VENDOR: %s\n", glGetString(GL_VENDOR));
	PrintF("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
	PrintF("GL_VERSION: %s\n", glGetString(GL_VERSION));

	window->nativeDevice = context;
}

static void ApplyVSync(VSyncMode mode)
{
	if (mode == VSyncMode_Off)
	{
		SDL_GL_SetSwapInterval(0);
	}
	else
	{
		if (mode == VSyncMode_AdaptiveVSync)
		{
			// first try adaptive vsync.
			if (SDL_GL_SetSwapInterval(-1) < 0)
			{
				// if that didn't work just use normal vsync.
				SDL_GL_SetSwapInterval(1);
			}
		}
		else
		{
			SDL_GL_SetSwapInterval(1);
		}
	}
}

static void Present(Window* window)
{
	SDL_Window* sdlWindow = (SDL_Window*)window->internalHandle;

	IVec2 windowSize = Window_GetSizeInPixels(window);
	glViewport(0, 0, windowSize.x, windowSize.y);

#if PLATFORM_WINDOWS
	{
		const bool useDwm = false;
		bool needsVSyncSet = true;
		if (useDwm && window->vSyncMode != VSyncMode_Off && window->windowMode == WindowMode_Window)
		{
			// hack to avoid stuttering in windowed mode on windows.
			bool compositionEnabled = false;
			if (SUCCEEDED(DwmIsCompositionEnabled(&compositionEnabled)) && compositionEnabled)
			{
				needsVSyncSet = false;
				ApplyVSync(VSyncMode_Off);
				DwmFlush();
			}
		}
		if (needsVSyncSet)
		{
			ApplyVSync(window->vSyncMode);
		}
	}
#else
	ApplyVSync(window->vSyncMode);
#endif

	SDL_GL_SwapWindow(sdlWindow);
}

static void VSyncModeChanged(Window* window)
{
	// vsync gets applied in Present now.
}

WindowBackend windowBackendGL = {
	.init = Init,
	.updateWindowFlags = UpdateWindowFlags,
	.createDevice = CreateDevice,
	.present = Present,
	.vsyncModeChanged = VSyncModeChanged,
};

WindowBackend* WindowBackendGL_Get()
{
	return &windowBackendGL;
}
