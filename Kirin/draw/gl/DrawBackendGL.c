#include "draw/gl/DrawBackendGL.h"

#include "common/Space.h"
#include "common/CString.h"
#include "draw/Draw.h"
#include "draw/gl/CommonGL.h"
#include "draw/gl/ShaderGL.h"
#include "draw/gl/MeshGL.h"
#include "draw/gl/ConstantBufferGL.h"

#include "thirdparty/glad/glad.h"

static void Init()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
	glDepthMask(GL_FALSE);
}

static void Free()
{

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
	if (ShaderGL_Load(vertPath, fragPath, prefix, prefix, &vertSource, &fragSource))
	{
		if (ShaderGL_Compile(vertSource, fragSource, vertPath, fragPath, shader))
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
	.freeShader = ShaderGL_Free,
	.setShader = SetShader,
	.initVertexBuffer = InitVertexBuffer,
	.updateVertexBufferData = UpdateVertexBufferData,
	.freeVertexBuffer = FreeVertexBuffer,
	.initMesh = MeshGL_Init,
	.applyMeshStructure = MeshGL_ApplyStructure,
	.freeMesh = MeshGL_Free,
	.setGeoType = SetGeoType,
	.setPolygonFillMode = SetPolygonFillMode,
	.setBlendMode = SetBlendMode,
	.setCullMode = SetCullMode,
	.setDepthTestMode = SetDepthTestMode,
	.setDepthWrite = SetDepthWrite,
	.setViewport = SetViewport,
	.clearColor = ClearColor,
	.clearDepth = ClearDepth,
	.clearStencil = ClearStencil,
	.drawMesh = DrawMesh,
	.constantBufferInit = ConstantBufferGL_Init,
	.constantBufferAttachToShader = ConstantBufferGL_AttachToShader,
	.constantBufferSetData = ConstantBufferGL_SetData,
	.constantBufferFree = ConstantBufferGL_Free,
};

DrawBackend* DrawBackendGL_Get()
{
	return &drawBackendGL;
}
