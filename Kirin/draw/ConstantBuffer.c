#include "draw/ConstantBuffer.h"

#include "draw/Draw.h"

void ConstantBuffer_Init(ConstantBuffer* self, int64 size)
{
	Draw_GetBackend()->constantBufferInit(self, size);
}

void ConstantBuffer_AttachToShader(ConstantBuffer* self, Shader* shader, ShaderConstantBuffer* constantBuffer)
{
	Draw_GetBackend()->constantBufferAttachToShader(self, shader, constantBuffer);
}

void ConstantBuffer_SetData(ConstantBuffer* self, int64 offset, int64 length, void* data)
{
	Draw_Flush();
	Draw_GetBackend()->constantBufferSetData(self, offset, length, data);
}

void ConstantBuffer_Free(ConstantBuffer* self)
{
	Draw_Flush();
	Draw_GetBackend()->constantBufferFree(self);
}
