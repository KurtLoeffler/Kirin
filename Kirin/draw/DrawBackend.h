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

typedef struct DrawState
{
#if 0
	uint64 bits;
#endif
	bool dirty;
	struct Shader* shader;
	DrawGeoType geoType;
	PolygonFillMode polygonFillMode;
	BlendMode blendMode;
	CullMode cullMode;
	DepthTestMode depthTestMode;
	bool depthWrite;
} DrawState;

typedef struct DrawBackend
{
	void (*init)();
	void (*free)();
	void (*initVertexBuffer)(VertexBuffer* vertexBuffer, VertexBufferUsage usage);
	void (*updateVertexBufferData)(VertexBuffer* vertexBuffer, int32 offset, int32 size, void* data);
	void (*freeVertexBuffer)(VertexBuffer* vertexBuffer);
	void (*initMesh)(Mesh* mesh);
	void (*applyMeshStructure)(Mesh* mesh);
	void (*freeMesh)(Mesh* mesh);
	void (*setGeoType)(DrawState* drawState);
	void (*setPolygonFillMode)(DrawState* drawState);
	void (*setBlendMode)(DrawState* drawState);
	void (*setCullMode)(DrawState* drawState);
	void (*setDepthTestMode)(DrawState* drawState);
	void (*setDepthWrite)(DrawState* drawState);
	void (*setViewport)(int32 x, int32 y, int32 width, int32 height);
	void (*clearColor)(float r, float g, float b, float a);
	void (*clearDepth)(float value);
	void (*clearStencil)(int32 value);
	void (*drawMesh)(Mesh* mesh, int32 vertexOffset, int32 vertexCount);
	bool (*shaderLoad)(const char* path, Shader* shader);
	void (*shaderFree)(Shader* shader);
	void (*shaderSet)(DrawState* drawState);
	void (*constantBufferInit)(ConstantBuffer* self, int64 size);
	void (*constantBufferAttachToShader)(ConstantBuffer* self, Shader* shader, ShaderConstantBuffer* constantBuffer);
	void (*constantBufferSetData)(ConstantBuffer* self, int64 offset, int64 length, void* data);
	void (*constantBufferFree)(ConstantBuffer* self);
	void (*textureInit)(Texture* self, TextureInitSettings* initSettings);
	void (*textureFree)(Texture* self);
	void (*textureSetData)(Texture* self, int32 x, int32 y, int32 width, int32 height, void* data);
} DrawBackend;
