
#pragma once

#include "common/Standard.h"

typedef enum TextureType
{
	TextureType_None,
	TextureType_1D,
	TextureType_2D,
	TextureType_3D,
	TextureType_FrameBuffer,
	TextureType_Count,
} TextureType;

static const char* TextureType_ToString(TextureType value)
{
	switch (value) {
	case TextureType_None: return "TextureType_None"; break;
	case TextureType_1D: return "TextureType_1D"; break;
	case TextureType_2D: return "TextureType_2D"; break;
	case TextureType_3D: return "TextureType_3D"; break;
	case TextureType_FrameBuffer: return "TextureType_FrameBuffer"; break;
	default: return "INVALID"; break;
	}
	static_assert(TextureType_Count == 5, "enum has changed.");
}

typedef enum TextureFormat
{
	TextureFormat_None,
	TextureFormat_RGB8,
	TextureFormat_RGBA8,
	TextureFormat_Count,
} TextureFormat;

static const char* TextureFormat_ToString(TextureFormat value)
{
	switch (value) {
	case TextureFormat_None: return "TextureFormat_None"; break;
	case TextureFormat_RGB8: return "TextureFormat_RGB8"; break;
	case TextureFormat_RGBA8: return "TextureFormat_RGBA8"; break;
	default: return "INVALID"; break;
	}
	static_assert(TextureFormat_Count == 3, "enum has changed.");
}

typedef enum TextureWrapMode
{
	TextureWrapMode_None,
	TextureWrapMode_Repeat,
	TextureWrapMode_Clamp,
	TextureWrapMode_Count,
} TextureWrapMode;

static const char* TextureWrapMode_ToString(TextureWrapMode value)
{
	switch (value) {
	case TextureWrapMode_None: return "TextureWrapMode_None"; break;
	case TextureWrapMode_Repeat: return "TextureWrapMode_Repeat"; break;
	case TextureWrapMode_Clamp: return "TextureWrapMode_Clamp"; break;
	default: return "INVALID"; break;
	}
	static_assert(TextureWrapMode_Count == 3, "enum has changed.");
}

typedef enum TextureFilterMode
{
	TextureFilterMode_None,
	TextureFilterMode_Nearest,
	TextureFilterMode_Linear,
	TextureFilterMode_Count,
} TextureFilterMode;

static const char* TextureFilterMode_ToString(TextureFilterMode value)
{
	switch (value) {
	case TextureFilterMode_None: return "TextureFilterMode_None"; break;
	case TextureFilterMode_Nearest: return "TextureFilterMode_Nearest"; break;
	case TextureFilterMode_Linear: return "TextureFilterMode_Linear"; break;
	default: return "INVALID"; break;
	}
	static_assert(TextureFilterMode_Count == 3, "enum has changed.");
}

typedef struct TextureInitSettings
{
	TextureType type;
	TextureFormat format;
	TextureWrapMode wrapMode;
	TextureFilterMode filterMode;
	TextureFilterMode mipFilterMode;
	// 0 or 1 is disabled. enable with powers of 2, or -1 to use highest supported value.
	int32 anisotropy;
	int32 width;
	int32 height;
	int32 depth;
} TextureInitSettings;

typedef struct Texture
{
	TextureType type;
	TextureFormat format;
	TextureWrapMode wrapMode;
	TextureFilterMode filterMode;
	TextureFilterMode mipFilterMode;
	// 0 or 1 is disabled. enable with powers of 2, or -1 to use highest supported value.
	int32 anisotropy;
	int32 width;
	int32 height;
	int32 depth;

	bool hasMipmaps;
	int32 internalHandle[4];
} Texture;

void Texture_Init(Texture* self, TextureInitSettings* initSettings);
void Texture_Free(Texture* self);
void Texture_SetData(Texture* self, int32 x, int32 y, int32 width, int32 height, void* data);
void Texture_GenerateMipmaps(Texture* self);
