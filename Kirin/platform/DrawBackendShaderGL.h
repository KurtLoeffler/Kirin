#pragma once

#include "common/Standard.h"
#include "platform/Shader.h"

bool DrawBackendShaderGL_Load(const char* vertPath, const char* fragPath, const char* vertPrefix, const char* fragPrefix, char** outVertStr, char** outFragStr);
bool DrawBackendShaderGL_Compile(const char* vertStr, const char* fragStr, const char* vertName, const char* fragName, Shader* shader);
void DrawBackendShaderGL_Free(Shader* self);
