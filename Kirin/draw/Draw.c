#include "draw/Draw.h"

DrawStatCounters gDrawStatCounters;

DrawBackend* currentBackend;

DrawState currentDrawState;
DrawState lastDrawState;
static bool currentDrawStateDirty;
static bool forceFullStateDirty;

static int32 immediateVertexOffset;
static int32 immediateVertexStride;
#define Draw_ImmediateBatchSize 65536
static uint8 immediateVertexBuffer[Draw_ImmediateBatchSize] = { 0 };
static Mesh immediateMesh;

void Draw_Init(DrawBackend* backend)
{
	currentBackend = backend;

	currentDrawState = (DrawState){ 0 };
	lastDrawState = currentDrawState;
	currentDrawStateDirty = true;
	forceFullStateDirty = true;

	currentBackend->init();

	Mesh_Init(&immediateMesh);
	immediateMesh.vertexBufferCount = 1;
	VertexBuffer_Init(&immediateMesh.vertexBuffers[0], Draw_ImmediateBatchSize, VertexBufferUsage_Dynamic);

	Draw_Flush();
}

void Draw_Free()
{
	Mesh_Free(&immediateMesh);

	currentBackend->free();
	currentBackend = null;
}

bool Draw_LoadShader(const char* path, Shader* shader)
{
	*shader = (Shader){ 0 };

	PrintF("loading shader \"%s\"\n", path);
	if (!currentBackend->shaderLoad(path, shader))
	{
		Error("shader load failed.");
	}
	return true;
}

void Draw_FreeShader(Shader* shader)
{
	currentBackend->shaderFree(shader);
}

DrawBackend* Draw_GetBackend()
{
	return currentBackend;
}

DrawState* Draw_GetDrawState()
{
	return &currentDrawState;
}

bool Draw_IsDrawStateDirty()
{
	return currentDrawStateDirty;
}

void Draw_MarkDrawStateDirty()
{
	currentDrawStateDirty = true;
}

bool Draw_SetDrawState(const DrawState* drawState)
{
	if (MemCmp(drawState, &currentDrawState, sizeof(DrawState)) != 0)
	{
		currentDrawStateDirty = true;
		currentDrawState = *drawState;
	}

	return false;
}

void Draw_SetImmediateVertexFormat(VertexFormatItem* format, int32 count)
{
	if (count <= 0)
	{
		Error("count must be greater than 0.");
	}

	if (count > Mesh_MaxVertexFormatItems)
	{
		Error("count is too large.");
	}

	bool isDirty = count != immediateMesh.vertexFormatCount;
	if (!isDirty)
	{
		isDirty = MemCmp(immediateMesh.vertexFormat, format, count*sizeof(VertexFormatItem)) != 0;
	}

	if (isDirty)
	{
		Draw_Flush();

		immediateMesh.vertexFormatCount = count;
		MemCpy(immediateMesh.vertexFormat, format, count*sizeof(VertexFormatItem));

		immediateVertexStride = immediateMesh.vertexFormat[0].stride;
#if CONFIG_DEBUG
		for (int32 i = 0; i < count; i++)
		{
			VertexFormatItem* item = &immediateMesh.vertexFormat[i];
			if (item->bufferIndex != 0)
			{
				Error("all immediate vertex format buffer indices must be 0.")
			}
			if (immediateVertexStride != item->stride)
			{
				Error("all immediate vertex format strides must be the same.")
			}
		}
#endif
		Mesh_ApplyStructure(&immediateMesh);
	}
}

