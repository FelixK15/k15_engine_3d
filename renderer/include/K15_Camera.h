/**
 * @file K15_Camera.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/09/22
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * @todo implement Orthgraphic projection plane.
 */
#pragma once

#ifndef __K15_CAMERA__
#define __K15_CAMERA__

#include "K15_Object.h"
#include "K15_Matrix4x4.h"
#include "K15_Transformation.h"

using namespace K15_EngineV2;
using namespace K15_Math;

namespace K15_Renderer
{
	class Camera : public Object
	{
	public:
		Camera(bool bPerspective = true);
		Camera(float fHeight,float fWidth,float fNearPlane,float fFarPlane,float fFOV = 0.f,bool bPerspective = true);
		~Camera();

		bool IsPerspective() const;
		bool IsOrthographic() const;
		bool IsActive() const;

		void SetPerspectiveProjection(float fAspectRatio,float fFOV,float fNearPlane,float fFarPlane);
		void SetOrthographicProjection(float fHeight,float fWidth,float fNearPlane,float fFarPlane);

		void SetActive(bool bActive);

		const Matrix4x4F &GetProjectionMatrix() const;
		const Matrix4x4F &GetViewMatrix() const;

	private:
		enum
		{
			VF_DMIN     = 0,  // near
			VF_DMAX     = 1,  // far
			VF_UMIN     = 2,  // bottom
			VF_UMAX     = 3,  // top
			VF_RMIN     = 4,  // left
			VF_RMAX     = 5,  // right
			VF_QUANTITY = 6
		};

	public:
		Transformation Transformation;
	
	private:
		Matrix4x4F m_matProjection;
		Matrix4x4F m_matView;
		float m_fFrustrum[VF_QUANTITY];
		bool m_bActive;
		bool m_bPerspective;
	};
	#include "../src/K15_Camera.inl"
	typedef Pointer<Camera> CameraPtr;
}

#endif //__K15_CAMERA__