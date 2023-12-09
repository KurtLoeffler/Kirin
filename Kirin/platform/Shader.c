#include "platform/Shader.h"

#include "common/CString.h"

int32 Shader_FindAttributeIndex(Shader* self, char* name)
{
	for (int i = 0; i < self->attributeCount; i++)
	{
		ShaderAttribute* attribute = &self->attributes[i];
		if (StrCmp(attribute->name, name, true) == 0)
		{
			return attribute->location;
		}
	}
	return -1;
}

int32 Shader_FindUniformIndex(Shader* self, char* name)
{
	for (int i = 0; i < self->uniformCount; i++)
	{
		ShaderUniform* uniform = &self->uniforms[i];
		if (StrCmp(uniform->name, name, true) == 0)
		{
			return uniform->location;
		}
	}
	return -1;
}
