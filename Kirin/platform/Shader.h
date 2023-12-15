
#pragma once

#include "common/Standard.h"

#define ShaderAttribute_MaxName 64
typedef struct ShaderAttribute
{
	char name[ShaderAttribute_MaxName];
	int32 location;
	uint32 type;
	uint32 length;
} ShaderAttribute;

#define ShaderUniform_MaxName 64
typedef struct ShaderUniform
{
	char name[ShaderUniform_MaxName];
	int32 location;
	uint32 type;
	uint32 length;
} ShaderUniform;

#define ShaderConstantBuffer_MaxName 64
typedef struct ShaderConstantBuffer
{
	char name[ShaderConstantBuffer_MaxName];
	int32 index;
	int32 bindingPoint;
} ShaderConstantBuffer;

#define Shader_MaxAttributes 32
#define Shader_MaxUniforms 64
#define Shader_MaxConstantBuffers 16
typedef struct Shader
{
	uint32 program;
	int32 attributeCount;
	ShaderAttribute attributes[Shader_MaxAttributes];
	int32 uniformCount;
	ShaderUniform uniforms[Shader_MaxUniforms];
	int32 constantBufferCount;
	ShaderConstantBuffer constantBuffers[Shader_MaxConstantBuffers];
} Shader;

int32 Shader_FindAttributeIndex(Shader* self, char* name);
int32 Shader_FindUniformIndex(Shader* self, char* name);
