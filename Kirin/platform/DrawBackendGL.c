#include "platform/DrawBackendGL.h"

#include "common/Space.h"
#include "common/CString.h"
#include "platform/Draw.h"
#include "platform/DrawBackendShaderGL.h"

#include "thirdparty/glad/glad.h"

static uint32 viewDataUBO;
typedef struct ViewData
{
	Matrix4 viewMatrix;
	Matrix4 projectionMatrix;
} ViewData;

static bool viewDataIsDirty;
static ViewData viewData;

static void Init()
{
	viewDataIsDirty = true;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
	glDepthMask(GL_FALSE);

	glGenBuffers(1, &viewDataUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, viewDataUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(ViewData), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// bind viewDataUBO to UBO binding point 0.
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, viewDataUBO);
	CheckGLError();
}

static void Free()
{
	if (viewDataUBO)
	{
		glDeleteBuffers(1, &viewDataUBO);
		viewDataUBO = 0;
	}

	CheckGLError();
}

static bool LoadShader(const char* path, Shader* shader)
{
	char vertPath[MaxPathLength];
	char fragPath[MaxPathLength];

	StrCpy(vertPath, path, MaxPathLength);
	StrCat(vertPath, ".vert.glsl", MaxPathLength);

	StrCpy(fragPath, path, MaxPathLength);
	StrCat(fragPath, ".frag.glsl", MaxPathLength);

	// TODO: get actual glsl version.
	char* prefix = "#version 460";

	char* vertSource;
	char* fragSource;
	if (DrawBackendShaderGL_Load(vertPath, fragPath, prefix, prefix, &vertSource, &fragSource))
	{
		if (DrawBackendShaderGL_Compile(vertSource, fragSource, vertPath, fragPath, shader))
		{
			return true;
		}
	}

	return false;
}

static void SetShader(DrawState* drawState)
{
	if (!drawState->shader)
	{
		glUseProgram(0);
	}
	else
	{
		glUseProgram(drawState->shader->program);
	}
	CheckGLError();
}

static void InitVertexBuffer(VertexBuffer* vertexBuffer, VertexBufferUsage usage)
{
	static int32 usageToGLUsage[] = {
		[VertexBufferUsage_Static] = GL_STATIC_DRAW,
		[VertexBufferUsage_Dynamic] = GL_DYNAMIC_DRAW,
	};

	glGenBuffers(1, &vertexBuffer->internalHandle);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->internalHandle);
	glBufferData(GL_ARRAY_BUFFER, vertexBuffer->sizeInBytes, null, usageToGLUsage[usage]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CheckGLError();
}

static void UpdateVertexBufferData(VertexBuffer* vertexBuffer, int32 offset, int32 size, void* data)
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->internalHandle);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CheckGLError();
}

static void FreeVertexBuffer(VertexBuffer* vertexBuffer)
{
	if (vertexBuffer->internalHandle)
	{
		glDeleteBuffers(1, &vertexBuffer->internalHandle);
		vertexBuffer->internalHandle = 0;
	}
}

static void InitMesh(Mesh* mesh)
{
	glGenVertexArrays(1, &mesh->internalHandle);
	CheckGLError();
}

static void ApplyMeshStructure(Mesh* mesh)
{
	static int32 typeToGLType[] = {
		[VertexFormatType_Float] = GL_FLOAT,
		[VertexFormatType_Byte] = GL_UNSIGNED_BYTE,
	};
	static_assert(VertexFormatType_Count == 3, "enum has changed.");

#if CONFIG_DEBUG
	if (mesh->internalHandle == 0)
	{
		Error("mesh internalHandle is 0.");
	}
#endif

	glBindVertexArray(mesh->internalHandle);

	int32 lastBoundBuffer = 0;
	for (int32 i = 0; i < mesh->vertexFormatCount; i++)
	{
		VertexFormatItem* item = &mesh->vertexFormat[i];
#if CONFIG_DEBUG
		if (item->bufferIndex < 0 || item->bufferIndex >= mesh->vertexBufferCount)
		{
			ErrorF("VertexFormatItem bufferIndex is out of range. bufferIndex: %d vertexBufferCount: %d", item->bufferIndex, mesh->vertexBufferCount);
		}
#endif
		VertexBuffer* vertexBuffer = &mesh->vertexBuffers[item->bufferIndex];

#if CONFIG_DEBUG
		if (vertexBuffer->internalHandle == 0)
		{
			Error("vertexBuffer internalHandle is 0.");
		}
#endif
		// bind the the formats associated vertex buffer.
		if (lastBoundBuffer != vertexBuffer->internalHandle)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->internalHandle);
			CheckGLError();
		}

		// configure vertex buffer with vertex format.
		glVertexAttribPointer(i, item->componentCount, typeToGLType[item->type], GL_FALSE, item->stride, (void*)(size_t)item->offset);
		CheckGLError();

		// enable vertex attribute layout location.
		glEnableVertexAttribArray(i);
		CheckGLError();
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

static void FreeMesh(Mesh* mesh)
{
	if (mesh->internalHandle)
	{
		glDeleteVertexArrays(1, &mesh->internalHandle);
		mesh->internalHandle = 0;
	}
}

static void SetGeoType(DrawState* drawState)
{

}

static void SetPolygonFillMode(DrawState* drawState)
{
	switch (drawState->polygonFillMode)
	{
	case PolygonFillMode_Fill:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case PolygonFillMode_Line:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case PolygonFillMode_Point:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	default:
		break;
	}
}

