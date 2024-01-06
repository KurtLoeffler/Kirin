#pragma once

#include "common/Standard.h"
#include "platform/WindowBackend.h"

WindowBackend* WindowBackendGL_Get();

void WindowBackendGL_GetGLVersion(int32* outMajor, int32* outMinor);
// return -1 if input is less than context.
// return 0 if input is equal to context.
// return 1 if input is larger than context.
int32 WindowBackendGL_CompareGLVersion(int32 major, int32 minor);
const char* WindowBackendGL_GetGLSLVersionString();
