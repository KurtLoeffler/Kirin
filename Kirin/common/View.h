#pragma once

#include "common/Standard.h"
#include "common/Space.h"

typedef struct View
{
	bool perspective;
	union {
		struct
		{
			float scale;
		} ortho;

		struct {
			float fov;
			float hFov;
		} persp;
	};
	float aspectRatio;
	float xScale;
	float yScale;
} View;

void View_InitPerspective(View* self, float aspectRatio, float fov);
void View_InitOrtho(View* self, float aspectRatio, float scale);

// updates internal data.
// must be called after changing these values:
// * perspective
// * ortho.scale
// * persp.fov
// * aspectRatio
void View_Update(View* self);

Vec3 View_WorldToView(const View* self, const Vec3* pos);
Vec3 View_WorldToScreen(const View* self, const Vec3* pos, int width, int height);
Vec3 View_ApplyPerspectiveToPoint(const View* self, const Vec3* pos);
// converts world coordinates (x forward, y right, z up) into view coordinates (z forward, y down, x right).
Vec3 View_WorldToViewSwizzle(const Vec3* pos);
// converts view coordinates (z forward, y down, x right) into world coordinates (x forward, y right, z up).
Vec3 View_ViewToWorldSwizzle(const Vec3* pos);
Vec3 View_ViewToScreen(const Vec3* pos, int width, int height);
// create a matrix that transforms from view (z forward, y up, x right) to world (x forward, y right, z up).
const Matrix3* View_GetViewToWorldMatrix();
