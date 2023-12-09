#include "common/BinWriter.h"

#include "common/Math.h"

void BinWriter_Init(BinWriter* self, int64 dataLength)
{
	*self = (BinWriter){ 0 };
	self->dataLength = dataLength;
	self->data = MAlloc((size_t)dataLength);
}

void BinWriter_Free(BinWriter* self)
{
	if (self->data)
	{
		MFree(self->data);
		self->data = null;
	}
}

void BinWriter_WriteBytes(BinWriter* self, void* source, int64 size)
{
	if (self->length+size > self->dataLength)
	{
		self->dataLength = (int64)Ceil((self->length+size)*1.5f);
		self->data = MRealloc(self->data, (size_t)self->dataLength);
	}

	MemCpy(self->data, source, (size_t)size);
	self->length += size;
}
