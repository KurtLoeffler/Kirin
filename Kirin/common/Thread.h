#pragma once

#include "common/Standard.h"

typedef struct Mutex
{
    void* internalHandle;
} Mutex;

void Mutex_Init(Mutex* self);
void Mutex_Free(Mutex* self);
void Mutex_Lock(Mutex* self);
void Mutex_Unlock(Mutex* self);
