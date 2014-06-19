/**
 * @file K15_Frustum.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2014/06/07
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
 * 
 */

#ifndef _K15Engine_Core_Frustum_h_
#define _K15Engine_Core_Frustum_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

namespace K15_Engine { namespace Core {
	class K15_CORE_API Frustum
	{
	public:
		/*********************************************************************************/
		struct Plane
		{
		  Vector3 normal;
		  Vector3 position;
		}; // end of Frustum::Plane struct declaration
		/*********************************************************************************/

	public:
		/*********************************************************************************/
		enum eFrustumPoints
		{
			FP_NEAR_LEFT_BOTTOM = 0,
			FP_NEAR_RIGHT_BOTTOM,
			FP_NEAR_LEFT_TOP,
			FP_NEAR_RIGHT_TOP,

			FP_FAR_LEFT_BOTTOM,
			FP_FAR_RIGHT_BOTTOM,
			FP_FAR_LEFT_TOP,
			FP_FAR_RIGHT_TOP,

			FP_COUNT
		}; //FrustumPoints
		/*********************************************************************************/
		enum eFrustumPlanes
		{
			CP_FAR = 0,
			CP_NEAR,
			CP_LEFT,
			CP_RIGHT,
			CP_TOP,
			CP_BOTTOM,

			CP_COUNT
		}; //CameraPlanes
		/*********************************************************************************/
		typedef FixedArray(Vector3, FP_COUNT) CornerArray;
		typedef FixedArray(Plane, CP_COUNT) PlaneArray;
		/*********************************************************************************/

	public:
		Frustum();
		~Frustum();

		void calculatePlanes();
		bool isInside(const Vector3& p_Position);

		INLINE void setCorner(const Vector3& p_CornerPos, Enum p_Corner);

		INLINE PlaneArray& getPlanes();
		INLINE CornerArray& getCorners();

		INLINE const Plane& getPlane(Enum p_Plane) const;
		INLINE const Vector3& getCorner(Enum p_Corner) const;

  private:
	  CornerArray m_Corners;
	  PlaneArray m_Planes;
	};// end of Frustum class declaration
#	include "K15_Frustum.inl"
}}// end of K15_Engine::Core namespace

#endif //