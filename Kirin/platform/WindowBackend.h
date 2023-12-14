#pragma once

#include "common/Standard.h"

typedef struct WindowBackend
{
	void (*init)(struct Window* window);
	void (*updateWindowFlags)(struct Window* window, uint32* flags);
	void (*createDevice)(struct Window* window);
	void (*present)(struct Window* window);
	void (*vsyncModeChanged)(struct Window* window);
} WindowBackend;
