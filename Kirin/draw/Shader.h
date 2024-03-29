
#pragma once

#include "common/Standard.h"
#include "draw/Texture.h"

#define ShaderAttribute_MaxName 64
typedef struct ShaderAttribute
{
	char name[ShaderAttribute_MaxName];
	int32 location;
	uint32 type;
} ShaderAttribute;

#define ShaderUniform_MaxName 64
typedef struct ShaderUniform
{
	char name[ShaderUniform_MaxName];
	int32 location;
	uint32 type;
	bool isArray;
	uint32 arrayCount;
} ShaderUniform;

#define ShaderConstantBuffer_MaxName 64
typedef struct ShaderConstantBuffer
{
	char name[ShaderConstantBuffer_MaxName];
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

bool Shader_Load(const char* path, Shader* shader);
void Shader_Free(Shader* shader);
int32 Shader_FindAttributeIndex(Shader* self, char* name);
int32 Shader_FindUniformIndex(Shader* self, char* name);
int32 Shader_FindConstantBufferIndex(Shader* self, char* name);
ShaderAttribute* Shader_FindAttribute(Shader* self, char* name);
ShaderUniform* Shader_FindUniform(Shader* self, char* name);
ShaderConstantBuffer* Shader_FindConstantBuffer(Shader* self, char* name);
void Shader_SetUniformInt(Shader* self, ShaderUniform* uniform, int32 arrayIndex, int32 value);
void Shader_SetUniformFloat(Shader* self, ShaderUniform* uniform, int32 arrayIndex, float value);
void Shader_SetUniformTexture(Shader* self, ShaderUniform* uniform, int32 arrayIndex, Texture* value);
