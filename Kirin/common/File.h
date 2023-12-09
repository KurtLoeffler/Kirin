#pragma once

#include "common/Standard.h"

typedef enum FileMode
{
	FileMode_None = 0,
	FileMode_Read,
	FileMode_Write,
	FileMode_Append,
	FileMode_Count
} FileMode;

static const char* FileMode_ToString(FileMode value)
{
	switch (value) {
	case FileMode_None: return "FileMode_None"; break;
	case FileMode_Read: return "FileMode_Read"; break;
	case FileMode_Write: return "FileMode_Write"; break;
	case FileMode_Append: return "FileMode_Append"; break;
	default: return "INVALID"; break;
	}
	static_assert(FileMode_Count == 4, "enum has changed.");
}

typedef struct File
{
	FileMode mode;
	void* fileHandle;
} File;

static void File_Init(File* self)
{
	self->mode = FileMode_None;
	self->fileHandle = null;
}

bool File_Open(File* self, const char* path, FileMode mode);
bool File_OpenW(File* self, const wchar_t* path, FileMode mode);

int64 File_GetSize(File* self);
int64 File_GetOffset(File* self);
void File_SetOffset(File* self, int64 value);
int64 File_ReadBinary(File* self, uint8* dest, int64 size);
int64 File_WriteBinary(File* self, const uint8* source, int64 size);
void File_Close(File* self);

bool File_WriteBinaryFile(const char* path, const uint8* data, int64 size);
bool File_WriteBinaryFileW(const wchar_t* path, const uint8* data, int64 size);

uint8* File_ReadBinaryFile(const char* path, int64* outSize);
uint8* File_ReadBinaryFileW(const wchar_t* path, int64* outSize);

char* File_ReadCStringFile(const char* path, int64* outLength);
char* File_ReadCStringFileW(const wchar_t* path, int64* outLength);

wchar_t* File_ReadWideCStringFile(const char* path, int64* outLength);
wchar_t* File_ReadWideCStringFileW(const wchar_t* path, int64* outLength);
