/**
 * @file K15_AABB.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/12/16
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
 */

#ifndef _K15Engine_Rendering_AABB_h_
#define _K15Engine_Rendering_AABB_h_

#include "K15_Prerequisites.h"

#include "K15_Vector3.h"

namespace K15_Engine { namespace Rendering {
	class K15_CORE_API AABB
	{
	public:
		/*********************************************************************************/
		enum eCornerTypes
		{
			CT_FAR_LEFT_TOP = 0,
			CT_FAR_RIGHT_TOP,
			CT_FAR_LEFT_BOTTOM,
			CT_FAR_RIGHT_BOTTOM,

			CT_NEAR_LEFT_TOP,
			CT_NEAR_RIGHT_TOP,
			CT_NEAR_LEFT_BOTTOM,
			CT_NEAR_RIGHT_BOTTOM,

			CT_COUNT
		};//CornerTypes
		/*********************************************************************************/

		/*********************************************************************************/
		typedef FixedArray(Vector3, CT_COUNT) CornerArray;
		/*********************************************************************************/
	public:
		AABB();
		AABB(const Vector3& p_Min, const Vector3& p_Max);
		~AABB();

		INLINE Vector3 getFarLeftTop() const;
		INLINE Vector3 getFarLeftBottom() const;
		INLINE Vector3 getFarRightTop() const;
		INLINE Vector3 getFarRightBottom() const;

		INLINE Vector3 getNearLeftTop() const;
		INLINE Vector3 getNearLeftBottom() const;
		INLINE Vector3 getNearRightTop() const;
		INLINE Vector3 getNearRightBottom() const;

		INLINE CornerArray getCorners() const;

		INLINE const Vector3& getMax() const;
		INLINE const Vector3& getMin() const;

		INLINE void setMin(const Vector3& p_Min);
		INLINE void setMax(const Vector3& p_Max);
		
		INLINE void merge(const AABB& p_AABB);

		INLINE bool operator>(const AABB& p_Rhs);
		INLINE bool operator<(const AABB& p_Rhs);
	
	private:
		Vector3 m_Min;
		Vector3 m_Max;
	};// end of AABB class declaration
#include "K15_AABB.inl"
}}// end of K15_Engine::Rendering namespace

#endif //_K15Engine_Rendering_AABB_h_