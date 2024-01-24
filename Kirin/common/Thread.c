#include "common/Thread.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

void Mutex_Free(Mutex* self)
{
    CloseHandle(self->internalHandle);
}

void Mutex_Lock(volatile Mutex* self)
{
    if (self->internalHandle == null)
    {
        HANDLE p = CreateMutex(NULL, FALSE, NULL);
        if (!p)
        {
            ErrorF("failed to create mutex: %d", GetLastError());
        }
        if (InterlockedCompareExchangePointer(&self->internalHandle, (void*)p, null) != null)
        {
            CloseHandle(p);
        }
    }
    WaitForSingleObject(self->internalHandle, INFINITE);
}

void Mutex_Unlock(Mutex* self)
{
    ReleaseMutex(self->internalHandle);
}
