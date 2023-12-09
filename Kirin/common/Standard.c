
#include "common/Standard.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void* MAlloc(size_t size)
{
	void* result = malloc(size);
	if (!result)
	{
		Error("allocation failed.");
	}
	return result;
}

void* MRealloc(void* block, size_t size)
{
	void* result = realloc(block, size);
	if (!result)
	{
		Error("allocation failed.");
	}
	return result;
}

void MFree(void* ptr)
{
	free(ptr);
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

bool WSPrintF(wchar_t* dest, int32 destLength, PrintFormatStringAttribute const wchar_t* format, ...)
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

wchar_t* WSPrintFAlloc(PrintFormatStringAttribute const wchar_t* format, ...)
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

int32 WSScanF(const wchar_t* buffer, ScanFormatStringAttribute const wchar_t* format, ...)
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
