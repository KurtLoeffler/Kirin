
#pragma once

#include "common/Standard.h"
#include "draw/Texture.h"

#define TextureGLHandle(self) ((self)->internalHandle[0])
#define TextureGLBindlessHandle(self) (*((uint64*)&(self)->internalHandle[1]))

void TextureGL_Init(Texture* self, TextureInitSettings* initSettings);
void TextureGL_Free(Texture* self);
void TextureGL_SetData(Texture* self, int32 x, int32 y, int32 width, int32 height, void* data);
