#ifndef _K15_Rendering_RenderCameraDesc_h_
#define _K15_Rendering_RenderCameraDesc_h_

#include "K15_RenderPrerequisites.h"

#include <K15_Vector3.h>
#include <K15_Matrix4.h>
#include <K15_Quaternion.h>

enum K15_RenderCameraType
{
	K15_RENDER_CAMERA_TYPE_INVALID = 0,
	K15_RENDER_CAMERA_TYPE_PERSPECTIVE_PROJECTION,
	K15_RENDER_CAMERA_TYPE_ORTHOGONAL_PROJECTION
};

enum K15_RenderCameraFlags
{
	K15_RENDER_CAMERA_FLAG_NONE = 0,
	K15_RENDER_CAMERA_FLAG_DRAW_TO_RENDER_TARGET	= 0x01,		//< Also draw the image to a render target
	K15_RENDER_CAMERA_FLAG_ACTIVATE_CAMERA			= 0x02		//< Active the camera on creation
};

struct K15_RenderCameraDesc
{
	K15_Vector3 position;
	K15_Quaternion orientation;

	float fov;
	float nearPlane;
	float farPlane;
	uint32 flags;

	K15_RenderCameraType type;
};

struct K15_InternalRenderCameraDesc
{
	K15_Matrix4 viewMatrix;
	K15_Matrix4 projectionMatrix;

	K15_RenderTargetHandle renderTargetHandle;
};

#endif //_K15_Rendering_RenderCameraDesc_h_