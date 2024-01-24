
#include "common/Standard.h"
#include "common/Thread.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define MALLOC_TRACKALLOCATIONS CONFIGTYPE_DEV
#define MAlloc_Magic 0xcacacd35

typedef struct MAllocHeader
{
	uint32 magic0;
	struct MAllocHeader* prev;
	struct MAllocHeader* next;
	size_t size;
	uint32 magic1;
#if BITNESS_64
	uint32 pad[2];
#else
	uint32 pad[3];
#endif
} MAllocHeader;
static_assert(sizeof(MAllocHeader)%16 == 0, "size of MAllocHeader must be divisible by 16.");

MAllocHeader rootMAllocHeader;
Mutex mAllocMutex;

void* MAlloc(size_t size)
{
#if MALLOC_TRACKALLOCATIONS
	size += sizeof(MAllocHeader);
#endif

	MAllocHeader* result = malloc(size);
	if (!result)
	{
		Error("allocation failed.");
	}

#if MALLOC_TRACKALLOCATIONS
	MemSet(result, 0, sizeof(MAllocHeader));
	result->magic0 = MAlloc_Magic;
	result->magic1 = MAlloc_Magic;
	
	Mutex_Lock(&mAllocMutex);
	result->prev = &rootMAllocHeader;
	result->next = rootMAllocHeader.next;
	if (result->next)
	{
		result->next->prev = result;
	}
	rootMAllocHeader.next = result;
	Mutex_Unlock(&mAllocMutex);

	result += 1;
#endif
	return result;
}

void* MRealloc(void* block, size_t size)
{
	if (!block)
	{
		return MAlloc(size);
	}

#if MALLOC_TRACKALLOCATIONS
	// PERF: locking while reallocating.
	Mutex_Lock(&mAllocMutex);
	MAllocHeader* header = block ? ((MAllocHeader*)block)-1 : null;
	MAllocHeader* prev = null;
	MAllocHeader* next = null;
	if (header)
	{
		Assert(header->magic0 == MAlloc_Magic && header->magic1 == MAlloc_Magic);

		prev = header->prev;
		next = header->next;
		block = header;
	}
	size += sizeof(MAllocHeader);
#endif
	MAllocHeader* result = realloc(block, size);
	if (!result)
	{
		Error("allocation failed.");
	}

#if MALLOC_TRACKALLOCATIONS
	if (header && result != block)
	{
		if (prev)
		{
			prev->next = result;
		}
		if (next)
		{
			next->prev = result;
		}
	}
	Mutex_Unlock(&mAllocMutex);

	result += 1;
#endif
	return result;
}

static void RemoveMAllocHeader(MAllocHeader* header)
{
	if (header->prev)
	{
		header->prev->next = header->next;
	}
	if (header->next)
	{
		header->next->prev = header->prev;
	}
}

void MFree(void* ptr)
{
#if MALLOC_TRACKALLOCATIONS
	if (ptr)
	{
		MAllocHeader* header = ((MAllocHeader*)ptr)-1;
		Assert(header->magic0 == MAlloc_Magic && header->magic1 == MAlloc_Magic);
		Mutex_Lock(&mAllocMutex);
		RemoveMAllocHeader(header);
		Mutex_Unlock(&mAllocMutex);
		ptr = header;
	}
#endif
	free(ptr);
}

void MAlloc_NoLeakCheck(void* ptr)
{
#if MALLOC_TRACKALLOCATIONS
	MAllocHeader* header = ((MAllocHeader*)ptr)-1;
	Assert(header->magic0 == MAlloc_Magic && header->magic1 == MAlloc_Magic);
	Mutex_Lock(&mAllocMutex);
	RemoveMAllocHeader(header);
	Mutex_Unlock(&mAllocMutex);
#endif
}

bool MAlloc_DetectLeaks()
{
	bool result = false;
#if MALLOC_TRACKALLOCATIONS
	Mutex_Lock(&mAllocMutex);
	result = rootMAllocHeader.next != null;
	Mutex_Unlock(&mAllocMutex);
#endif
	return result;
}

void MemCpy(void* dest, const void* source, size_t size)
{
	memcpy(dest, source, size);
}

void MemSet(void* dest, uint8 source, size_t size)
{
	memset(dest, source, size);
}

int32 MemCmp(const void* a, const void* b, size_t size)
{
	return memcmp(a, b, size);
}


bool SPrintF(char* dest, int32 destLength, PrintFormatStringAttribute const char* format, ...)
{
	va_list args = null;
	va_start(args, format);

	size_t needed = vsnprintf(null, 0, format, args)+1;
	if ((size_t)destLength < needed)
	{
		// not enough space in dest.
		return false;
	}
	vsprintf_s(dest, needed, format, args);

	va_end(args);

	return true;
}

char* SPrintFAlloc(PrintFormatStringAttribute const char* format, ...)
{
	va_list args = null;
	va_start(args, format);

	size_t needed = vsnprintf(null, 0, format, args)+1;
	char* mem = (char*)MAlloc(needed);
	vsprintf_s(mem, needed, format, args);

	va_end(args);
	return mem;
}

bool SPrintFW(wchar_t* dest, int32 destLength, PrintFormatStringAttribute const wchar_t* format, ...)
{
	va_list args = null;
	va_start(args, format);

	size_t needed = _vsnwprintf(null, 0, format, args)+1;
	if ((size_t)destLength < needed)
	{
		// not enough space in dest.
		return false;
	}
	vswprintf_s(dest, needed, format, args);

	va_end(args);

	return true;
}

wchar_t* SPrintFWAlloc(PrintFormatStringAttribute const wchar_t* format, ...)
{
	va_list args = null;
	va_start(args, format);

	size_t needed = _vsnwprintf(null, 0, format, args)+1;
	wchar_t* mem = (wchar_t*)MAlloc(needed);
	vswprintf_s(mem, needed, format, args);

	va_end(args);
	return mem;
}

int32 SScanF(const char* buffer, ScanFormatStringAttribute const char* format, ...)
{
	va_list args = null;
	va_start(args, format);
	int32 result = vsscanf(buffer, format, args);
	va_end(args);
	return result;
}

int32 SScanFW(const wchar_t* buffer, ScanFormatStringAttribute const wchar_t* format, ...)
{
	va_list args = null;
	va_start(args, format);
	int32 result = vswscanf(buffer, format, args);
	va_end(args);
	return result;
}

void Print(const char* message)
{
	printf("%s", message);
}

void PrintLine(const char* message)
{
	printf("%s\n", message);
}

void PrintF(PrintFormatStringAttribute const char* format, ...)
{
	va_list args = null;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}


void ErrorFInternal(const char* file, int32 line, PrintFormatStringAttribute const char* format, ...)
{
	va_list args = null;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);

	if (file)
	{
		PrintLine("");

		PrintF("at %s:%d\n", file, line);
	}
}

void ErrorInternal(const char* file, int32 line, const char* message)
{
	if (message)
	{
		PrintLine(message);
	}
	if (file)
	{
		PrintF("at %s:%d\n", file, line);
	}
}
