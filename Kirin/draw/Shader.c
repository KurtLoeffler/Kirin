#include "draw/Shader.h"

#include "common/CString.h"

int32 Shader_FindAttributeIndex(Shader* self, char* name)
{
	for (int i = 0; i < self->attributeCount; i++)
	{
		ShaderAttribute* attribute = &self->attributes[i];
		if (StrCmp(attribute->name, name, true) == 0)
		{
			return i;
		}
	}
	return -1;
}

ShaderAttribute* Shader_FindAttribute(Shader* self, char* name)
{
	int32 index = Shader_FindAttributeIndex(self, name);
	return index >= 0 ? &self->attributes[index] : null;
}

int32 Shader_FindUniformIndex(Shader* self, char* name)
{
	for (int i = 0; i < self->uniformCount; i++)
	{
		ShaderUniform* uniform = &self->uniforms[i];
		if (StrCmp(uniform->name, name, true) == 0)
		{
			return i;
		}
	}
	return -1;
}

ShaderUniform* Shader_FindUniform(Shader* self, char* name)
{
	int32 index = Shader_FindUniformIndex(self, name);
	return index >= 0 ? &self->uniforms[index] : null;
}

int32 Shader_FindConstantBufferIndex(Shader* self, char* name)
{
	for (int i = 0; i < self->constantBufferCount; i++)
	{
		ShaderConstantBuffer* constantBuffer = &self->constantBuffers[i];
		if (StrCmp(constantBuffer->name, name, true) == 0)
		{
			return i;
		}
	}
	return -1;
}

ShaderConstantBuffer* Shader_FindConstantBuffer(Shader* self, char* name)
{
	int32 index = Shader_FindConstantBufferIndex(self, name);
	return index >= 0 ? &self->constantBuffers[index] : null;
}

void Shader_SetUniformInt(Shader* self, ShaderUniform* uniform)
{

}