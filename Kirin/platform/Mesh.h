#pragma once

#include "common/Standard.h"
#include "platform/Draw.h"

void VertexBuffer_Init(VertexBuffer* self, int32 sizeInBytes, VertexBufferUsage usage);
void VertexBuffer_SetData(VertexBuffer* self, int32 offset, int32 size, void* data);
void VertexBuffer_Free(VertexBuffer* self);
void Mesh_Init(Mesh* self);
void Mesh_ApplyStructure(Mesh* self);
void Mesh_Free(Mesh* self);
