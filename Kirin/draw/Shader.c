#include "draw/Shader.h"

#include "common/CString.h"
#include "draw/Draw.h"

bool Shader_Load(const char* path, Shader* shader)
{
	*shader = (Shader){ 0 };

	PrintF("loading shader \"%s\"\n", path);
	if (!Draw_GetBackend()->shaderLoad(path, shader))
	{
		Error("shader load failed.");
	}
	return true;
}

void Shader_Free(Shader* shader)
{
	if (Draw_GetShader() == shader)
	{
		Draw_SetShader(null);
	}
	Draw_GetBackend()->shaderFree(shader);
}

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

void Shader_SetUniformInt(Shader* self, ShaderUniform* uniform, int32 arrayIndex, int32 value)
{
	// PERF: do we only need to flush if this is the current draw shader?
	Draw_Flush();
	Draw_GetBackend()->shaderSetUniformInt(self, uniform, arrayIndex, value);
}

void Shader_SetUniformTexture(Shader* self, ShaderUniform* uniform, int32 arrayIndex, Texture* value)
{
	Draw_Flush();
	Draw_GetBackend()->shaderSetUniformTexture(self, uniform, arrayIndex, value);
}
