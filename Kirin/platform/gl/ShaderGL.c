#include "platform/gl/ShaderGL.h"

#include "common/File.h"
#include "platform/gl/DrawBackendGL.h"
#include "platform/gl/CommonGL.h"

#include "thirdparty/glad/glad.h"

static bool CheckShader(GLuint shader, char* prefix)
{
	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	CheckGLError();

	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		CheckGLError();

		// max length includes null terminator.
		if (maxLength > 1)
		{
			char* errorStr = (char*)MAlloc(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, errorStr);
			CheckGLError();

			char* messageType = success ? "warning" : "error";

			PrintF("%sshader compilation %s:\n%s", prefix, messageType, errorStr);

			MFree(errorStr);
		}
	}

	return success;
}

static bool CheckShaderProgram(GLuint program, char* prefix)
{
	GLint success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	CheckGLError();

	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
		CheckGLError();

		// max length includes null terminator.
		if (maxLength > 1)
		{
			char* errorStr = (char*)MAlloc(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, errorStr);
			CheckGLError();

			char* messageType = success ? "warning" : "error";

			PrintF("%sshader linking %s:\n%s", prefix, messageType, errorStr);

			MFree(errorStr);
		}
	}

	return success;
}

bool ShaderGL_Load(const char* vertPath, const char* fragPath, const char* vertPrefix, const char* fragPrefix, char** outVertStr, char** outFragStr)
{
	*outVertStr = null;
	*outFragStr = null;

	char* vertStr = File_ReadCStringFile(vertPath, null);
	if (!vertStr)
	{
		return false;
	}
	if (vertPrefix)
	{
		char* oldStr = vertStr;
		vertStr = SPrintFAlloc("%s\n%s\n%s", vertPrefix, "#line 1", vertStr);
		MFree(oldStr);
	}

	char* fragStr = File_ReadCStringFile(fragPath, null);
	if (!fragStr)
	{
		MFree(vertStr);
		return false;
	}
	if (fragPrefix)
	{
		char* oldStr = fragStr;
		fragStr = SPrintFAlloc("%s\n%s\n%s", fragPrefix, "#line 1", fragStr);
		MFree(oldStr);
	}

	*outVertStr = vertStr;
	*outFragStr = fragStr;

	return true;
}

bool ShaderGL_Compile(const char* vertStr, const char* fragStr, const char* vertName, const char* fragName, Shader* shader)
{
	// Compile vertex shader

	PrintF("    compiling vertex shader \"%s\"\n", vertName);
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertStr, NULL);
	glCompileShader(vertShader);
	CheckGLError();

	if (!CheckShader(vertShader, "        "))
	{
		glDeleteShader(vertShader);
		CheckGLError();
		return false;
	}

	// Compile fragment shader

	PrintF("    compiling fragment shader \"%s\"\n", fragName);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragStr, NULL);
	glCompileShader(fragShader);
	CheckGLError();

	if (!CheckShader(fragShader, "        "))
	{
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
		CheckGLError();
		return false;
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);
	CheckGLError();
	if (!CheckShaderProgram(program, "        "))
	{
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
		glDeleteProgram(program);
		CheckGLError();
		return false;
	}

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	CheckGLError();

	shader->program = program;

	{
		PrintF("    attributes:\n");

		int32 attributeCount;
		glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &attributeCount);
		CheckGLError();

		for (int32 i = 0; i < attributeCount; i++)
		{
			if (shader->attributeCount >= Shader_MaxAttributes)
			{
				Warning("too many shader attributes.");
				break;
			}

			ShaderAttribute attribute = { 0 };

			int32 nameLength;
			glGetActiveAttrib(program, i, ShaderAttribute_MaxName, &nameLength, &attribute.length, &attribute.type, attribute.name);
			CheckGLError();

			attribute.location = glGetAttribLocation(program, attribute.name);
			CheckGLError();

			PrintF("        layout(location = %d) attribute (%d) %s [%d];\n", attribute.location, attribute.type, attribute.name, attribute.length);

			shader->attributes[i] = attribute;
			shader->attributeCount++;
		}

		PrintF("    uniforms:\n");

		int32 uniformCount;
		glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniformCount);
		CheckGLError();

		for (int32 i = 0; i < uniformCount; i++)
		{
			if (shader->uniformCount >= Shader_MaxUniforms)
			{
				Warning("too many shader uniforms.");
				break;
			}

			ShaderUniform uniform = { 0 };

			int32 nameLength;
			glGetActiveUniform(program, i, ShaderUniform_MaxName, &nameLength, &uniform.length, &uniform.type, uniform.name);
			CheckGLError();

			uniform.location = glGetUniformLocation(program, uniform.name);
			CheckGLError();

			// ignore uniforms in uniform blocks.
			if (uniform.location < 0)
			{
				continue;
			}

			PrintF("        layout(location = %d) uniform (%d) %s [%d];\n", uniform.location, uniform.type, uniform.name, uniform.length);

			shader->uniforms[i] = uniform;
			shader->uniformCount++;
		}

		PrintF("    uniform buffers:\n");

		int32 uniformBufferCount;
		glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, &uniformBufferCount);
		CheckGLError();

		for (int32 i = 0; i < uniformBufferCount; i++)
		{
			if (shader->constantBufferCount >= Shader_MaxConstantBuffers)
			{
				Warning("too many shader uniform buffers.");
				break;
			}

			ShaderConstantBuffer constantBuffer = { 0 };

			int32 nameLength;
			glGetActiveUniformBlockName(program, i, ShaderConstantBuffer_MaxName, &nameLength, constantBuffer.name);
			CheckGLError();

			// should be equal to i?
			int32 index = glGetUniformBlockIndex(program, constantBuffer.name);
			CheckGLError();

			glGetActiveUniformBlockiv(program, index, GL_UNIFORM_BLOCK_BINDING, &constantBuffer.bindingPoint);
			CheckGLError();

			PrintF("        layout(binding = %d) uniform buffer %s;\n", constantBuffer.bindingPoint, constantBuffer.name);

			shader->constantBuffers[i] = constantBuffer;
			shader->constantBufferCount++;
		}
	}

	return true;
}

void ShaderGL_Free(Shader* self)
{
	if (self->program)
	{
		glDeleteProgram(self->program);
		CheckGLError();
	}
	self->program = 0;
}
