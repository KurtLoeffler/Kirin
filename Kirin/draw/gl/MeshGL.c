#include "draw/gl/MeshGL.h"

#include "draw/gl/CommonGL.h"

#include "thirdparty/glad/glad.h"

void VertexBufferGL_Init(VertexBuffer* self, VertexBufferUsage usage)
{
	static int32 usageToGLUsage[] = {
		[VertexBufferUsage_Static] = GL_STATIC_DRAW,
		[VertexBufferUsage_Dynamic] = GL_DYNAMIC_DRAW,
	};

	glGenBuffers(1, &self->internalHandle);
	glBindBuffer(GL_ARRAY_BUFFER, self->internalHandle);
	glBufferData(GL_ARRAY_BUFFER, self->sizeInBytes, null, usageToGLUsage[usage]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CheckGLError();
}

void VertexBufferGL_UpdateData(VertexBuffer* self, int32 offset, int32 size, void* data)
{
	glBindBuffer(GL_ARRAY_BUFFER, self->internalHandle);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CheckGLError();
}

void VertexBufferGL_Free(VertexBuffer* self)
{
	if (self->internalHandle)
	{
		glDeleteBuffers(1, &self->internalHandle);
		self->internalHandle = 0;
	}
}

void MeshGL_Init(Mesh* self)
{
	glGenVertexArrays(1, &self->internalHandle);
	CheckGLError();
}

void MeshGL_ApplyStructure(Mesh* self)
{
	static int32 typeToGLType[] = {
		[VertexFormatType_Float] = GL_FLOAT,
		[VertexFormatType_Byte] = GL_UNSIGNED_BYTE,
	};
	static_assert(VertexFormatType_Count == 3, "enum has changed.");

#if CONFIG_DEBUG
	if (self->internalHandle == 0)
	{
		Error("mesh internalHandle is 0.");
	}
#endif

	glBindVertexArray(self->internalHandle);
	CheckGLError();

	int32 lastBoundBuffer = 0;
	for (int32 i = 0; i < self->vertexFormatCount; i++)
	{
		VertexFormatItem* item = &self->vertexFormat[i];
#if CONFIG_DEBUG
		if (item->bufferIndex < 0 || item->bufferIndex >= self->vertexBufferCount)
		{
			ErrorF("VertexFormatItem bufferIndex is out of range. bufferIndex: %d vertexBufferCount: %d", item->bufferIndex, self->vertexBufferCount);
		}
#endif
		VertexBuffer* vertexBuffer = &self->vertexBuffers[item->bufferIndex];

#if CONFIG_DEBUG
		if (vertexBuffer->internalHandle == 0)
		{
			Error("vertexBuffer internalHandle is 0.");
		}
#endif
		// bind the the formats associated vertex buffer.
		if (lastBoundBuffer != vertexBuffer->internalHandle)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->internalHandle);
			CheckGLError();
		}

		// configure vertex buffer with vertex format.
		glVertexAttribPointer(item->inputIndex, item->componentCount, typeToGLType[item->type], GL_FALSE, item->stride, (void*)(size_t)item->offset);
		CheckGLError();

		// enable vertex attribute layout location.
		glEnableVertexAttribArray(item->inputIndex);
		CheckGLError();
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	CheckGLError();
}

void MeshGL_Draw(Mesh* self, int32 vertexOffset, int32 vertexCount)
{
	glBindVertexArray(self->internalHandle);
	CheckGLError();

	if (false /*programmable blending*/)
	{
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
	}

	glDrawArrays(GL_TRIANGLES, vertexOffset, vertexCount);
	CheckGLError();
}

void MeshGL_Free(Mesh* self)
{
	if (self->internalHandle)
	{
		glDeleteVertexArrays(1, &self->internalHandle);
		CheckGLError();
		self->internalHandle = 0;
	}
}
