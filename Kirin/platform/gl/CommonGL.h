#pragma once

#include "common/Standard.h"

#if CONFIG_DEBUG
bool _CheckGLErrorPrintError(char* file, int line);
#define CheckGLError() {if (_CheckGLErrorPrintError(__FILE__, __LINE__)) __debugbreak();}
#else
#define CheckGLError()
#endif
