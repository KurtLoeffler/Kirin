#include "draw/Mesh.h"

#include "draw/Draw.h"

void VertexBuffer_Init(VertexBuffer* self, int32 sizeInBytes, VertexBufferUsage usage)
{
	*self = (VertexBuffer){ 0 };
	self->sizeInBytes = sizeInBytes;

	Draw_GetBackend()->initVertexBuffer(self, usage);
}

void VertexBuffer_SetData(VertexBuffer* self, int32 offset, int32 size, void* data)
{
	Draw_GetBackend()->updateVertexBufferData(self, offset, size, data);
}

void VertexBuffer_Free(VertexBuffer* self)
{
	Draw_GetBackend()->freeVertexBuffer(self);
}

void Mesh_Init(Mesh* self)
{
	*self = (Mesh){ 0 };

	Draw_GetBackend()->initMesh(self);
}

void Mesh_ApplyStructure(Mesh* self)
{
	Draw_GetBackend()->applyMeshStructure(self);
}

void Mesh_Free(Mesh* self)
{
	for (int32 i = 0; i < self->vertexBufferCount; i++)
	{
		VertexBuffer_Free(&self->vertexBuffers[i]);
	}
	Draw_GetBackend()->freeMesh(self);
}
