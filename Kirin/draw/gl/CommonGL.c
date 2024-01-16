#include "draw/gl/CommonGL.h"

#include "common/CString.h"

#include "thirdparty/glad/glad.h"

bool IsGLExtensionSupported(const char* extension)
{
	GLint numExtensions;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
	for (GLint i = 0; i < numExtensions; i++)
	{
		const GLubyte* ext = glGetStringi(GL_EXTENSIONS, i);
		if (StrCmp((const char*)ext, extension, true) == 0)
		{
			return true;
		}
	}
	return false;
}

#if CONFIG_DEBUG
static const char* GetGLErrorString(uint32 err)
{
	switch (err)
	{
	case GL_NO_ERROR:
		return "GL_NO_ERROR";
	case GL_INVALID_ENUM:
		return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE:
		return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION:
		return "GL_INVALID_OPERATION";
	case GL_STACK_OVERFLOW:
		return "GL_STACK_OVERFLOW";
	case GL_STACK_UNDERFLOW:
		return "GL_STACK_UNDERFLOW";
	case GL_OUT_OF_MEMORY:
		return "GL_OUT_OF_MEMORY";
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return "GL_INVALID_FRAMEBUFFER_OPERATION";
	case GL_CONTEXT_LOST:
		return "GL_CONTEXT_LOST";
	default:
		return null;
	}
}

bool _CheckGLErrorPrintError(char* file, int line)
{
	GLenum error;
	bool hadAny = false;
	while ((error = glGetError()) != GL_NO_ERROR)
	{
		PrintF("gl error: %s\n", GetGLErrorString(error));
		hadAny = true;
	}
	if (hadAny)
	{
		PrintF("    found at %s:%d\n", file, line);
	}
	return hadAny;
}
#endif
