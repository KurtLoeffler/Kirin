#include "common/View.h"

#include "common/Math.h"

void View_InitPerspective(View* self, float aspectRatio, float fov)
{
	*self = (View){ 0 };

	self->perspective = true;
	self->aspectRatio = aspectRatio;
	self->persp.fov = fov;
	View_Update(self);
}

void View_InitOrtho(View* self, float aspectRatio, float scale)
{
	*self = (View){ 0 };

	self->ortho.scale = scale;
	self->aspectRatio = aspectRatio;
	View_Update(self);
}

void View_Update(View* self)
{
	if (self->perspective)
	{
		self->persp.hFov = 2*Atan(Tan(self->persp.fov*0.5f)*self->aspectRatio);

		self->yScale = 1.0f/(2*Tan(self->persp.fov*0.5f));
		self->xScale = 1.0f/(2*Tan(self->persp.hFov*0.5f));
	}
	else
	{
		self->xScale = self->aspectRatio*self->ortho.scale;
		self->yScale = self->ortho.scale;
	}
}

Vec3 View_WorldToView(const View* self, const Vec3* pos)
{
	Vec3 result;
	if (self->perspective)
	{
		result = View_WorldToViewSwizzle(pos);
		result = View_ApplyPerspectiveToPoint(self, &result);
	}
	else
	{
		result.x = pos->x*self->xScale;
		result.y = pos->y*self->yScale;
	}
	return result;
}

Vec3 View_WorldToScreen(const View* self, const Vec3* pos, int width, int height)
{
	Vec3 result;
	result = View_WorldToView(self, pos);
	return View_ViewToScreen(&result, width, height);
}

Vec3 View_ApplyPerspectiveToPoint(const View* self, const Vec3* pos)
{
	float inverseZ = 1.0f/pos->z;
	Vec3 result;
	result.x = pos->x*inverseZ*self->xScale;
	result.y = pos->y*inverseZ*self->yScale;
	return result;
}

Vec3 View_WorldToViewSwizzle(const Vec3* pos)
{
	return (Vec3) { pos->y, -pos->z, pos->x };
}

Vec3 View_ViewToWorldSwizzle(const Vec3* pos)
{
	return (Vec3) { pos->z, pos->x, -pos->y };
}

Vec3 View_ViewToScreen(const Vec3* pos, int width, int height)
{
	Vec3 result;
	result.x = (pos->x+0.5f)*width;
	result.y = (1.0f-(pos->y+0.5f))*height;
	return result;
}

const Matrix3* View_GetViewToWorldMatrix()
{
	static Matrix3 matrix = {
		{
			0, 1, 0,
			0, 0, 1,
			1, 0, 0
		}
	};
	return &matrix;
}
