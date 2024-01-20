#pragma once

#include "common/Standard.h"
#include "common/Space.h"
#include "draw/Mesh.h"
#include "draw/Shader.h"
#include "draw/ConstantBuffer.h"
#include "draw/Texture.h"

typedef enum DrawGeoType {
	DrawGeoType_None,
	DrawGeoType_Triangle,
	DrawGeoType_Line,
	DrawGeoType_Count,
} DrawGeoType;

static const char* DrawGeoType_ToString(DrawGeoType value)
{
	switch (value) {
	case DrawGeoType_None: return "DrawGeoType_None"; break;
	case DrawGeoType_Triangle: return "DrawGeoType_Triangle"; break;
	case DrawGeoType_Line: return "DrawGeoType_Line"; break;
	default: return "INVALID"; break;
	}
	static_assert(DrawGeoType_Count == 3, "enum has changed.");
}

typedef enum PolygonFillMode {
	PolygonFillMode_Fill,
	PolygonFillMode_Line,
	PolygonFillMode_Point,
	PolygonFillMode_Count,
} PolygonFillMode;

static const char* PolygonFillMode_ToString(PolygonFillMode value)
{
	switch (value) {
	case PolygonFillMode_Fill: return "PolygonFillMode_Fill"; break;
	case PolygonFillMode_Line: return "PolygonFillMode_Line"; break;
	case PolygonFillMode_Point: return "PolygonFillMode_Point"; break;
	default: return "INVALID"; break;
	}
	static_assert(PolygonFillMode_Count == 3, "enum has changed.");
}

typedef enum BlendMode {
	BlendMode_None,
	BlendMode_Alpha,
	BlendMode_Add,
	BlendMode_Subtract,
	BlendMode_Multiply,
	BlendMode_Count,
} BlendMode;

static const char* BlendMode_ToString(BlendMode value)
{
	switch (value) {
	case BlendMode_None: return "BlendMode_None"; break;
	case BlendMode_Alpha: return "BlendMode_Alpha"; break;
	case BlendMode_Add: return "BlendMode_Add"; break;
	case BlendMode_Subtract: return "BlendMode_Subtract"; break;
	case BlendMode_Multiply: return "BlendMode_Multiply"; break;
	default: return "INVALID"; break;
	}
	static_assert(BlendMode_Count == 5, "enum has changed.");
}

typedef enum CullMode
{
	CullMode_None,
	CullMode_Back,
	CullMode_Front,
	CullMode_Count,
} CullMode;

static const char* CullMode_ToString(CullMode value)
{
	switch (value) {
	case CullMode_None: return "CullMode_None"; break;
	case CullMode_Back: return "CullMode_Back"; break;
	case CullMode_Front: return "CullMode_Front"; break;
	default: return "INVALID"; break;
	}
	static_assert(CullMode_Count == 3, "enum has changed.");
}

typedef enum DepthTestMode
{
	DepthTestMode_None,
	DepthTestMode_Always,
	DepthTestMode_Never,
	DepthTestMode_Equal,
	DepthTestMode_NotEqual,
	DepthTestMode_Less,
	DepthTestMode_LessEqual,
	DepthTestMode_Greater,
	DepthTestMode_GreaterEqual,
	DepthTestMode_Count,
} DepthTestMode;

static const char* DepthTestMode_ToString(DepthTestMode value)
{
	switch (value) {
	case DepthTestMode_None: return "DepthTestMode_None"; break;
	case DepthTestMode_Always: return "DepthTestMode_Always"; break;
	case DepthTestMode_Never: return "DepthTestMode_Never"; break;
	case DepthTestMode_Equal: return "DepthTestMode_Equal"; break;
	case DepthTestMode_NotEqual: return "DepthTestMode_NotEqual"; break;
	case DepthTestMode_Less: return "DepthTestMode_Less"; break;
	case DepthTestMode_LessEqual: return "DepthTestMode_LessEqual"; break;
	case DepthTestMode_Greater: return "DepthTestMode_Greater"; break;
	case DepthTestMode_GreaterEqual: return "DepthTestMode_GreaterEqual"; break;
	default: return "INVALID"; break;
	}
	static_assert(DepthTestMode_Count == 9, "enum has changed.");
}

typedef enum StencilOpMode
{
	StencilOpMode_Keep,
	StencilOpMode_Zero,
	StencilOpMode_Replace,
	StencilOpMode_Inc,
	StencilOpMode_IncWrap,
	StencilOpMode_Dec,
	StencilOpMode_DecWrap,
	StencilOpMode_Invert,
	StencilOpMode_Count,
} StencilOpMode;

