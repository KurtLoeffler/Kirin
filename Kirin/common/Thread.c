#include "common/Thread.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

void Mutex_Init(Mutex* self)
{
    self->internalHandle = CreateMutex(NULL, FALSE, NULL);
    if (!self->internalHandle)
    {
        ErrorF("failed to create mutex: %d", GetLastError());
    }
}

void Mutex_Free(Mutex* self)
{
    CloseHandle(self->internalHandle);
}

void Mutex_Lock(Mutex* self)
{
    WaitForSingleObject(self->internalHandle, INFINITE);
}

void Mutex_Unlock(Mutex* self)
{
    ReleaseMutex(self->internalHandle);
}
