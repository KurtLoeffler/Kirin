#pragma once

#include "common/Standard.h"

typedef enum VertexFormatType {
	VertexFormatType_None,
	VertexFormatType_Float,
	VertexFormatType_Byte,
	VertexFormatType_Count,
} VertexFormatType;

static const char* VertexFormatType_ToString(VertexFormatType value)
{
	switch (value) {
	case VertexFormatType_None: return "VertexFormatType_None"; break;
	case VertexFormatType_Float: return "VertexFormatType_Float"; break;
	case VertexFormatType_Byte: return "VertexFormatType_Byte"; break;
	default: return "INVALID"; break;
	}
	static_assert(VertexFormatType_Count == 3, "enum has changed.");
}

static const int32 VertexFormatType_ToSize(VertexFormatType value)
{
	switch (value) {
	case VertexFormatType_Float: return 4; break;
	case VertexFormatType_Byte: return 1; break;
	default: ErrorF("cannot get size of VertexFormatType \"%s\".", VertexFormatType_ToString(value)); break;
	}
	static_assert(VertexFormatType_Count == 3, "enum has changed.");
	return 0;
}

typedef enum VertexBufferUsage {
	VertexBufferUsage_None,
	VertexBufferUsage_Static,
	VertexBufferUsage_Dynamic,
	VertexBufferUsage_Count,
} VertexBufferUsage;

static const char* VertexBufferUsage_ToString(VertexBufferUsage value)
{
	switch (value) {
	case VertexBufferUsage_None: return "VertexBufferUsage_None"; break;
	case VertexBufferUsage_Static: return "VertexBufferUsage_Static"; break;
	case VertexBufferUsage_Dynamic: return "VertexBufferUsage_Dynamic"; break;
	default: return "INVALID"; break;
	}
	static_assert(VertexBufferUsage_Count == 3, "enum has changed.");
}

typedef struct VertexFormatItem
{
	int32 bufferIndex;
	int32 offset;
	int32 stride;
	VertexFormatType type;
	int32 componentCount;
} VertexFormatItem;

typedef struct VertexBuffer {
	int32 internalHandle;
	int32 sizeInBytes;
} VertexBuffer;

#define Mesh_MaxVertexFormatItems 32
#define Mesh_MaxVertexBuffers 16
typedef struct Mesh {
	int32 internalHandle;
	int32 vertexCount;
	int32 vertexFormatCount;
	VertexFormatItem vertexFormat[Mesh_MaxVertexFormatItems];
	int32 vertexBufferCount;
	VertexBuffer vertexBuffers[Mesh_MaxVertexBuffers];
} Mesh;

void VertexBuffer_Init(VertexBuffer* self, int32 sizeInBytes, VertexBufferUsage usage);
void VertexBuffer_SetData(VertexBuffer* self, int32 offset, int32 size, void* data);
void VertexBuffer_Free(VertexBuffer* self);
void Mesh_Init(Mesh* self);
void Mesh_ApplyStructure(Mesh* self);
void Mesh_Draw(Mesh* self, int32 vertexOffset, int32 vertexCount);
void Mesh_Free(Mesh* self);
