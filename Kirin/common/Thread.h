#pragma once

#include "common/Standard.h"

#if PLATFORM_WINDOWS
#if BITNESS_64
__forceinline void KMemoryBarrier() { __faststorefence(); }
#else
__forceinline void KMemoryBarrier() { long barrier; _InterlockedOr(&barrier, 0); }
#endif
#else
#error KMemoryBarrier not implemented on this platform.
#endif

typedef struct Mutex
{
    void* internalHandle;
} Mutex;

void Mutex_Init(Mutex* self);
void Mutex_Free(Mutex* self);
void Mutex_Lock(Mutex* self);
void Mutex_Unlock(Mutex* self);
