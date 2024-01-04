#include "draw/Mesh.h"

#include "draw/Draw.h"

void VertexBuffer_Init(VertexBuffer* self, int32 sizeInBytes, VertexBufferUsage usage)
{
	*self = (VertexBuffer){ 0 };
	self->sizeInBytes = sizeInBytes;

	Draw_GetBackend()->vertexBufferInit(self, usage);
}

void VertexBuffer_SetData(VertexBuffer* self, int32 offset, int32 size, void* data)
{
	Draw_GetBackend()->vertexBufferUpdateData(self, offset, size, data);
}

void VertexBuffer_Free(VertexBuffer* self)
{
	Draw_GetBackend()->vertexBufferFree(self);
}

void Mesh_Init(Mesh* self)
{
	*self = (Mesh){ 0 };

	Draw_GetBackend()->meshInit(self);
}

void Mesh_ApplyStructure(Mesh* self)
{
	Draw_GetBackend()->meshApplyStructure(self);
}

void Mesh_Draw(Mesh* self, int32 vertexOffset, int32 vertexCount)
{
	Draw_Flush();
	Draw_GetBackend()->meshDraw(self, vertexOffset, vertexCount);
}

void Mesh_Free(Mesh* self)
{
	for (int32 i = 0; i < self->vertexBufferCount; i++)
	{
		VertexBuffer_Free(&self->vertexBuffers[i]);
	}
	Draw_GetBackend()->meshFree(self);
}
