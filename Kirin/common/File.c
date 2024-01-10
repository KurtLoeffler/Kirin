#include "common/File.h"

#include "common/Math.h"
#include "common/CString.h"

#include <stdio.h>

const wchar_t* FileModeToCMode(FileMode mode)
{
	static_assert((int)FileMode_Count == 4, "FileMode_Count has changed.");
	switch (mode)
	{
	case FileMode_Read:
		return L"rb";
	case FileMode_Write:
		return L"wb";
	case FileMode_Append:
		return L"a+";
	default:
		ErrorF("FileModeToCMode: invalid FileMode %s (%x).", FileMode_ToString(mode), mode);
		break;
	}

	return L"";
}

bool File_Open(File* self, const char* path, FileMode mode)
{
	wchar_t* wpath = CStringToWideStringAlloc(path, -1);
	bool result = File_OpenW(self, wpath, mode);
	MFree(wpath);
	return result;
}

bool File_OpenW(File* self, const wchar_t* path, FileMode mode)
{
	self->mode = mode;

	static_assert((int)FileMode_Count == 4, "FileMode_Count has changed.");
	if (mode != FileMode_Read && mode != FileMode_Write && mode != FileMode_Append)
	{
		Error("mode must be FileMode_Read, FileMode_Write, or FileMode_Append.");
	}

	self->fileHandle = _wfopen(path, FileModeToCMode(mode));
	if (!self->fileHandle)
	{
		return false;
	}

	return true;
}

int64 File_GetSize(File* self)
{
	if (!self->fileHandle)
	{
		ErrorF("file is not open.");
		return 0;
	}

	int64 prevOffset = File_GetOffset(self);

	_fseeki64((FILE*)self->fileHandle, 0L, SEEK_END);
	int64 size = _ftelli64((FILE*)self->fileHandle);

	if (size < 0)
	{
		size = 0;
	}
	File_SetOffset(self, prevOffset);

	return size;
}

int64 File_GetOffset(File* self)
{
	if (!self->fileHandle)
	{
		ErrorF("file is not open.");
	}
	return _ftelli64((FILE*)self->fileHandle);
}

void File_SetOffset(File* self, int64 value)
{
	if (!self->fileHandle)
	{
		ErrorF("file is not open.");
	}
	_fseeki64((FILE*)self->fileHandle, value, SEEK_SET);
}

int64 File_ReadBinary(File* self, uint8* dest, int64 size)
{
	if (!self->fileHandle)
	{
		ErrorF("file is not open.");
	}

	if (!dest)
	{
		ErrorF("dest is null.");
	}

	int64 count = fread(dest, 1, (size_t)size, (FILE*)self->fileHandle);
	return count;
}

int64 File_WriteBinary(File* self, const uint8* source, int64 size)
{
	if (!self->fileHandle)
	{
		ErrorF("file is not open.");
	}

	if (!source)
	{
		ErrorF("source is null.");
	}

	int64 count = fwrite(source, 1, (size_t)size, (FILE*)self->fileHandle);
	return count;
}

void File_Close(File* self)
{
	if (self->fileHandle)
	{
		fclose((FILE*)self->fileHandle);
		self->fileHandle = null;
	}
}

bool File_WriteBinaryFile(const char* path, const uint8* data, int64 size)
{
	wchar_t* wpath = CStringToWideStringAlloc(path, -1);
	bool result = File_WriteBinaryFileW(wpath, data, size);
	MFree(wpath);
	return result;
}

bool File_WriteBinaryFileW(const wchar_t* path, const uint8* data, int64 size)
{
	if (data == null && size > 0)
	{
		Error("data is null but size is > 0.");
	}
	size = MaxI64(size, 0ll);

	File file;
	if (!File_OpenW(&file, path, FileMode_Write))
	{
		return false;
	}
	if (data != null)
	{
		File_WriteBinary(&file, data, size);
	}
	File_Close(&file);
	return true;
}

uint8* File_ReadBinaryFileAlloc(const char* path, int64* outSize)
{
	wchar_t* wpath = CStringToWideStringAlloc(path, -1);
	uint8* result = File_ReadBinaryFileWAlloc(wpath, outSize);
	MFree(wpath);
	return result;
}

uint8* File_ReadBinaryFileWAlloc(const wchar_t* path, int64* outSize)
{
	File file;
	if (!File_OpenW(&file, path, FileMode_Read))
	{
		return null;
	}
	int64 size = File_GetSize(&file);
	uint8* data = (uint8*)MAlloc((size_t)size);
	File_ReadBinary(&file, data, size);
	File_Close(&file);

	if (outSize)
	{
		*outSize = size;
	}

	return data;
}

static void* ReadCStringFileGeneric(const wchar_t* path, size_t charSize, int64* outLength)
{
	File file;
	if (!File_OpenW(&file, path, FileMode_Read))
	{
		return null;
	}
	int64 size = File_GetSize(&file);
	uint8* data = (uint8*)MAlloc((size_t)size+charSize);
	File_ReadBinary(&file, data, size);
	File_Close(&file);
	
	MemSet(data+size, 0, charSize);

	if (outLength)
	{
		*outLength = size/charSize;
	}

	return data;
}

char* File_ReadCStringFileAlloc(const char* path, int64* outLength)
{
	wchar_t* wpath = CStringToWideStringAlloc(path, -1);
	char* result = File_ReadCStringFileWAlloc(wpath, outLength);
	MFree(wpath);
	return result;
}

char* File_ReadCStringFileWAlloc(const wchar_t* path, int64* outLength)
{
	return (char*)ReadCStringFileGeneric(path, sizeof(char), outLength);
}

wchar_t* File_ReadWideCStringFileAlloc(const char* path, int64* outLength)
{
	wchar_t* wpath = CStringToWideStringAlloc(path, -1);
	wchar_t* result = File_ReadWideCStringFileWAlloc(wpath, outLength);
	MFree(wpath);
	return result;
}

wchar_t* File_ReadWideCStringFileWAlloc(const wchar_t* path, int64* outLength)
{
	return (wchar_t*)ReadCStringFileGeneric(path, sizeof(wchar_t), outLength);
}