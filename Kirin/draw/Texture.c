#include "draw/Texture.h"

#include "draw/Draw.h"

void Texture_Init(Texture* self, TextureInitSettings* initSettings)
{
	MemSet(self, 0, sizeof(Texture));

	TextureInitSettings newSettings = *initSettings;
	if (!newSettings.wrapMode)
	{
		newSettings.wrapMode = TextureWrapMode_Repeat;
	}
	if (!newSettings.filterMode)
	{
		newSettings.filterMode = TextureFilterMode_Linear;
	}

	Draw_GetBackend()->textureInit(self, &newSettings);
}

void Texture_Free(Texture* self)
{
	Draw_GetBackend()->textureFree(self);
}

void Texture_SetData(Texture* self, int32 x, int32 y, int32 width, int32 height, void* data)
{
	Draw_GetBackend()->textureSetData(self, x, y, width, height, data);
}
