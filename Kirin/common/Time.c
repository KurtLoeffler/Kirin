
#include "common/Time.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static double invTickFrequency = 1.0;
static bool needsFrequencyScale;
void InitTime()
{
	LARGE_INTEGER freq;
	if (!QueryPerformanceFrequency(&freq))
	{
		Error("QueryPerformanceFrequency failed.");
	}
	if (freq.QuadPart == 10000000)
	{
		invTickFrequency = 1.0;
		needsFrequencyScale = false;
	}
	else
	{
		invTickFrequency = 1.0/((double)freq.QuadPart/10000000.0);
		needsFrequencyScale = true;
	}
}

uint64 GetTicks()
{
	LARGE_INTEGER query;
	if (!QueryPerformanceCounter(&query))
	{
		Error("QueryPerformanceCounter failed.");
	}
	uint64 ticks = query.QuadPart;
	if (needsFrequencyScale)
	{
		ticks = (uint64)((double)ticks*invTickFrequency);
	}
	return ticks;
}

void SleepTicks(int64 ticks)
{
	__declspec(thread) static HANDLE timer;
	__declspec(thread) static bool timerCreated = false;

	if (!timerCreated)
	{
		if (!(timer = CreateWaitableTimer(NULL, TRUE, NULL)))
		{
			Error("create timer failed.");
		}
		timerCreated = true;
	}

	LARGE_INTEGER li;
	li.QuadPart = -ticks;
	if (!SetWaitableTimer(timer, &li, 0, NULL, NULL, FALSE)) {
		Error("set waitable timer failed.");
	}

	WaitForSingleObject(timer, INFINITE);
}