static void SetBlendMode(DrawState* drawState)
{
	BlendMode mode = drawState->blendMode;

	if (mode != BlendMode_None)
	{
		glEnable(GL_BLEND);
		switch (mode)
		{
		case BlendMode_Alpha:
			glBlendEquation(GL_FUNC_ADD);
			// does accumulate alpha correctly.
			// for straight alpha:
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			// for premultiplied alpha:
			//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case BlendMode_Add:
			glBlendEquation(GL_FUNC_ADD);
			// does not accumulate alpha correctly.
			// does blend with alpha.
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
		case BlendMode_Subtract:
			glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
			// does not accumulate alpha correctly.
			// does blend with alpha.
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
		case BlendMode_Multiply:
			// does not accumulate alpha correctly.
			// does not blend with alpha.
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_DST_COLOR, GL_ZERO);
			break;
		default:
			Error("unexpected blend mode");
		}
	}
	else
	{
		glDisable(GL_BLEND);
	}
	CheckGLError();
}

static void SetCullMode(DrawState* drawState)
{
	switch (drawState->cullMode)
	{
	case CullMode_None:
		glDisable(GL_CULL_FACE);
		break;
	case CullMode_Back:
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		break;
	case CullMode_Front:
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		break;
	default:
		break;
	}
}

static void SetDepthTestMode(DrawState* drawState)
{
	if (drawState->depthTestMode == DepthTestMode_None)
	{
		glDisable(GL_DEPTH_TEST);
	}
	else
	{
		glEnable(GL_DEPTH_TEST);
		switch (drawState->depthTestMode)
		{
		case DepthTestMode_Always:
			glDepthFunc(GL_ALWAYS);
			break;
		case DepthTestMode_Never:
			glDepthFunc(GL_NEVER);
			break;
		case DepthTestMode_Equal:
			glDepthFunc(GL_EQUAL);
			break;
		case DepthTestMode_NotEqual:
			glDepthFunc(GL_NOTEQUAL);
			break;
		case DepthTestMode_Less:
			glDepthFunc(GL_LESS);
			break;
		case DepthTestMode_LessEqual:
			glDepthFunc(GL_LEQUAL);
			break;
		case DepthTestMode_Greater:
			glDepthFunc(GL_GREATER);
			break;
		case DepthTestMode_GreaterEqual:
			glDepthFunc(GL_GEQUAL);
			break;
		default:
			break;
		}
	}
	CheckGLError();
}

static void SetDepthWrite(DrawState* drawState)
{
	glDepthMask(drawState->depthWrite);
	CheckGLError();
}

static void SetViewMatrix(const Matrix4* matrix)
{
	MemCpy(&viewData.viewMatrix, matrix, sizeof(Matrix4));
	viewDataIsDirty = true;
}

static void SetProjectionMatrix(const Matrix4* matrix)
{
	MemCpy(&viewData.projectionMatrix, matrix, sizeof(Matrix4));
	viewDataIsDirty = true;
}

static void UploadViewDataIfDirty()
{
	// shader should aready be set.

	if (viewDataIsDirty)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, viewDataUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ViewData), &viewData);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		CheckGLError();

		viewDataIsDirty = false;
	}
}

static void SetViewport(int32 x, int32 y, int32 width, int32 height)
{
	glViewport(x, y, width, height);
	CheckGLError();
}

static void ClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
	CheckGLError();
}

static void ClearDepth(float value)
{
	glClearDepthf(value);
	glClear(GL_DEPTH_BUFFER_BIT);
	CheckGLError();
}

static void ClearStencil(int32 value)
{
	glClearStencil(value);
	glClear(GL_STENCIL_BUFFER_BIT);
	CheckGLError();
}

static void DrawMesh(Mesh* mesh, int32 vertexOffset, int32 vertexCount)
{
	UploadViewDataIfDirty();

	glBindVertexArray(mesh->internalHandle);
	CheckGLError();

	if (false /*programmable blending*/)
	{
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
	}

	glDrawArrays(GL_TRIANGLES, vertexOffset, vertexCount);
	CheckGLError();
}

DrawBackend drawBackendGL = {
	.init = Init,
	.free = Free,
	.loadShader = LoadShader,
	.freeShader = DrawBackendShaderGL_Free,
	.setShader = SetShader,
	.initVertexBuffer = InitVertexBuffer,
	.updateVertexBufferData = UpdateVertexBufferData,
	.freeVertexBuffer = FreeVertexBuffer,
	.initMesh = InitMesh,
	.applyMeshStructure = ApplyMeshStructure,
	.freeMesh = FreeMesh,
	.setGeoType = SetGeoType,
	.setPolygonFillMode = SetPolygonFillMode,
	.setBlendMode = SetBlendMode,
	.setCullMode = SetCullMode,
	.setDepthTestMode = SetDepthTestMode,
	.setDepthWrite = SetDepthWrite,
	.setViewMatrix = SetViewMatrix,
	.setProjectionMatrix = SetProjectionMatrix,
	.setViewport = SetViewport,
	.clearColor = ClearColor,
	.clearDepth = ClearDepth,
	.clearStencil = ClearStencil,
	.drawMesh = DrawMesh,
};

DrawBackend* DrawBackendGL_Get()
{
	return &drawBackendGL;
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
