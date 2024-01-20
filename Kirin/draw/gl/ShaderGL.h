#pragma once

#include "common/Standard.h"
#include "draw/Shader.h"

bool ShaderGL_Load(const char* vertPath, const char* fragPath, const char* vertPrefix, const char* fragPrefix, char** outVertStr, char** outFragStr);
bool ShaderGL_Compile(const char* vertStr, const char* fragStr, const char* vertName, const char* fragName, Shader* shader);
void ShaderGL_Free(Shader* self);
void ShaderGL_SetUniformInt(Shader* self, ShaderUniform* uniform, int32 arrayIndex, int32 value);
void ShaderGL_SetUniformFloat(Shader* self, ShaderUniform* uniform, int32 arrayIndex, float value);
void ShaderGL_SetUniformTexture(Shader* self, ShaderUniform* uniform, int32 arrayIndex, Texture* value);
