#include "draw/Texture.h"

#include "draw/Draw.h"

void Texture_Init(Texture* self, TextureInitSettings* initSettings)
{
	MemSet(self, 0, sizeof(Texture));

	TextureInitSettings newSettings = *initSettings;
	if (!newSettings.type)
	{
		newSettings.type = TextureType_2D;
	}
	if (!newSettings.wrapMode)
	{
		newSettings.wrapMode = TextureWrapMode_Repeat;
	}
	if (!newSettings.filterMode)
	{
		newSettings.filterMode = TextureFilterMode_Linear;
	}
	AssertMessage(newSettings.format != TextureFormat_None, "a texture format must be specified.");
	AssertMessage(newSettings.width > 0 && newSettings.height > 0, "texture width and height must be > 0.");

	Draw_GetBackend()->textureInit(self, &newSettings);
}

void Texture_Free(Texture* self)
{
	Draw_Flush();
	Draw_GetBackend()->textureFree(self);
}

void Texture_SetData(Texture* self, int32 x, int32 y, int32 width, int32 height, void* data)
{
	Draw_Flush();
	Draw_GetBackend()->textureSetData(self, x, y, width, height, data);
}
