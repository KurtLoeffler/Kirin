#pragma once

#include "common/Standard.h"
#include "draw/Shader.h"

bool ShaderGL_Load(const char* vertPath, const char* fragPath, const char* vertPrefix, const char* fragPrefix, char** outVertStr, char** outFragStr);
bool ShaderGL_Compile(const char* vertStr, const char* fragStr, const char* vertName, const char* fragName, Shader* shader);
void ShaderGL_Free(Shader* self);
