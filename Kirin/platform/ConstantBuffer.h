#pragma once

#include "common/Standard.h"

#include "platform/Shader.h"

typedef struct ConstantBuffer
{
	int32 internalHandle;
} ConstantBuffer;

void ConstantBuffer_Init(ConstantBuffer* self, int64 size);
void ConstantBuffer_AttachToShader(ConstantBuffer* self, Shader* shader, ShaderConstantBuffer* constantBuffer);
void ConstantBuffer_SetData(ConstantBuffer* self, int64 offset, int64 length, void* data);
void ConstantBuffer_Free(ConstantBuffer* self);
