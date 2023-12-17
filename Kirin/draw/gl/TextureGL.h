
#pragma once

#include "common/Standard.h"
#include "draw/Texture.h"

void TextureGL_Init(Texture* self, TextureInitSettings* initSettings);
void TextureGL_Free(Texture* self);
void TextureGL_SetData(Texture* self, int32 x, int32 y, int32 width, int32 height, void* data);
