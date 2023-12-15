#pragma once

#include "common/Standard.h"
#include "platform/DrawBackend.h"

void Draw_Init(DrawBackend* backend);
void Draw_Free();
bool Draw_LoadShader(const char* path, Shader* shader);
void Draw_FreeShader(Shader* shader);
DrawBackend* Draw_GetBackend();
DrawState* Draw_GetDrawState();
void Draw_SetImmediateVertexFormat(VertexFormatItem* format, int32 count);
void Draw_CommitDrawState();
void Draw_Flush();
void Draw_SubmitImmediatePoly(const void* vertices, int32 vertexCount);
void Draw_SetViewport(int32 x, int32 y, int32 width, int32 height);
void Draw_ClearColor(float r, float g, float b, float a);
void Draw_ClearDepth(float value);
void Draw_ClearStencil(int32 value);
