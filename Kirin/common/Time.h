#pragma once

#include "common/Standard.h"

void InitTime();

// get tick counter. there are 10000 ticks in 1 milisecond.
uint64 GetTicks();

// sleep for a number of ticks. there are 10000 ticks in 1 milisecond.
void SleepTicks(int64 ticks);
