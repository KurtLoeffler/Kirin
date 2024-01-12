#pragma once

#include "common/Defines.h"

#include <stdlib.h>

typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef __int64 int64;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned __int64 uint64;

#ifndef __cplusplus
typedef int bool;
#define true (1)
#define false (0)
#endif
#define null NULL

// including null terminator.
#define MaxPathLength 260

#define ArrayCountOf(a) (sizeof(a) / sizeof(a[0]))

void* MAlloc(size_t size);
void* MRealloc(void* block, size_t size);
void MFree(void* ptr);

void MemCpy(void* dest, const void* source, size_t size);
void MemSet(void* dest, uint8 source, size_t size);
int32 MemCmp(const void* a, const void* b, size_t size);

#if PLATFORM_WINDOWS
#define PrintFormatStringAttribute _Printf_format_string_
#define ScanFormatStringAttribute _Scanf_format_string_
#else
#define PrintFormatStringAttribute
#define ScanFormatStringAttribute
#endif

bool SPrintF(char* dest, int32 destLength, PrintFormatStringAttribute const char* format, ...);
char* SPrintFAlloc(PrintFormatStringAttribute const char* format, ...);
bool SPrintFW(wchar_t* dest, int32 destLength, PrintFormatStringAttribute const wchar_t* format, ...);
wchar_t* SPrintFWAlloc(PrintFormatStringAttribute const wchar_t* format, ...);
int32 SScanF(const char* buffer, ScanFormatStringAttribute const char* format, ...);
int32 SScanFW(const wchar_t* buffer, ScanFormatStringAttribute const wchar_t* format, ...);
void Print(const char* message);
void PrintLine(const char* message);
void PrintF(PrintFormatStringAttribute const char* format, ...);

void ErrorFInternal(const char* file, int32 line, PrintFormatStringAttribute const char* format, ...);
void ErrorInternal(const char* file, int32 line, const char* message);
#if PLATFORM_WINDOWS
#define ERROR_BREAK __debugbreak()
#else
#define ERROR_BREAK exit(1)
#endif

#define ErrorF(format, ...) { ErrorFInternal(__FILE__, __LINE__, format, __VA_ARGS__); ERROR_BREAK; }
#define Error(message) { ErrorInternal(__FILE__, __LINE__, message); ERROR_BREAK; }
#define WarningF(format, ...) { ErrorFInternal(__FILE__, __LINE__, format, __VA_ARGS__); }
#define Warning(message) { ErrorInternal(__FILE__, __LINE__, message); }
#define AssertMessage(expression, message) if (!(expression)) { ErrorF("assert failed: %s", message); }
#define Assert(expression) if (!(expression)) { ErrorF("assert failed: (%s)", #expression); }
#if CONFIGTYPE_DEV
// removed from release config.
#define DevErrorF(format, ...) ErrorF(format, __VA_ARGS__)
// removed from release config.
#define DevError(message) Error(message)
// removed from release config.
#define DevWarningF(format, ...) WarningF(format, __VA_ARGS__)
// removed from release config.
#define DevWarning(message) Warning(message)
// removed from release config.
#define DevAssertMessage(expression, message) AssertMessage(expression, message)
// removed from release config.
#define DevAssert(expression) Assert(expression)
#else
// removed from release config.
#define DevErrorF(format, ...)
// removed from release config.
#define DevError(message)
// removed from release config.
#define DevWarningF(format, ...)
// removed from release config.
#define DevWarning(message)
// removed from release config.
#define DevAssertMessage(expression, message)
// removed from release config.
#define DevAssert(expression)
#endif
