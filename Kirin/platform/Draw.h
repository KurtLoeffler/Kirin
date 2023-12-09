#pragma once

#include "common/Standard.h"
#include "platform/DrawBackend.h"

void Draw_Init(DrawBackend* backend);
void Draw_Free();
bool Draw_LoadShader(const char* path, Shader* shader);
void Draw_FreeShader(Shader* shader);
DrawState* Draw_GetDrawState();
void Draw_SetImmediateVertexFormat(VertexFormatItem* format, int32 count);
void Draw_CommitDrawState();
void Draw_Flush();
void Draw_SubmitImmediatePoly(const void* vertices, int32 vertexCount);
void Draw_SetViewMatrix(const Matrix4* matrix);
void Draw_SetProjectionMatrix(const Matrix4* matrix);
void Draw_SetViewport(int32 x, int32 y, int32 width, int32 height);
void Draw_ClearColor(float r, float g, float b, float a);
void Draw_ClearDepth(float value);
void Draw_ClearStencil(int32 value);

void VertexBuffer_Init(VertexBuffer* self, int32 sizeInBytes, VertexBufferUsage usage);
void VertexBuffer_SetData(VertexBuffer* self, int32 offset, int32 size, void* data);
void VertexBuffer_Free(VertexBuffer* self);
void Mesh_Init(Mesh* self);
void Mesh_ApplyStructure(Mesh* self);
void Mesh_Free(Mesh* self);
