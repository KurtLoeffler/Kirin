#pragma once

#include "common/Standard.h"
#include "platform/WindowBackend.h"

WindowBackend* WindowBackendGL_Get();
const char* WindowBackendGL_GetGLSLVersionString();
