#include "platform/ConstantBuffer.h"

#include "platform/gl/CommonGL.h"

#include "thirdparty/glad/glad.h"

void ConstantBufferGL_Init(ConstantBuffer* self, int64 size)
{
	glGenBuffers(1, &self->internalHandle);
	glBindBuffer(GL_UNIFORM_BUFFER, self->internalHandle);
	glBufferData(GL_UNIFORM_BUFFER, (size_t)size, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	CheckGLError();
}

void ConstantBufferGL_AttachToShader(ConstantBuffer* self, Shader* shader, ShaderConstantBuffer* constantBuffer)
{
	// we don't actually need to bind to a specific shader in opengl.
	// just bind to the binding point that the shader uniform buffer is assigned to.
	glBindBufferBase(GL_UNIFORM_BUFFER, constantBuffer->bindingPoint, self->internalHandle);
	CheckGLError();
}

void ConstantBufferGL_SetData(ConstantBuffer* self, int64 offset, int64 length, void* data)
{
	glBindBuffer(GL_UNIFORM_BUFFER, self->internalHandle);
	glBufferSubData(GL_UNIFORM_BUFFER, (size_t)offset, (size_t)length, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	CheckGLError();
}

void ConstantBufferGL_Free(ConstantBuffer* self)
{
	if (self->internalHandle)
	{
		glDeleteBuffers(1, &self->internalHandle);
		CheckGLError();
		self->internalHandle = 0;
	}
}