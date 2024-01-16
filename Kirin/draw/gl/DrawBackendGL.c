#include "draw/gl/DrawBackendGL.h"

#include "common/Space.h"
#include "common/CString.h"
#include "draw/Draw.h"
#include "draw/gl/CommonGL.h"
#include "draw/gl/ShaderGL.h"
#include "draw/gl/MeshGL.h"
#include "draw/gl/ConstantBufferGL.h"
#include "draw/gl/TextureGL.h"
#include "platform/gl/WindowBackendGL.h"

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

	SPrintF(vertPath, MaxPathLength, "%s.vert.glsl", path);
	SPrintF(fragPath, MaxPathLength, "%s.frag.glsl", path);

	char prefix[32];
	SPrintF(prefix, 32, "#version %s", WindowBackendGL_GetGLSLVersionString());

	char* vertSource;
	char* fragSource;
	if (ShaderGL_Load(vertPath, fragPath, prefix, prefix, &vertSource, &fragSource))
	{
		bool compileResult = ShaderGL_Compile(vertSource, fragSource, vertPath, fragPath, shader);
		MFree(vertSource);
		MFree(fragSource);
		if (compileResult)
		{
			return true;
		}
	}

	return false;
}

static void SetShader(Shader* shader)
{
	if (!shader)
	{
		glUseProgram(0);
	}
	else
	{
		glUseProgram(shader->program);
	}
	CheckGLError();
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
		}
	}
	CheckGLError();
}

static void SetDepthWrite(DrawState* drawState)
{
	glDepthMask(drawState->depthWrite);
	CheckGLError();
}



static int32 TranslateStencilOpMode(StencilOpMode mode)
{
	switch (mode)
	{
	case StencilOpMode_Keep:
		return GL_KEEP;
	case StencilOpMode_Zero:
		return GL_ZERO;
	case StencilOpMode_Replace:
		return GL_REPLACE;
	case StencilOpMode_Inc:
		return GL_INCR;
	case StencilOpMode_IncWrap:
		return GL_INCR_WRAP;
	case StencilOpMode_Dec:
		return GL_DECR;
	case StencilOpMode_DecWrap:
		return GL_DECR_WRAP;
	case StencilOpMode_Invert:
		return GL_INVERT;
	}
	return 0;
}

static void SetStencilWrite(DrawState* drawState)
{
	if (drawState->stencilFunc == DepthTestMode_None)
	{
		glDisable(GL_STENCIL_TEST);
	}
	else
	{
		glEnable(GL_STENCIL_TEST);
		switch (drawState->stencilFunc)
		{
		case DepthTestMode_Always:
			glStencilFunc(GL_ALWAYS, drawState->stencilFuncRef, drawState->stencilFuncMask);
			break;
		case DepthTestMode_Never:
			glStencilFunc(GL_NEVER, drawState->stencilFuncRef, drawState->stencilFuncMask);
			break;
		case DepthTestMode_Equal:
			glStencilFunc(GL_EQUAL, drawState->stencilFuncRef, drawState->stencilFuncMask);
			break;
		case DepthTestMode_NotEqual:
			glStencilFunc(GL_NOTEQUAL, drawState->stencilFuncRef, drawState->stencilFuncMask);
			break;
		case DepthTestMode_Less:
			glStencilFunc(GL_LESS, drawState->stencilFuncRef, drawState->stencilFuncMask);
			break;
		case DepthTestMode_LessEqual:
			glStencilFunc(GL_LEQUAL, drawState->stencilFuncRef, drawState->stencilFuncMask);
			break;
		case DepthTestMode_Greater:
			glStencilFunc(GL_GREATER, drawState->stencilFuncRef, drawState->stencilFuncMask);
			break;
		case DepthTestMode_GreaterEqual:
			glStencilFunc(GL_GEQUAL, drawState->stencilFuncRef, drawState->stencilFuncMask);
			break;
		}
	}
	CheckGLError();

	glStencilMask(drawState->stencilMask);
	CheckGLError();

	glStencilOp(
		TranslateStencilOpMode(drawState->stencilOpFailStencil),
		TranslateStencilOpMode(drawState->stencilOpFailDepth),
		TranslateStencilOpMode(drawState->stencilOpPass));
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

DrawBackend drawBackendGL = {
	.init = Init,
	.free = Free,
	.setGeoType = SetGeoType,
	.setPolygonFillMode = SetPolygonFillMode,
	.setBlendMode = SetBlendMode,
	.setCullMode = SetCullMode,
	.setDepthTestMode = SetDepthTestMode,
	.setDepthWrite = SetDepthWrite,
	.setStencilState = SetStencilWrite,
	.setViewport = SetViewport,
	.clearColor = ClearColor,
	.clearDepth = ClearDepth,
	.clearStencil = ClearStencil,
	.shaderLoad = LoadShader,
	.shaderFree = ShaderGL_Free,
	.shaderSet = SetShader,
	.shaderSetUniformInt = ShaderGL_SetUniformInt,
	.shaderSetUniformTexture = ShaderGL_SetUniformTexture,
	.constantBufferInit = ConstantBufferGL_Init,
	.constantBufferAttachToShader = ConstantBufferGL_AttachToShader,
	.constantBufferSetData = ConstantBufferGL_SetData,
	.constantBufferFree = ConstantBufferGL_Free,
	.textureInit = TextureGL_Init,
	.textureFree = TextureGL_Free,
	.textureSetData = TextureGL_SetData,
	.generateMipmaps = TextureGL_GenerateMipmaps,
	.vertexBufferInit = VertexBufferGL_Init,
	.vertexBufferUpdateData = VertexBufferGL_UpdateData,
	.vertexBufferFree = VertexBufferGL_Free,
	.meshInit = MeshGL_Init,
	.meshApplyStructure = MeshGL_ApplyStructure,
	.meshDraw = MeshGL_Draw,
	.meshFree = MeshGL_Free,
};

DrawBackend* DrawBackendGL_Get()
{
	return &drawBackendGL;
}
