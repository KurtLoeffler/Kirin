#include "draw/gl/TextureGL.h"

#include "common/Math.h"
#include "draw/gl/CommonGL.h"

#include "thirdparty/glad/glad.h"

static GLint TextureFormatToGLInternalFormat(TextureFormat format)
{
	switch (format)
	{
	case TextureFormat_RGB8:
		return GL_RGB8;
	case TextureFormat_RGBA8:
		return GL_RGBA8;
	default:
		break;
	}

	Error("bad format.");
	return 0;
}

static GLint TextureFormatToGLFormat(TextureFormat format)
{
	switch (format)
	{
	case TextureFormat_RGB8:
		return GL_RGB;
	case TextureFormat_RGBA8:
		return GL_RGBA;
	default:
		break;
	}

	Error("bad format.");
	return 0;
}

static GLint TextureFormatToGLPixelType(TextureFormat format)
{
	switch (format)
	{
	case TextureFormat_RGB8:
		return GL_UNSIGNED_BYTE;
	case TextureFormat_RGBA8:
		return GL_UNSIGNED_BYTE;
	default:
		break;
	}

	Error("bad format.");
	return 0;
}

static void UpdateFiltering(Texture* self)
{
	uint32 minFilter;
	uint32 magFilter;
	switch (self->filterMode)
	{
	case TextureFilterMode_Nearest:
		minFilter = GL_NEAREST;
		magFilter = GL_NEAREST;
		break;
	case TextureFilterMode_Linear:
		minFilter = GL_LINEAR;
		magFilter = GL_LINEAR;
		break;
	default:
		Error("invalid texture wrap mode.");
		break;
	}

	if (self->hasMipmaps)
	{
		if (self->mipFilterMode > TextureFilterMode_Nearest)
		{
			minFilter = minFilter == GL_NEAREST ? GL_NEAREST_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_LINEAR;
		}
		else
		{
			minFilter = minFilter == GL_NEAREST ? GL_NEAREST_MIPMAP_NEAREST : GL_LINEAR_MIPMAP_NEAREST;
		}
	}

	glBindTexture(GL_TEXTURE_2D, TextureGLHandle(self));
	CheckGLError();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	CheckGLError();

	if (self->hasMipmaps)
	{
		static float maxAnisotropy = -1.0f;
		if (maxAnisotropy < 0)
		{
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
			Assert(maxAnisotropy >= 0);
		}

		float anisotropy = maxAnisotropy;
		if (self->anisotropy >= 0)
		{
			anisotropy = Min((float)MaxI(self->anisotropy, 1), maxAnisotropy);
		}
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
	}
	else
	{
		// disable anisotropy.
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	CheckGLError();
}

void TextureGL_Init(Texture* self, TextureInitSettings* initSettings)
{
	self->type = initSettings->type;
	self->format = initSettings->format;
	self->wrapMode = initSettings->wrapMode;
	self->filterMode = initSettings->filterMode;
	self->mipFilterMode = initSettings->mipFilterMode;
	self->anisotropy = initSettings->anisotropy;
	self->width = initSettings->width;
	self->height = initSettings->height;
	self->depth = initSettings->depth;

	glGenTextures(1, &TextureGLHandle(self));
	if (self->type == TextureType_2D)
	{
		glBindTexture(GL_TEXTURE_2D, TextureGLHandle(self));
		glTexImage2D(GL_TEXTURE_2D, 0, TextureFormatToGLInternalFormat(self->format), self->width, self->height, 0, TextureFormatToGLFormat(self->format), TextureFormatToGLPixelType(self->format), NULL);
	}
	else
	{
		Error("unimplemented texture type.");
	}
	CheckGLError();

	{
		uint32 wrapParam;
		switch (self->wrapMode)
		{
		case TextureWrapMode_Repeat:
			wrapParam = GL_REPEAT;
			break;
		case TextureWrapMode_Clamp:
			wrapParam = GL_CLAMP_TO_EDGE;
			break;
		default:
			Error("invalid texture wrap mode.");
			break;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapParam);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapParam);
		CheckGLError();
	}

	const bool useBindlessTexture = false;
	if (useBindlessTexture)
	{
		TextureGLBindlessHandle(self) = glGetTextureHandleARB(TextureGLHandle(self));
		glMakeTextureHandleResidentARB(TextureGLBindlessHandle(self));
		CheckGLError();
	}

	UpdateFiltering(self);

	glBindTexture(GL_TEXTURE_2D, 0);
	CheckGLError();
}

void TextureGL_Free(Texture* self)
{
	if (TextureGLHandle(self))
	{
		const bool useBindlessTexture = false;
		if (useBindlessTexture)
		{
			glMakeTextureHandleNonResidentARB(TextureGLBindlessHandle(self));
		}
		glDeleteTextures(1, &TextureGLHandle(self));
		CheckGLError();
		MemSet(self->internalHandle, 0, sizeof(self->internalHandle));
	}
}

void TextureGL_SetData(Texture* self, int32 x, int32 y, int32 width, int32 height, void* data)
{
	if (!TextureGLHandle(self))
	{
		Error("texture is not created.");
	}

	x = x >= 0 ? x : 0;
	y = y >= 0 ? y : 0;
	width = width >= 0 ? width : self->width;
	height = height >= 0 ? height : self->height;

	glBindTexture(GL_TEXTURE_2D, TextureGLHandle(self));
	CheckGLError();
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, TextureFormatToGLFormat(self->format), TextureFormatToGLPixelType(self->format), data);
	CheckGLError();

	glBindTexture(GL_TEXTURE_2D, 0);
	CheckGLError();
}

void TextureGL_GenerateMipmaps(Texture* self)
{
	glBindTexture(GL_TEXTURE_2D, TextureGLHandle(self));
	CheckGLError();

	bool didHaveMipmaps = self->hasMipmaps;
	self->hasMipmaps = true;

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	CheckGLError();

	if (self->hasMipmaps != didHaveMipmaps)
	{
		UpdateFiltering(self);
	}
}