static void CommitDrawState()
{
	if (forceFullStateDirty || currentDrawStateDirty)
	{
		if (forceFullStateDirty || currentDrawState.shader != lastDrawState.shader)
		{
			currentBackend->shaderSet(&currentDrawState);
			lastDrawState.shader = currentDrawState.shader;
			gDrawStatCounters.shaderChanges++;
			gDrawStatCounters.granularStateChanges++;
		}

		if (forceFullStateDirty || currentDrawState.polygonFillMode != lastDrawState.polygonFillMode)
		{
			currentBackend->setPolygonFillMode(&currentDrawState);
			lastDrawState.polygonFillMode = currentDrawState.polygonFillMode;
			gDrawStatCounters.granularStateChanges++;
		}

		if (forceFullStateDirty || currentDrawState.blendMode != lastDrawState.blendMode)
		{
			currentBackend->setBlendMode(&currentDrawState);
			lastDrawState.blendMode = currentDrawState.blendMode;
			gDrawStatCounters.granularStateChanges++;
		}

		if (forceFullStateDirty || currentDrawState.cullMode != lastDrawState.cullMode)
		{
			currentBackend->setCullMode(&currentDrawState);
			lastDrawState.cullMode = currentDrawState.cullMode;
			gDrawStatCounters.granularStateChanges++;
		}

		if (forceFullStateDirty || currentDrawState.depthTestMode != lastDrawState.depthTestMode)
		{
			currentBackend->setDepthTestMode(&currentDrawState);
			lastDrawState.depthTestMode = currentDrawState.depthTestMode;
			gDrawStatCounters.granularStateChanges++;
		}

		if (forceFullStateDirty || currentDrawState.depthWrite != lastDrawState.depthWrite)
		{
			currentBackend->setDepthWrite(&currentDrawState);
			lastDrawState.depthWrite = currentDrawState.depthWrite;
			gDrawStatCounters.granularStateChanges++;
		}

		// PERF: is this slow?
		if (forceFullStateDirty ||
			currentDrawState.stencilFunc != lastDrawState.stencilFunc ||
			currentDrawState.stencilOpFailStencil != lastDrawState.stencilOpFailStencil ||
			currentDrawState.stencilOpFailDepth != lastDrawState.stencilOpFailDepth ||
			currentDrawState.stencilOpPass != lastDrawState.stencilOpPass ||
			currentDrawState.stencilMask != lastDrawState.stencilMask ||
			currentDrawState.stencilFuncRef != lastDrawState.stencilFuncRef ||
			currentDrawState.stencilFuncMask != lastDrawState.stencilFuncMask)
		{
			currentBackend->setStencilState(&currentDrawState);
			lastDrawState.stencilFunc = currentDrawState.stencilFunc;
			lastDrawState.stencilOpFailStencil = currentDrawState.stencilOpFailStencil;
			lastDrawState.stencilOpFailDepth = currentDrawState.stencilOpFailDepth;
			lastDrawState.stencilOpPass = currentDrawState.stencilOpPass;
			lastDrawState.stencilMask = currentDrawState.stencilMask;
			lastDrawState.stencilFuncRef = currentDrawState.stencilFuncRef;
			lastDrawState.stencilFuncMask = currentDrawState.stencilFuncMask;
			gDrawStatCounters.granularStateChanges++;
		}

		forceFullStateDirty = false;
		currentDrawStateDirty = false;

#if CONFIG_DEBUG
		if (MemCmp(&currentDrawState, &lastDrawState, sizeof(DrawState)) != 0)
		{
			Error("draw state data mismatch.");
		}
#endif
	}
}

void Draw_Flush()
{
	// draw immediate geometry before applying new state because it must be drawn with the state state from when the geometry was submitted.
	if (immediateVertexOffset > 0)
	{
		VertexBuffer_SetData(&immediateMesh.vertexBuffers[0], 0, immediateVertexOffset, immediateVertexBuffer);
		// not using Mesh_Draw here because that calls Draw_Flush.
		currentBackend->meshDraw(&immediateMesh, 0, immediateMesh.vertexCount);
		immediateMesh.vertexCount = 0;
		immediateVertexOffset = 0;
		gDrawStatCounters.immediateDraws++;
		gDrawStatCounters.meshDraws++;
	}

	CommitDrawState();
}

void Draw_SubmitImmediatePoly(const void* vertices, int32 vertexCount)
{
#if CONFIG_DEBUG
	if (!immediateVertexStride)
	{
		Error("immediateVertexStride is 0. this probably means the immediate vertex format has not been set.");
	}
#endif

	if (vertexCount < 3)
	{
		ErrorF("unsupported vertex count: %d", vertexCount);
	}
	int32 actualVertexCount = 3;

	if (vertexCount > 3)
	{
		actualVertexCount += (vertexCount-3)*3;
	}

	int32 actualVertexCountSize = actualVertexCount*immediateVertexStride;

	if (actualVertexCountSize > Draw_ImmediateBatchSize)
	{
#if CONFIG_DEBUG
		ErrorF("poly would require too many vertices: %d*%d (%d) > %d", actualVertexCount, immediateVertexStride, actualVertexCountSize, Draw_ImmediateBatchSize);
#else
		return;
#endif
	}

	// if the state is dirty the immediate geometry must be flushed now.
	if (currentDrawStateDirty || immediateVertexOffset+actualVertexCountSize >= Draw_ImmediateBatchSize)
	{
		Draw_Flush();
	}

	MemCpy(&immediateVertexBuffer[immediateVertexOffset], vertices, immediateVertexStride*3);
	immediateMesh.vertexCount += 3;
	immediateVertexOffset += immediateVertexStride*3;
	if (vertexCount > 3)
	{
		for (int32 i = 3; i < vertexCount; i++)
		{
			int32 vertByteOffset = immediateVertexStride*i;
			const void* prevVertex = &((uint8*)vertices)[vertByteOffset-immediateVertexStride];
			
			uint8* dest = &immediateVertexBuffer[immediateVertexOffset];
			MemCpy(dest, prevVertex, immediateVertexStride);
			dest += immediateVertexStride;
			MemCpy(dest, &((uint8*)vertices)[vertByteOffset], immediateVertexStride);
			dest += immediateVertexStride;
			MemCpy(dest, &((uint8*)vertices)[0], immediateVertexStride);

			immediateMesh.vertexCount += 3;
			immediateVertexOffset += immediateVertexStride*3;
		}
	}
}

void Draw_SetViewport(int32 x, int32 y, int32 width, int32 height)
{
	Draw_Flush();
	currentBackend->setViewport(x, y, width, height);
}

void Draw_ClearColor(float r, float g, float b, float a)
{
	Draw_Flush();
	currentBackend->clearColor(r, g, b, a);
}

void Draw_ClearDepth(float value)
{
	Draw_Flush();
	currentBackend->clearDepth(value);
}

void Draw_ClearStencil(int32 value)
{
	Draw_Flush();
	currentBackend->clearStencil(value);
}
