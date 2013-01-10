/**
 * @file K15_Camera.cpp
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
 */

#include "K15_Camera.h"
#include "K15_Mathematic.h"
#include "windows.h" //@todo remove dependecy of xnamath.h and windows.h
#include "xnamath.h"

using namespace K15_Renderer;
using namespace K15_Math;

Camera::Camera(bool bPerspective)
	: m_bPerspective(bPerspective)
{
	/*Frustrum Points:
				VF_UMAX
				  |    VF_DMAX (Farplane)  
				  |  /
				  | /
				  |/ VF_DMIN (Nearplane)
	VF_RMIN--------------- VF_RMAX
				  |
				  |
				  |
				  |
				VF_UMIN
	*/
	m_fFrustrum[VF_UMAX] = 0.f;
	m_fFrustrum[VF_RMAX] = 0.f;
	m_fFrustrum[VF_UMIN] = 0.f;
	m_fFrustrum[VF_RMIN] = 0.f;
	m_fFrustrum[VF_DMIN] = 0.f;
	m_fFrustrum[VF_DMAX] = 0.f;
}

Camera::Camera(float fHeight,float fWidth,float fNearPlane,float fFarPlane,float fFOV,bool bPerspective)
	: m_bPerspective(bPerspective)
{
	if(bPerspective){
		SetPerspectiveProjection(fWidth/fHeight,fFOV,fNearPlane,fFarPlane);
	}else{
		SetOrthographicProjection(fHeight,fWidth,fNearPlane,fFarPlane);
	}
}

Camera::~Camera()
{

}

void Camera::SetPerspectiveProjection(float fAspectRatio,float fFOV,float fNearPlane,float fFarPlane)
{
	m_bPerspective = true;
	XMMATRIX matPerspective = XMMatrixPerspectiveFovLH(fFOV,fAspectRatio,fNearPlane,fFarPlane);
	XMMATRIX matView = XMMatrixLookAtLH(XMVectorSet(0.f,0.f,-10.f,1.f),XMVectorZero(),XMVectorSet(0.f,1.f,0.f,1.f));
	Memory::MemCopy(&m_matProjection.m_fMatrix,&matPerspective.m,sizeof(Matrix4x4F));
	Memory::MemCopy(&m_matView.m_fMatrix,&matView.m,sizeof(Matrix4x4F));

	float fHalfFOV = 0.5f * fFOV * MatheF::DEGREE_TO_RAD;
 	m_fFrustrum[VF_UMAX] = fNearPlane*MatheF::Tan(fHalfFOV);
 	m_fFrustrum[VF_RMAX] = fAspectRatio*m_fFrustrum[VF_UMAX];
 	m_fFrustrum[VF_UMIN] = -m_fFrustrum[VF_UMAX];
 	m_fFrustrum[VF_RMIN] = -m_fFrustrum[VF_RMAX];
 	m_fFrustrum[VF_DMIN] = fNearPlane;
 	m_fFrustrum[VF_DMAX] = fFarPlane;
}

void Camera::SetOrthographicProjection(float fHeight,float fWidth,float fNearPlane,float fFarPlane)
{
	m_bPerspective = false;
	XMMATRIX matOrthographic = XMMatrixOrthographicLH(fWidth,fHeight,fNearPlane,fFarPlane);
	Memory::MemCopy(&m_matProjection.m_fMatrix,&matOrthographic.m,sizeof(Matrix4x4F));
	//m_matProjection.m_fMatrix = matOrthographic.m;

	m_fFrustrum[VF_UMAX] = 1;
	m_fFrustrum[VF_RMAX] = 1;
	m_fFrustrum[VF_UMIN] = -m_fFrustrum[VF_UMAX];
	m_fFrustrum[VF_RMIN] = -m_fFrustrum[VF_RMAX];
	m_fFrustrum[VF_DMIN] = fNearPlane;
	m_fFrustrum[VF_DMAX] = fFarPlane;
}
