#pragma once

#include "Standard.h"

typedef struct BinWriter
{
	uint8* data;
	int64 length;
	int64 dataLength;
} BinWriter;

void BinWriter_Init(BinWriter* self, int64 dataLength);
void BinWriter_Free(BinWriter* self);
void BinWriter_WriteBytes(BinWriter* self, void* source, int64 count);
