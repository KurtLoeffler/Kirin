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

#define CONFIGTYPE_DEBUG (CONFIG_DEBUG)
#define CONFIGTYPE_RELEASE (CONFIG_RELEASEDEV|CONFIG_RELEASE)
#define CONFIGTYPE_DEV (CONFIG_DEBUG|CONFIG_RELEASEDEV)

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

#if _WIN64
#define BITNESS_64 1
#else
#define BITNESS_32 1
#endif