static const char* StencilOpMode_ToString(StencilOpMode value)
{
	switch (value) {
	case StencilOpMode_Keep: return "StencilOpMode_Keep"; break;
	case StencilOpMode_Zero: return "StencilOpMode_Zero"; break;
	case StencilOpMode_Replace: return "StencilOpMode_Replace"; break;
	case StencilOpMode_Inc: return "StencilOpMode_Inc"; break;
	case StencilOpMode_IncWrap: return "StencilOpMode_IncWrap"; break;
	case StencilOpMode_Dec: return "StencilOpMode_Dec"; break;
	case StencilOpMode_DecWrap: return "StencilOpMode_DecWrap"; break;
	case StencilOpMode_Invert: return "StencilOpMode_Invert"; break;
	default: return "INVALID"; break;
	}
	static_assert(StencilOpMode_Count == 8, "enum has changed.");
}

#pragma push_macro("DrawStatePackEnum")
#undef DrawStatePackEnum
#define DrawStatePackEnum(type, bits) : bits; static_assert(type##_Count <= 1 << bits, "bitfield cannot fit enum.")
typedef struct DrawState
{
	PolygonFillMode polygonFillMode DrawStatePackEnum(PolygonFillMode, 2);
	BlendMode blendMode DrawStatePackEnum(BlendMode, 3);
	CullMode cullMode DrawStatePackEnum(CullMode, 2);
	DepthTestMode depthTestMode DrawStatePackEnum(DepthTestMode, 4);
	bool depthWrite : 1;
	DepthTestMode stencilFunc DrawStatePackEnum(DepthTestMode, 4);
	StencilOpMode stencilOpFailStencil DrawStatePackEnum(StencilOpMode, 3);
	StencilOpMode stencilOpFailDepth DrawStatePackEnum(StencilOpMode, 3);
	StencilOpMode stencilOpPass DrawStatePackEnum(StencilOpMode, 3);
	uint8 stencilMask;
	uint8 stencilFuncRef;
	uint8 stencilFuncMask;
} DrawState;
#pragma pop_macro("DrawStatePackEnum")

typedef struct DrawBackend
{
	void (*init)();
	void (*free)();
	void (*vertexBufferInit)(VertexBuffer* vertexBuffer, VertexBufferUsage usage);
	void (*vertexBufferUpdateData)(VertexBuffer* vertexBuffer, int32 offset, int32 size, void* data);
	void (*vertexBufferFree)(VertexBuffer* vertexBuffer);
	void (*drawStateUpdate)(DrawState* previousState, DrawState* state, bool forceUpdate);
	void (*setViewport)(int32 x, int32 y, int32 width, int32 height);
	void (*clearColor)(float r, float g, float b, float a);
	void (*clearDepth)(float value);
	void (*clearStencil)(int32 value);
	bool (*shaderLoad)(const char* path, Shader* shader);
	void (*shaderFree)(Shader* shader);
	void (*shaderSet)(Shader* shader);
	void (*shaderSetUniformInt)(Shader* self, ShaderUniform* uniform, int32 arrayIndex, int32 value);
	void (*shaderSetUniformFloat)(Shader* self, ShaderUniform* uniform, int32 arrayIndex, float value);
	void (*shaderSetUniformTexture)(Shader* self, ShaderUniform* uniform, int32 arrayIndex, Texture* value);
	void (*constantBufferInit)(ConstantBuffer* self, int64 size);
	void (*constantBufferAttachToShader)(ConstantBuffer* self, Shader* shader, ShaderConstantBuffer* constantBuffer);
	void (*constantBufferSetData)(ConstantBuffer* self, int64 offset, int64 length, void* data);
	void (*constantBufferFree)(ConstantBuffer* self);
	void (*textureInit)(Texture* self, TextureInitSettings* initSettings);
	void (*textureFree)(Texture* self);
	void (*textureSetData)(Texture* self, int32 x, int32 y, int32 width, int32 height, void* data);
	void (*generateMipmaps)(Texture* self);
	void (*meshInit)(Mesh* mesh);
	void (*meshApplyStructure)(Mesh* mesh);
	void (*meshDraw)(Mesh* mesh, int32 vertexOffset, int32 vertexCount);
	void (*meshFree)(Mesh* mesh);
} DrawBackend;
