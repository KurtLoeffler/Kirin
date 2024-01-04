#pragma once

#include "common/Standard.h"
#include "draw/Mesh.h"

void VertexBufferGL_Init(VertexBuffer* self, VertexBufferUsage usage);
void VertexBufferGL_UpdateData(VertexBuffer* self, int32 offset, int32 size, void* data);
void VertexBufferGL_Free(VertexBuffer* self);

void MeshGL_Init(Mesh* mesh);
void MeshGL_ApplyStructure(Mesh* mesh);
void MeshGL_Draw(Mesh* self, int32 vertexOffset, int32 vertexCount);
void MeshGL_Free(Mesh* mesh);
