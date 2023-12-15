#pragma once

#include "common/Standard.h"
#include "platform/ConstantBuffer.h"

void ConstantBufferGL_Init(ConstantBuffer* self, int64 size);
void ConstantBufferGL_AttachToShader(ConstantBuffer* self, Shader* shader, ShaderConstantBuffer* constantBuffer);
void ConstantBufferGL_SetData(ConstantBuffer* self, int64 offset, int64 length, void* data);
void ConstantBufferGL_Free(ConstantBuffer* self);
