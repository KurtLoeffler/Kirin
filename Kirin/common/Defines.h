#pragma once

#ifndef CONFIG_DEBUG
#define CONFIG_DEBUG 0
#endif

#ifndef CONFIG_RELEASEDEV
#define CONFIG_RELEASEDEV 0
#endif

#ifndef CONFIG_RELEASE
#define CONFIG_RELEASE 0
#endif

#if CONFIG_DEBUG+CONFIG_RELEASEDEV+CONFIG_RELEASE != 1
#error exactly one of the following must be defined: CONFIG_DEBUG, CONFIG_RELEASEDEV, CONFIG_RELEASE
#endif

#ifndef PLATFORM_WINDOWS
#define PLATFORM_WINDOWS 0
#endif

#if PLATFORM_WINDOWS != 1
#error exactly one of the following must be defined: PLATFORM_WINDOWS
#endif

#if PLATFORM_WINDOWS
#define _CONSOLE 1

#if CONFIG_DEBUG
#define _DEBUG 1
#else
#define NDEBUG 1
#endif
#endif
